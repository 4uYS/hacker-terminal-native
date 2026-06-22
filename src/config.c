/*
 * 配置管理模块
 * 使用 Windows INI 文件 API
 */

#include "hacker.h"

HackerConfig g_config;
TCHAR g_config_dir[MAX_PATH] = {0};
TCHAR g_config_file[MAX_PATH] = {0};

// 权限等级名称表
static LPCTSTR levelNames[] = {
    _T("只读模式"),
    _T("查看模式"),
    _T("文档模式"),
    _T("文件管理"),
    _T("开发模式"),
    _T("标准模式"),
    _T("高级模式"),
    _T("管理员模式"),
    _T("高级管理员"),
    _T("几乎完全控制"),
    _T("完全控制"),
};

// 初始化配置
void InitConfig() {
    // 设置配置目录
    TCHAR appData[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appData);
    _stprintf(g_config_dir, _T("%s\\HackerTerminal"), appData);
    _stprintf(g_config_file, _T("%s\\config.ini"), g_config_dir);
    
    // 确保目录存在
    EnsureDirExists(g_config_dir);
    
    // 加载配置
    LoadConfig();
}

// 加载配置
void LoadConfig() {
    // 默认值
    g_config.permission_level = 5;
    g_config.operation_mode = MODE_CLI;
    _tcscpy(g_config.ai_backend, _T("cloud"));
    g_config.cloud_api_url[0] = 0;
    g_config.cloud_api_key[0] = 0;
    g_config.cloud_model_name[0] = 0;
    g_config.share_os_info = TRUE;
    g_config.share_username = FALSE;
    g_config.share_timezone = TRUE;
    g_config.share_skills_list = TRUE;
    g_config.show_welcome = TRUE;
    g_config.show_skill_hints = TRUE;
    g_config.confirm_dangerous = TRUE;
    g_config.installed = FALSE;
    
    if (!PathFileExists(g_config_file)) {
        return;
    }
    
    // 读取 INI 配置
    g_config.permission_level = GetPrivateProfileInt(
        _T("General"), _T("PermissionLevel"), 5, g_config_file);
    
    g_config.operation_mode = GetPrivateProfileInt(
        _T("General"), _T("OperationMode"), MODE_CLI, g_config_file);
    
    GetPrivateProfileString(
        _T("AI"), _T("Backend"), _T("cloud"),
        g_config.ai_backend, 64, g_config_file);
    
    GetPrivateProfileString(
        _T("AI"), _T("CloudApiUrl"), _T(""),
        g_config.cloud_api_url, 512, g_config_file);
    
    GetPrivateProfileString(
        _T("AI"), _T("CloudApiKey"), _T(""),
        g_config.cloud_api_key, 256, g_config_file);
    
    GetPrivateProfileString(
        _T("AI"), _T("CloudModelName"), _T(""),
        g_config.cloud_model_name, 128, g_config_file);
    
    g_config.share_os_info = GetPrivateProfileInt(
        _T("Privacy"), _T("ShareOsInfo"), 1, g_config_file);
    
    g_config.share_username = GetPrivateProfileInt(
        _T("Privacy"), _T("ShareUsername"), 0, g_config_file);
    
    g_config.share_timezone = GetPrivateProfileInt(
        _T("Privacy"), _T("ShareTimezone"), 1, g_config_file);
    
    g_config.share_skills_list = GetPrivateProfileInt(
        _T("Privacy"), _T("ShareSkillsList"), 1, g_config_file);
    
    g_config.show_welcome = GetPrivateProfileInt(
        _T("UI"), _T("ShowWelcome"), 1, g_config_file);
    
    g_config.confirm_dangerous = GetPrivateProfileInt(
        _T("Security"), _T("ConfirmDangerous"), 1, g_config_file);
    
    g_config.installed = GetPrivateProfileInt(
        _T("General"), _T("Installed"), 0, g_config_file);
}

// 保存配置
void SaveConfig() {
    TCHAR buf[32];
    
    WritePrivateProfileString(_T("General"), _T("PermissionLevel"), 
        _itot(g_config.permission_level, buf, 10), g_config_file);
    
    WritePrivateProfileString(_T("General"), _T("OperationMode"), 
        _itot(g_config.operation_mode, buf, 10), g_config_file);
    
    WritePrivateProfileString(_T("General"), _T("Installed"), 
        g_config.installed ? _T("1") : _T("0"), g_config_file);
    
    WritePrivateProfileString(_T("AI"), _T("Backend"), 
        g_config.ai_backend, g_config_file);
    
    WritePrivateProfileString(_T("AI"), _T("CloudApiUrl"), 
        g_config.cloud_api_url, g_config_file);
    
    WritePrivateProfileString(_T("AI"), _T("CloudApiKey"), 
        g_config.cloud_api_key, g_config_file);
    
    WritePrivateProfileString(_T("AI"), _T("CloudModelName"), 
        g_config.cloud_model_name, g_config_file);
    
    WritePrivateProfileString(_T("Privacy"), _T("ShareOsInfo"), 
        g_config.share_os_info ? _T("1") : _T("0"), g_config_file);
    
    WritePrivateProfileString(_T("Privacy"), _T("ShareUsername"), 
        g_config.share_username ? _T("1") : _T("0"), g_config_file);
    
    WritePrivateProfileString(_T("Privacy"), _T("ShareTimezone"), 
        g_config.share_timezone ? _T("1") : _T("0"), g_config_file);
    
    WritePrivateProfileString(_T("Privacy"), _T("ShareSkillsList"), 
        g_config.share_skills_list ? _T("1") : _T("0"), g_config_file);
    
    WritePrivateProfileString(_T("UI"), _T("ShowWelcome"), 
        g_config.show_welcome ? _T("1") : _T("0"), g_config_file);
    
    WritePrivateProfileString(_T("Security"), _T("ConfirmDangerous"), 
        g_config.confirm_dangerous ? _T("1") : _T("0"), g_config_file);
}

// 获取权限等级
int GetPermissionLevel() {
    return g_config.permission_level;
}

// 设置权限等级
void SetPermissionLevel(int level) {
    if (level < 0) level = 0;
    if (level > 10) level = 10;
    g_config.permission_level = level;
    SaveConfig();
}

// 获取操作模式
int GetOperationMode() {
    return g_config.operation_mode;
}

// 设置操作模式
void SetOperationMode(int mode) {
    if (mode < 0) mode = 0;
    if (mode > 2) mode = 2;
    g_config.operation_mode = mode;
    SaveConfig();
}

// 获取权限等级名称
LPCTSTR GetPermissionLevelName(int level) {
    if (level < 0 || level > 10) {
        return _T("未知");
    }
    return levelNames[level];
}
