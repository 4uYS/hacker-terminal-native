/*
 * 安装向导程序
 * Hacker Terminal 交互式安装流程
 */

#include "hacker.h"

// 安装步骤
#define STEP_WELCOME      0
#define STEP_PERMISSION   1
#define STEP_MODE         2
#define STEP_AI_CONFIG    3
#define STEP_COMPLETE     4

static int current_step = STEP_WELCOME;
static int selected_permission = 5;
static int selected_mode = MODE_CLI;
static int selected_ai_model = 0;

// AI 模型选项
typedef struct {
    LPCTSTR name;
    LPCTSTR description;
    LPCTSTR memory_req;
    int recommended;
} AIModelOption;

static AIModelOption ai_models[] = {
    { _T("规则模式"), _T("无需模型，基于规则的简单回复"), _T("0 MB"), 1 },
    { _T("云端 API"), _T("调用云端大模型 API（需配置）"), _T("0 MB"), 1 },
    { _T("本地小模型 0.5B"), _T("超轻量本地模型，速度快"), _T("~300 MB"), 0 },
    { _T("本地模型 1.5B"), _T("平衡性能和速度"), _T("~1 GB"), 0 },
    { _T("本地模型 3B"), _T("较好的推理能力"), _T("~2 GB"), 0 },
    { _T("本地模型 7B"), _T("强大的推理能力"), _T("~4 GB"), 0 },
    { NULL, NULL, NULL, 0 }
};

// 打印欢迎界面
static void PrintWelcome() {
    ClearScreen();
    
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n\n"));
    _tprintf(_T("  ╔══════════════════════════════════════════════════════════════╗\n"));
    _tprintf(_T("  ║                                                              ║\n"));
    _tprintf(_T("  ║        Hacker Terminal 安装向导                              ║\n"));
    _tprintf(_T("  ║                                                              ║\n"));
    _tprintf(_T("  ╚══════════════════════════════════════════════════════════════╝\n"));
    ResetColor();
    
    _tprintf(_T("\n\n"));
    _tprintf(_T("  版本: %s %s\n"), APP_NAME, APP_VERSION);
    _tprintf(_T("  研发: %s\n"), APP_AUTHOR);
    _tprintf(_T("\n\n"));
    
    SetColor(COLOR_YELLOW);
    _tprintf(_T("  欢迎使用 Hacker Terminal！\n\n"));
    ResetColor();
    
    _tprintf(_T("  这是一个专为终端折腾打造的 AI 助手。\n"));
    _tprintf(_T("  安装向导将引导您完成初始配置。\n\n"));
    
    _tprintf(_T("  安装完成后，您可以随时在设置中修改所有选项。\n\n"));
    
    SetColor(COLOR_GREEN | FOREGROUND_INTENSITY);
    _tprintf(_T("  [按 Enter 继续安装]"));
    ResetColor();
    _tprintf(_T("\n\n"));
}

