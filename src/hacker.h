/*
 * Hacker Terminal v1.0 - 纯原生 Win32 版本
 * 爱折腾实验室 · 豆包 AI Agent Lab
 * 
 * 仅使用 Windows 系统 API，零第三方库依赖
 */

#pragma once

#ifndef HACKER_TERMINAL_H
#define HACKER_TERMINAL_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// ==================== 版本信息 ====================
#define APP_NAME        TEXT("Hacker Terminal")
#define APP_VERSION     TEXT("v1.0")
#define APP_AUTHOR      TEXT("爱折腾实验室 · 豆包 AI Agent Lab")
#define APP_CODENAME    TEXT("Hacker")

// ==================== 权限等级 ====================
#define PERM_LEVEL_0    0   // 只读模式
#define PERM_LEVEL_1    1   // 查看模式
#define PERM_LEVEL_2    2   // 文档模式
#define PERM_LEVEL_3    3   // 文件管理
#define PERM_LEVEL_4    4   // 开发模式
#define PERM_LEVEL_5    5   // 标准模式
#define PERM_LEVEL_6    6   // 高级模式
#define PERM_LEVEL_7    7   // 管理员模式
#define PERM_LEVEL_8    8   // 高级管理员
#define PERM_LEVEL_9    9   // 几乎完全控制
#define PERM_LEVEL_10   10  // 完全控制

// ==================== 操作模式 ====================
#define MODE_CLI        0   // 仅命令行
#define MODE_GUI        1   // 仅模拟点击
#define MODE_BOTH       2   // 混合模式

// ==================== 颜色定义 ====================
#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_LIGHTGRAY     7
#define COLOR_DARKGRAY      8
#define COLOR_LIGHTBLUE     9
#define COLOR_LIGHTGREEN    10
#define COLOR_LIGHTCYAN     11
#define COLOR_LIGHTRED      12
#define COLOR_LIGHTMAGENTA  13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

// 背景色
#define BG_BLUE         (COLOR_BLUE << 4)
#define BG_GREEN        (COLOR_GREEN << 4)
#define BG_RED          (COLOR_RED << 4)
#define BG_CYAN         (COLOR_CYAN << 4)
#define BG_MAGENTA      (COLOR_MAGENTA << 4)
#define BG_YELLOW       (COLOR_BROWN << 4)
#define BG_WHITE        (COLOR_LIGHTGRAY << 4)

// ==================== 配置结构 ====================
typedef struct {
    int permission_level;
    int operation_mode;
    
    // AI 模型配置
    TCHAR ai_backend[64];      // local / cloud
    TCHAR cloud_api_url[512];
    TCHAR cloud_api_key[256];
    TCHAR cloud_model_name[128];
    
    // 隐私设置
    BOOL share_os_info;
    BOOL share_username;
    BOOL share_timezone;
    BOOL share_skills_list;
    
    // UI 设置
    BOOL show_welcome;
    BOOL show_skill_hints;
    
    // 安全设置
    BOOL confirm_dangerous;
    
    // 安装状态
    BOOL installed;
} HackerConfig;

// ==================== 上下文消息 ====================
typedef struct MessageNode {
    TCHAR role[32];
    TCHAR* content;
    struct MessageNode* next;
} MessageNode;

typedef struct {
    MessageNode* head;
    MessageNode* tail;
    int count;
    int max_history;
} ContextManager;

// ==================== 技能信息 ====================
typedef struct SkillNode {
    TCHAR name[64];
    TCHAR path[MAX_PATH];
    BOOL has_code;
    struct SkillNode* next;
} SkillNode;

typedef struct {
    SkillNode* head;
    int count;
} SkillManager;

// ==================== 全局变量声明 ====================
extern HANDLE hConsole;
extern HackerConfig g_config;
extern ContextManager g_context;
extern SkillManager g_skills;
extern TCHAR g_skills_dir[MAX_PATH];
extern TCHAR g_config_dir[MAX_PATH];
extern TCHAR g_config_file[MAX_PATH];

// ==================== 函数声明 ====================

// terminal.c
void InitTerminal();
void RestoreTerminal();
void SetColor(WORD color);
void ResetColor();
void ClearScreen();
void PrintBanner();
void PrintUser(LPCTSTR text);
void PrintAssistant(LPCTSTR text);
void PrintTool(LPCTSTR name);
void PrintGui(LPCTSTR action);
void PrintResult(LPCTSTR text);
void PrintError(LPCTSTR text);
void PrintSuccess(LPCTSTR text);
void PrintInfo(LPCTSTR text);
void PrintHacker(LPCTSTR text);
void PrintWarning(LPCTSTR text);
void PrintPermissionInfo();
void PrintGuiTools();
int  ReadLine(TCHAR* buffer, int max_len);

// config.c
void InitConfig();
void LoadConfig();
void SaveConfig();
int  GetPermissionLevel();
void SetPermissionLevel(int level);
int  GetOperationMode();
void SetOperationMode(int mode);
LPCTSTR GetPermissionLevelName(int level);

// gui.c
BOOL InitGuiControl();
BOOL GuiIsAvailable();
LPCTSTR GuiExecute(LPCTSTR action, LPCTSTR params);
void GuiGetPosition(int* x, int* y);
void GuiGetSize(int* width, int* height);
BOOL GuiScreenshot(LPCTSTR path);

