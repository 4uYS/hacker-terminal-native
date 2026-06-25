/*
 * 上下文管理模块
 */
#include "hacker.h"

ContextManager g_context;

// 初始化上下文
void InitContext() {
    g_context.head = NULL;
    g_context.tail = NULL;
    g_context.count = 0;
    g_context.max_history = 50;
}

// 释放上下文
void FreeContext() {
    MessageNode* node = g_context.head;
    while (node) {
        MessageNode* next = node->next;
        if (node->content) free(node->content);
        free(node);
        node = next;
    }
    g_context.head = NULL;
    g_context.tail = NULL;
    g_context.count = 0;
}

// 添加消息
void AddMessage(LPCTSTR role, LPCTSTR content) {
    if (!role || !content) return;
    
    MessageNode* node = (MessageNode*)malloc(sizeof(MessageNode));
    if (!node) return;
    
    ZeroMemory(node, sizeof(MessageNode));
    _tcsncpy(node->role, role, 31);
    node->role[31] = 0;
    node->content = StrDup(content);
    node->next = NULL;
    
    if (!node->content) {
        free(node);
        return;
    }
    
    if (g_context.tail) {
        g_context.tail->next = node;
    } else {
        g_context.head = node;
    }
    g_context.tail = node;
    g_context.count++;
    
    // 限制历史数量
    while (g_context.count > g_context.max_history && g_context.head) {
        MessageNode* old = g_context.head;
        g_context.head = old->next;
        if (old->content) free(old->content);
        free(old);
        g_context.count--;
    }
    
    if (!g_context.head) {
        g_context.tail = NULL;
    }
}

// 清空上下文
void ClearContext() {
    FreeContext();
}

// 辅助函数：写入文件
static void WriteToFile(HANDLE hFile, LPCTSTR text) {
    if (!hFile || !text) return;
    DWORD written;
    WriteFile(hFile, text, _tcslen(text) * sizeof(TCHAR), &written, NULL);
}

// 导出上下文为JSON
int ExportContext(LPCTSTR path) {
    if (!path) return -1;
    
    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }
    
    WriteToFile(hFile, _T("{\n  \"messages\": [\n"));
    
    MessageNode* node = g_context.head;
    int index = 0;
    
    while (node) {
        TCHAR escaped_role[64];
        TCHAR escaped_content[4096];
        
        JsonEscape(node->role, escaped_role, 64);
        JsonEscape(node->content, escaped_content, 4096);
        
        TCHAR line[8192];
        _stprintf(line, 
            _T("    {\"role\": \"%s\", \"content\": \"%s\"}%s\n"),
            escaped_role,
            escaped_content,
            node->next ? _T(",") : _T("")
        );
        WriteToFile(hFile, line);
        node = node->next;
        index++;
    }
    
    WriteToFile(hFile, _T("  ]\n}\n"));
    
    CloseHandle(hFile);
    return index;
}

// 辅助函数：从 JSON 字符串中提取字段值
static BOOL ExtractJsonField(LPCTSTR json, LPCTSTR field, TCHAR* value, int max_len) {
    if (!json || !field || !value || max_len <= 0) return FALSE;
    
    TCHAR search[128];
    _stprintf(search, _T("\"%s\":"), field);
    
    LPCTSTR pos = _tcsstr(json, search);
    if (!pos) return FALSE;
    
    pos += _tcslen(search);
    
    // 跳过空白
    while (*pos && _istspace(*pos)) pos++;
    
    // 查找引号
    if (*pos != _T('"')) return FALSE;
    pos++;
    
    // 提取值
    int i = 0;
    while (*pos && *pos != _T('"') && i < max_len - 1) {
        if (*pos == _T('\\') && *(pos + 1)) {
            pos++;
            switch (*pos) {
                case _T('n'): value[i++] = _T('\n'); break;
                case _T('r'): value[i++] = _T('\r'); break;
                case _T('t'): value[i++] = _T('\t'); break;
                case _T('\\'): value[i++] = _T('\\'); break;
                case _T('"'): value[i++] = _T('"'); break;
                case _T('/'): value[i++] = _T('/'); break;
                default: value[i++] = *pos; break;
            }
        } else {
            value[i++] = *pos;
        }
        pos++;
    }
    value[i] = 0;
    
    return i > 0;
}

// 导入上下文
int ImportContext(LPCTSTR path) {
    if (!path) return -1;
    
    HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }
    
    // 读取文件内容
    DWORD file_size = GetFileSize(hFile, NULL);
    if (file_size == 0 || file_size > 10 * 1024 * 1024) {
        CloseHandle(hFile);
        return -1;
    }
    
    TCHAR* content = (TCHAR*)malloc(file_size + sizeof(TCHAR));
    if (!content) {
        CloseHandle(hFile);
        return -1;
    }
    
    DWORD bytes_read;
    ReadFile(hFile, content, file_size, &bytes_read, NULL);
    content[bytes_read / sizeof(TCHAR)] = 0;
    CloseHandle(hFile);
    
    // 清空现有上下文
    ClearContext();
    
    // 解析每条消息
    int count = 0;
    LPCTSTR search = content;
    
    while ((search = _tcsstr(search, _T("\"role\""))) != NULL) {
        TCHAR role[64] = {0};
        TCHAR msg_content[4096] = {0};
        
        // 回退到对象开始
        LPCTSTR obj_start = search;
        while (obj_start > content && *obj_start != _T('{')) obj_start--;
        
        if (ExtractJsonField(obj_start, _T("role"), role, 64) &&
            ExtractJsonField(obj_start, _T("content"), msg_content, 4096)) {
            AddMessage(role, msg_content);
            count++;
        }
        
        search += 5; // 跳过 "role"
    }
    
    free(content);
    
    if (count == 0) {
        // 如果没有解析到消息，添加一条提示
        AddMessage(_T("system"), _T("上下文导入完成"));
        return 1;
    }
    
    return count;
}

// 获取上下文消息（JSON格式）
void GetContextMessages(TCHAR* buffer, int max_len) {
    if (!buffer || max_len <= 0) return;
    
    buffer[0] = 0;
    SafeAppend(buffer, max_len, _T("["));
    
    MessageNode* node = g_context.head;
    while (node) {
        TCHAR escaped_role[64];
        TCHAR escaped_content[2048];
        
        JsonEscape(node->role, escaped_role, 64);
        JsonEscape(node->content, escaped_content, 2048);
        
        TCHAR msg[4096];
        _stprintf(msg, 
            _T("{\"role\":\"%s\",\"content\":\"%s\"}"),
            escaped_role,
            escaped_content
        );
        SafeAppend(buffer, max_len, msg);
        
        if (node->next) {
            SafeAppend(buffer, max_len, _T(","));
        }
        node = node->next;
    }
    
    SafeAppend(buffer, max_len, _T("]"));
}