// 打印权限等级设置
static void PrintPermissionStep() {
    ClearScreen();
    
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n\n  步骤 2/5 - 权限等级设置\n\n"));
    ResetColor();
    
    _tprintf(_T("  选择您希望授予 AI 的操作权限等级：\n\n"));
    
    // 显示等级列表
    for (int i = 0; i <= 10; i++) {
        if (i == selected_permission) {
            SetColor(BG_BLUE | COLOR_WHITE | FOREGROUND_INTENSITY);
            _tprintf(_T("  > Level %2d - %-15s "), i, GetPermissionLevelName(i));
            ResetColor();
            _tprintf(_T("  ◄ 当前选择\n"));
        } else {
            SetColor(COLOR_DARKGRAY);
            _tprintf(_T("    Level %2d - %-15s\n"), i, GetPermissionLevelName(i));
            ResetColor();
        }
    }
    
    _tprintf(_T("\n\n"));
    
    // 显示当前等级说明
    SetColor(COLOR_YELLOW);
    _tprintf(_T("  当前等级: Level %d - %s\n\n"), selected_permission, GetPermissionLevelName(selected_permission));
    ResetColor();
    
    switch (selected_permission) {
        case 0:
            _tprintf(_T("  只能读取文件和查看系统信息，无法修改任何内容。\n"));
            _tprintf(_T("  最安全的模式，适合仅用于查询和学习。\n"));
            break;
        case 3:
            _tprintf(_T("  可以管理文件和文件夹，包括复制、移动、删除等。\n"));
            _tprintf(_T("  适合日常文件处理任务。\n"));
            break;
        case 5:
            _tprintf(_T("  可以执行大部分安全的系统命令和开发工具。\n"));
            _tprintf(_T("  推荐大多数用户使用此等级。\n"));
            break;
        case 7:
            SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
            _tprintf(_T("  ⚠ 警告：此等级可以执行系统管理操作。\n"));
            _tprintf(_T("    请确保您了解可能的风险。\n"));
            ResetColor();
            break;
        case 10:
            SetColor(COLOR_RED | FOREGROUND_INTENSITY);
            _tprintf(_T("  ⚠ 严重警告：完全控制模式！\n"));
            _tprintf(_T("    AI 可以执行任意操作，包括删除系统文件。\n"));
            _tprintf(_T("    请谨慎使用，仅在完全信任时选择。\n"));
            ResetColor();
            break;
        default:
            _tprintf(_T("  标准操作权限，可以执行大部分日常任务。\n"));
            break;
    }
    
    _tprintf(_T("\n\n"));
    SetColor(COLOR_GREEN);
    _tprintf(_T("  ← → 调整等级    Enter 确认选择"));
    ResetColor();
    _tprintf(_T("\n\n"));
}

// 打印操作模式选择
static void PrintModeStep() {
    ClearScreen();
    
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n\n  步骤 3/5 - 操作模式选择\n\n"));
    ResetColor();
    
    _tprintf(_T("  选择 AI 的操作方式：\n\n"));
    
    LPCTSTR mode_names[] = { _T("仅命令行 (CLI)"), _T("仅模拟点击 (GUI)"), _T("混合模式 (Both)") };
    LPCTSTR mode_descs[] = {
        _T("AI 仅通过命令行和文件操作与系统交互"),
        _T("AI 仅通过模拟鼠标键盘操作与系统交互"),
        _T("AI 可以同时使用命令行和模拟点击，自主选择最佳方式")
    };
    
    for (int i = 0; i < 3; i++) {
        if (i == selected_mode) {
            SetColor(BG_BLUE | COLOR_WHITE | FOREGROUND_INTENSITY);
            _tprintf(_T("  > %-25s "), mode_names[i]);
            ResetColor();
            _tprintf(_T("  ◄ 当前选择\n"));
        } else {
            SetColor(COLOR_DARKGRAY);
            _tprintf(_T("    %-25s\n"), mode_names[i]);
            ResetColor();
        }
    }
    
    _tprintf(_T("\n\n"));
    
    SetColor(COLOR_YELLOW);
    _tprintf(_T("  当前选择: %s\n\n"), mode_names[selected_mode]);
    ResetColor();
    
    _tprintf(_T("  %s\n\n"), mode_descs[selected_mode]);
    
    if (selected_mode == MODE_GUI || selected_mode == MODE_BOTH) {
        SetColor(COLOR_YELLOW);
        _tprintf(_T("  提示：GUI 模拟模式下，AI 运行时请勿操作鼠标键盘。\n"));
        _tprintf(_T("        可以使用 Ctrl+Shift+E 紧急终止。\n"));
        ResetColor();
    }
    
    _tprintf(_T("\n\n"));
    SetColor(COLOR_GREEN);
    _tprintf(_T("  ↑ ↓ 切换选项    Enter 确认选择"));
    ResetColor();
    _tprintf(_T("\n\n"));
}