// shell.c
int     CheckDangerousCommand(LPCTSTR command);
BOOL    CanExecuteCommand(LPCTSTR command, int required_level);
LPCTSTR ExecuteShellCommand(LPCTSTR command);
LPCTSTR GetSystemInfoText();
LPCTSTR ReadFileContent(LPCTSTR path);
LPCTSTR ListDirectory(LPCTSTR path);
LPCTSTR WriteFileContent(LPCTSTR path, LPCTSTR content);

// ocr.c
BOOL    InitOcr();
BOOL    OcrIsAvailable();
LPCTSTR OcrImageFile(LPCTSTR imagePath);
LPCTSTR OcrScreenRegion(int x, int y, int width, int height);
LPCTSTR OcrFullScreen();

// ggml.c
BOOL    InitGGML();
BOOL    ModelIsAvailable();
LPCTSTR GetModelStatus();
BOOL    LoadGGUFModel(LPCTSTR path);
void    UnloadModel();
int     EncodeToken(LPCTSTR text);
LPCTSTR DecodeToken(int token);
int     ModelInfer(int* input_tokens, int n_input);
LPCTSTR ModelGenerate(LPCTSTR prompt, int max_tokens);
LPCTSTR GetModelInfoText();
void    ClearModelContext();

// network.c
BOOL InitNetwork();
LPCTSTR HttpPostJson(LPCTSTR url, LPCTSTR jsonData, LPCTSTR apiKey);
LPCTSTR CallCloudAI(LPCTSTR messages);

// skills.c
void InitSkills();
void ScanSkills();
void FreeSkills();
SkillNode* FindSkill(LPCTSTR name);
void ListSkillsText(TCHAR* buffer, int max_len);
LPCTSTR ReadSkillDoc(LPCTSTR name);
LPCTSTR ExecuteSkill(LPCTSTR name, LPCTSTR params);

// context.c
void InitContext();
void FreeContext();
void AddMessage(LPCTSTR role, LPCTSTR content);
void ClearContext();
int  ExportContext(LPCTSTR path);
int  ImportContext(LPCTSTR path);
void GetContextMessages(TCHAR* buffer, int max_len);

// ai.c
void InitAI();
LPCTSTR GenerateResponse(LPCTSTR user_input);
LPCTSTR GetSystemPrompt();

// utils.c
void Trim(TCHAR* str);
void ToLowerCase(TCHAR* str);
BOOL StartsWith(LPCTSTR str, LPCTSTR prefix);
BOOL StrEquals(LPCTSTR a, LPCTSTR b);
TCHAR* StrDup(LPCTSTR str);
void GetAppDir(TCHAR* buffer);
void EnsureDirExists(LPCTSTR path);
void IntToStr(int value, TCHAR* buffer);
void SafeAppend(TCHAR* dest, int max_len, LPCTSTR src);
int  JsonEscape(LPCTSTR src, TCHAR* dest, int dest_max_len);

// commands.c
BOOL HandleCommand(LPCTSTR cmd);
void ShowHelp();
void ShowStatus();
void ShowSkills();
void ShowTools();
void OpenSettings();
void ShowPermission();
void ShowGuiInfo();

// settings.c
void RunSettingsPanel();

// shell.c
int  CheckDangerousCommand(LPCTSTR cmd);
BOOL CanExecuteCommand(LPCTSTR cmd, int* required_level);
LPCTSTR ExecuteShellCommand(LPCTSTR command);
LPCTSTR GetSystemInfoText();
LPCTSTR ReadFileContent(LPCTSTR path);
LPCTSTR ListDirectory(LPCTSTR path);
LPCTSTR WriteFileContent(LPCTSTR path, LPCTSTR content);

// ocr.c
BOOL InitOcr();
BOOL OcrIsAvailable();
LPCTSTR OcrImageFile(LPCTSTR imagePath);
LPCTSTR OcrScreenRegion(int x, int y, int width, int height);
LPCTSTR OcrFullScreen();

// installer.c
void RunInstaller();

// database.c
BOOL    InitDatabase();
BOOL    SaveDatabase();
LPCTSTR DBGetValue(int table, LPCTSTR key);
BOOL    DBSetValue(int table, LPCTSTR key, LPCTSTR value);
BOOL    DBDeleteKey(int table, LPCTSTR key);
BOOL    DBKeyExists(int table, LPCTSTR key);
int     DBGetRecordCount(int table);
LPCTSTR DBGetAllKeys(int table);
BOOL    DBClearTable(int table);
void    CloseDatabase();
LPCTSTR GetDatabaseInfoText();

// plugin.c
BOOL    InitPlugins();
LPCTSTR GetPluginListText();
LPCTSTR GetPluginInfoText(LPCTSTR id);
BOOL    EnablePlugin(LPCTSTR id);
BOOL    DisablePlugin(LPCTSTR id);
LPTSTR  ExecutePlugin(LPCTSTR id, LPCTSTR params);
BOOL    InstallPlugin(LPCTSTR source_path);
BOOL    UninstallPlugin(LPCTSTR id);
LPCTSTR GetPluginStatsText();
void    ShutdownPlugins();

#endif // HACKER_TERMINAL_H
