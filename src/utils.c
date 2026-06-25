/*
 * 工具函数模块
 */

#include "hacker.h"

// 去除首尾空白
void Trim(TCHAR* str) {
    if (!str) return;
    
    int len = _tcslen(str);
    if (len == 0) return;
    
    int start = 0, end = len - 1;
    
    while (start < len && _istspace(str[start])) start++;
    while (end > start && _istspace(str[end])) end--;
    
    int new_len = end - start + 1;
    if (new_len <= 0) {
        str[0] = 0;
        return;
    }
    
    // 使用 memmove 安全处理重叠内存区域
    if (start > 0) {
        memmove(str, str + start, new_len * sizeof(TCHAR));
    }
    str[new_len] = 0;
}

// 转小写
void ToLowerCase(TCHAR* str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = _totlower(str[i]);
    }
}

// 判断前缀
BOOL StartsWith(LPCTSTR str, LPCTSTR prefix) {
    if (!str || !prefix) return FALSE;
    return _tcsncmp(str, prefix, _tcslen(prefix)) == 0;
}

// 字符串相等
BOOL StrEquals(LPCTSTR a, LPCTSTR b) {
    if (!a || !b) return FALSE;
    return _tcscmp(a, b) == 0;
}

// 字符串复制
TCHAR* StrDup(LPCTSTR str) {
    if (!str) return NULL;
    int len = _tcslen(str);
    TCHAR* dup = (TCHAR*)malloc((len + 1) * sizeof(TCHAR));
    if (dup) {
        _tcscpy(dup, str);
    }
    return dup;
}

// 获取程序目录
void GetAppDir(TCHAR* buffer) {
    GetModuleFileName(NULL, buffer, MAX_PATH);
    TCHAR* lastSlash = _tcsrchr(buffer, _T('\\'));
    if (lastSlash) {
        *(lastSlash + 1) = 0;
    }
}

// 确保目录存在
void EnsureDirExists(LPCTSTR path) {
    TCHAR temp[MAX_PATH];
    _tcscpy(temp, path);
    
    for (TCHAR* p = temp + 1; *p; p++) {
        if (*p == _T('\\')) {
            *p = 0;
            CreateDirectory(temp, NULL);
            *p = _T('\\');
        }
    }
    CreateDirectory(path, NULL);
}

// 整数转字符串
void IntToStr(int value, TCHAR* buffer) {
    _stprintf(buffer, _T("%d"), value);
}

// 安全追加字符串
void SafeAppend(TCHAR* dest, int max_len, LPCTSTR src) {
    if (!dest || !src || max_len <= 0) return;
    
    int dest_len = _tcslen(dest);
    int src_len = _tcslen(src);
    
    // 确保有足够空间：dest_len + src_len + 1(null终止符) <= max_len
    if (dest_len + src_len + 1 <= max_len) {
        _tcscat(dest, src);
    } else if (dest_len < max_len - 1) {
        // 空间不足时，尽可能追加部分内容
        int can_copy = max_len - dest_len - 1;
        if (can_copy > 0) {
            _tcsncat(dest, src, can_copy);
            dest[max_len - 1] = 0;
        }
    }
}

// JSON 字符串转义
// 转义特殊字符：" \ \n \r \t 等
int JsonEscape(LPCTSTR src, TCHAR* dest, int dest_max_len) {
    if (!src || !dest || dest_max_len <= 0) return 0;
    
    int dest_len = 0;
    dest[0] = 0;
    
    for (int i = 0; src[i]; i++) {
        TCHAR escaped[8];
        int esc_len = 0;
        
        switch (src[i]) {
            case _T('"'):  escaped[0] = _T('\\'); escaped[1] = _T('"');  esc_len = 2; break;
            case _T('\\'): escaped[0] = _T('\\'); escaped[1] = _T('\\'); esc_len = 2; break;
            case _T('\n'): escaped[0] = _T('\\'); escaped[1] = _T('n');  esc_len = 2; break;
            case _T('\r'): escaped[0] = _T('\\'); escaped[1] = _T('r');  esc_len = 2; break;
            case _T('\t'): escaped[0] = _T('\\'); escaped[1] = _T('t');  esc_len = 2; break;
            case _T('\b'): escaped[0] = _T('\\'); escaped[1] = _T('b');  esc_len = 2; break;
            case _T('\f'): escaped[0] = _T('\\'); escaped[1] = _T('f');  esc_len = 2; break;
            default:
                escaped[0] = src[i];
                esc_len = 1;
                break;
        }
        
        if (dest_len + esc_len + 1 > dest_max_len) {
            break;
        }
        
        _tcsncpy(dest + dest_len, escaped, esc_len);
        dest_len += esc_len;
        dest[dest_len] = 0;
    }
    
    return dest_len;
}