// 打印 AI 模型配置
static void PrintAIStep() {
    ClearScreen();
    
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n\n  步骤 4/5 - AI 模型配置\n\n"));
    ResetColor();
    
    _tprintf(_T("  选择您要使用的 AI 模型：\n\n"));
    
    // 检测系统内存
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&ms);
    DWORDLONG total_mem_mb = ms.ullTotalPhys / 1024 / 1024;
    
    _tprintf(_T("  系统内存: %llu MB\n\n"), total_mem_mb);
    
    // 显示模型列表
    int model_count = 0;
    while (ai_models[model_count].name) {
        if (model_count == selected_ai_model) {
            SetColor(BG_BLUE | COLOR_WHITE | FOREGROUND_INTENSITY);
            _tprintf(_T("  > %-20s "), ai_models[model_count].name);
            ResetColor();
            if (ai_models[model_count].recommended) {
                SetColor(COLOR_GREEN);
                _tprintf(_T("[推荐] "));
                ResetColor();
            }
            _tprintf(_T("◄ 当前选择\n"));
        } else {
            SetColor(COLOR_DARKGRAY);
            _tprintf(_T("    %-20s "), ai_models[model_count].name);
            if (ai_models[model_count].recommended) {
                SetColor(COLOR_GREEN);
                _tprintf(_T("[推荐] "));
                ResetColor();
            }
            _tprintf(_T("\n"));
            ResetColor();
        }
        model_count++;
    }
    
    _tprintf(_T("\n\n"));
    
    SetColor(COLOR_YELLOW);
    _tprintf(_T("  当前选择: %s\n\n"), ai_models[selected_ai_model].name);
    ResetColor();
    
    _tprintf(_T("  %s\n"), ai_models[selected_ai_model].description);
    _tprintf(_T("  内存需求: %s\n\n"), ai_models[selected_ai_model].memory_req);
    
    if (selected_ai_model >= 2) {
        SetColor(COLOR_YELLOW);
        _tprintf(_T("  提示：本地模型需要额外下载 GGUF 模型文件。\n"));
        _tprintf(_T("        可以稍后在设置中配置模型路径。\n"));
        ResetColor();
    }
    
    if (selected_ai_model == 1) {
        SetColor(COLOR_YELLOW);
        _tprintf(_T("  提示：云端 API 需要在设置中配置 API 地址和密钥。\n"));
        ResetColor();
    }
    
    _tprintf(_T("\n\n"));
    SetColor(COLOR_GREEN);
    _tprintf(_T("  ↑ ↓ 切换模型    Enter 确认选择"));
    ResetColor();
    _tprintf(_T("\n\n"));
}

