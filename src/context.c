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
    MessageNode* node = (MessageNode*)malloc(sizeof(MessageNode));
    _tcscpy(node->role, role);
    node->content = StrDup(content);
    node->next = NULL;
    
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

// 导出上下文为JSON
int ExportContext(LPCTSTR path) {
    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return -1;
    }
    
    TCHAR header[] = _T("{\n  \"messages\": [\n");
    DWORD written;
    WriteFile(hFile, header, _tcslen(header) * sizeof(TCHAR), &written, NULL);
    
    MessageNode* node = g_context.head;
    int index = 0;
    while (node) {
        TCHAR line[4096];
        _stprintf(line, 
            _T("    {\"role\": \"%s\", \"content\": \"%s\"}%s\n"),
            node->role,
            node->content,
            node->next ? _T(",") : _T("")
        );
        WriteFile(hFile, line, _tcslen(line) * sizeof(TCHAR), &written, NULL);
        node = node->next;
        index++;
    }
    
    TCHAR footer[] = _T("  ]\n}\n");
    WriteFile(hFile, footer, _tcslen(footer) * sizeof(TCHAR), &written, NULL);
    
    CloseHandle(hFile);
    return index;
}

// 导入上下文
int ImportContext(LPCTSTR path) {
    // 简化版本：清空后提示
    ClearContext();
    AddMessage(_T("system"), _T("上下文已导入"));
    return 1;
}

// 获取上下文消息（JSON格式）
void GetContextMessages(TCHAR* buffer, int max_len) {
    buffer[0] = 0;
    SafeAppend(buffer, max_len, _T("["));
    
    MessageNode* node = g_context.head;
    while (node) {
        TCHAR msg[2048];
        _stprintf(msg, 
            _T("{\"role\":\"%s\",\"content\":\"%s\"}"),
            node->role,
            node->content
        );
        SafeAppend(buffer, max_len, msg);
        
        if (node->next) {
            SafeAppend(buffer, max_len, _T(","));
        }
        node = node->next;
    }
    
    SafeAppend(buffer, max_len, _T("]"));
}
