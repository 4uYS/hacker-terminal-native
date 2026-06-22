/*
 * 工具函数模块
 */

#include "hacker.h"

// 去除首尾空白
void Trim(TCHAR* str) {
    if (!str) return;
    
    int len = _tcslen(str);
    int start = 0, end = len - 1;
    
    while (start < len && _istspace(str[start])) start++;
    while (end > start && _istspace(str[end])) end--;
    
    if (start > 0) {
        _tcscpy(str, str + start);
    }
    str[end - start + 1] = 0;
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
    if (!dest || !src) return;
    int dest_len = _tcslen(dest);
    int src_len = _tcslen(src);
    if (dest_len + src_len < max_len - 1) {
        _tcscat(dest, src);
    }
}
