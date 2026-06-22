/*
 * 技能系统模块
 */

#include "hacker.h"

SkillManager g_skills;
TCHAR g_skills_dir[MAX_PATH] = {0};

// 初始化技能系统
void InitSkills() {
    g_skills.head = NULL;
    g_skills.count = 0;
    
    // 设置技能目录
    GetAppDir(g_skills_dir);
    _tcscat(g_skills_dir, _T("skills\\"));
    
    // 扫描技能
    ScanSkills();
}

// 扫描技能目录
void ScanSkills() {
    FreeSkills();
    
    WIN32_FIND_DATA findData;
    TCHAR searchPath[MAX_PATH];
    _stprintf(searchPath, _T("%s*"), g_skills_dir);
    
    HANDLE hFind = FindFirstFile(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }
    
    do {
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            _tcscmp(findData.cFileName, _T(".")) != 0 &&
            _tcscmp(findData.cFileName, _T("..")) != 0) {
            
            // 检查是否有SKILL.md
            TCHAR skillPath[MAX_PATH];
            _stprintf(skillPath, _T("%s%s\\SKILL.md"), g_skills_dir, findData.cFileName);
            
            if (PathFileExists(skillPath)) {
                SkillNode* node = (SkillNode*)malloc(sizeof(SkillNode));
                _tcscpy(node->name, findData.cFileName);
                _stprintf(node->path, _T("%s%s\\"), g_skills_dir, findData.cFileName);
                
                // 检查是否有代码文件
                TCHAR codePath[MAX_PATH];
                _stprintf(codePath, _T("%s%s\\skill.dll"), g_skills_dir, findData.cFileName);
                node->has_code = PathFileExists(codePath);
                
                node->next = g_skills.head;
                g_skills.head = node;
                g_skills.count++;
            }
        }
    } while (FindNextFile(hFind, &findData));
    
    FindClose(hFind);
}

// 释放技能列表
void FreeSkills() {
    SkillNode* node = g_skills.head;
    while (node) {
        SkillNode* next = node->next;
        free(node);
        node = next;
    }
    g_skills.head = NULL;
    g_skills.count = 0;
}

// 查找技能
SkillNode* FindSkill(LPCTSTR name) {
    SkillNode* node = g_skills.head;
    while (node) {
        if (StrEquals(node->name, name)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// 列出技能文本
void ListSkillsText(TCHAR* buffer, int max_len) {
    buffer[0] = 0;
    
    if (g_skills.count == 0) {
        SafeAppend(buffer, max_len, _T("暂无可用技能\n"));
        return;
    }
    
    TCHAR line[256];
    _stprintf(line, _T("可用技能 (%d个):\n\n"), g_skills.count);
    SafeAppend(buffer, max_len, line);
    
    SkillNode* node = g_skills.head;
    while (node) {
        _stprintf(line, _T("  - %s%s\n"), 
            node->name, 
            node->has_code ? _T(" [可执行]") : _T(" [文档]"));
        SafeAppend(buffer, max_len, line);
        node = node->next;
    }
}

// 读取技能文档
LPCTSTR ReadSkillDoc(LPCTSTR name) {
    static TCHAR result[8192];
    result[0] = 0;
    
    SkillNode* skill = FindSkill(name);
    if (!skill) {
        _stprintf(result, _T("技能不存在: %s"), name);
        return result;
    }
    
    TCHAR docPath[MAX_PATH];
    _stprintf(docPath, _T("%sSKILL.md"), skill->path);
    
    HANDLE hFile = CreateFile(docPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        _stprintf(result, _T("无法读取技能文档: %s"), name);
        return result;
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize > 8000) fileSize = 8000;
    
    char* buffer = (char*)malloc(fileSize + 1);
    DWORD bytesRead = 0;
    ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
    buffer[bytesRead] = 0;
    
    CloseHandle(hFile);
    
    // 转换为UTF-16
    int wLen = MultiByteToWideChar(CP_UTF8, 0, buffer, bytesRead, NULL, 0);
    if (wLen > 0 && wLen < 8000) {
        MultiByteToWideChar(CP_UTF8, 0, buffer, bytesRead, result, wLen);
        result[wLen] = 0;
    }
    
    free(buffer);
    return result;
}

// 执行技能
LPCTSTR ExecuteSkill(LPCTSTR name, LPCTSTR params) {
    static TCHAR result[4096];
    
    SkillNode* skill = FindSkill(name);
    if (!skill) {
        _stprintf(result, _T("技能不存在: %s"), name);
        return result;
    }
    
    if (!skill->has_code) {
        _stprintf(result, _T("技能 %s 暂无可执行代码，仅有文档说明"), name);
        return result;
    }
    
    // 这里可以加载DLL并执行
    // 简化版本：返回提示
    _stprintf(result, _T("执行技能: %s\n参数: %s\n\n(技能DLL执行功能待实现)"), name, params);
    return result;
}