// 打印安装完成
static void PrintComplete() {
    ClearScreen();
    
    SetColor(COLOR_GREEN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n\n"));
    _tprintf(_T("  ╔══════════════════════════════════════════════════════════════╗\n"));
    _tprintf(_T("  ║                                                              ║\n"));
    _tprintf(_T("  ║        ✓ 安装完成！                                          ║\n"));
    _tprintf(_T("  ║                                                              ║\n"));
    _tprintf(_T("  ╚══════════════════════════════════════════════════════════════╝\n"));
    ResetColor();
    
    _tprintf(_T("\n\n"));
    SetColor(COLOR_YELLOW);
    _tprintf(_T("  配置摘要:\n\n"));
    ResetColor();
    
    _tprintf(_T("  权限等级: Level %d - %s\n"), selected_permission, GetPermissionLevelName(selected_permission));
    
    LPCTSTR mode_names[] = { _T("仅命令行"), _T("仅模拟点击"), _T("混合模式") };
    _tprintf(_T("  操作模式: %s\n"), mode_names[selected_mode]);
    
    _tprintf(_T("  AI 模型: %s\n"), ai_models[selected_ai_model].name);
    
    _tprintf(_T("\n\n"));
    _tprintf(_T("  配置文件已保存到: %s\n"), g_config_file);
    _tprintf(_T("  技能目录: %s\n"), g_skills_dir);
    
    _tprintf(_T("\n\n"));
    SetColor(COLOR_CYAN);
    _tprintf(_T("  感谢使用 Hacker Terminal！\n"));
    _tprintf(_T("  输入 /help 查看可用命令，开始您的折腾之旅吧！\n"));
    ResetColor();
    
    _tprintf(_T("\n\n"));
    SetColor(COLOR_GREEN | FOREGROUND_INTENSITY);
    _tprintf(_T("  [按 Enter 启动 Hacker Terminal]"));
    ResetColor();
    _tprintf(_T("\n\n"));
}

// 保存配置
static void SaveInstallConfig() {
    g_config.permission_level = selected_permission;
    g_config.operation_mode = selected_mode;
    g_config.installed = TRUE;
    
    // 设置 AI 后端
    switch (selected_ai_model) {
        case 0: // 规则模式
            _tcscpy(g_config.ai_backend, _T("rule"));
            break;
        case 1: // 云端 API
            _tcscpy(g_config.ai_backend, _T("cloud"));
            break;
        default: // 本地模型
            _tcscpy(g_config.ai_backend, _T("local"));
            break;
    }
    
    SaveConfig();
}

// 运行安装向导
void RunInstaller() {
    InitTerminal();
    InitConfig();
    
    current_step = STEP_WELCOME;
    
    while (1) {
        switch (current_step) {
            case STEP_WELCOME:
                PrintWelcome();
                break;
            case STEP_PERMISSION:
                PrintPermissionStep();
                break;
            case STEP_MODE:
                PrintModeStep();
                break;
            case STEP_AI_CONFIG:
                PrintAIStep();
                break;
            case STEP_COMPLETE:
                PrintComplete();
                break;
        }
        
        // 读取输入
        TCHAR input[256];
        ReadLine(input, 256);
        
        // 处理输入
        if (current_step == STEP_WELCOME) {
            // 欢迎界面，按 Enter 继续
            if (_tcslen(input) == 0) {
                current_step = STEP_PERMISSION;
            }
        }
        else if (current_step == STEP_PERMISSION) {
            if (_tcslen(input) == 0) {
                // Enter 确认
                current_step = STEP_MODE;
            }
            else if (_tcscmp(input, _T("left")) == 0 || _tcscmp(input, _T("←")) == 0) {
                if (selected_permission > 0) selected_permission--;
            }
            else if (_tcscmp(input, _T("right")) == 0 || _tcscmp(input, _T("→")) == 0) {
                if (selected_permission < 10) selected_permission++;
            }
        }
        else if (current_step == STEP_MODE) {
            if (_tcslen(input) == 0) {
                current_step = STEP_AI_CONFIG;
            }
            else if (_tcscmp(input, _T("up")) == 0 || _tcscmp(input, _T("↑")) == 0) {
                if (selected_mode > 0) selected_mode--;
            }
            else if (_tcscmp(input, _T("down")) == 0 || _tcscmp(input, _T("↓")) == 0) {
                if (selected_mode < 2) selected_mode++;
            }
        }
        else if (current_step == STEP_AI_CONFIG) {
            if (_tcslen(input) == 0) {
                SaveInstallConfig();
                current_step = STEP_COMPLETE;
            }
            else if (_tcscmp(input, _T("up")) == 0 || _tcscmp(input, _T("↑")) == 0) {
                if (selected_ai_model > 0) selected_ai_model--;
            }
            else if (_tcscmp(input, _T("down")) == 0 || _tcscmp(input, _T("↓")) == 0) {
                if (ai_models[selected_ai_model + 1].name) selected_ai_model++;
            }
        }
        else if (current_step == STEP_COMPLETE) {
            if (_tcslen(input) == 0) {
                // 安装完成，退出
                break;
            }
        }
    }
    
    RestoreTerminal();
}
