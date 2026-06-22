/*
 * Hacker Terminal v1.0 - 主程序入口
 * 纯原生 Win32 API 实现
 * 爱折腾实验室 · 豆包 AI Agent Lab
 */

#include "hacker.h"
#include <shellapi.h>
#include <shlobj.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "wininet.lib")

// 主函数
int _tmain(int argc, _TCHAR* argv[]) {
    // 初始化各模块
    InitTerminal();
    InitConfig();
    InitContext();
    InitSkills();
    InitGuiControl();
    InitNetwork();
    InitAI();
    InitDatabase();
    InitPlugins();
    
    // 显示欢迎横幅
    if (g_config.show_welcome) {
        ClearScreen();
        PrintBanner();
        
        // 首次运行提示
        if (!g_config.installed) {
            PrintInfo(_T("欢迎使用 Hacker Terminal！这是首次运行。"));
            PrintInfo(_T("建议先打开设置面板配置云端AI API: /settings"));
            g_config.installed = TRUE;
            SaveConfig();
        }
    }
    
    // 主循环
    TCHAR input[2048];
    
    while (TRUE) {
        // 读取输入
        int len = ReadLine(input, 2048);
        if (len < 0) break;
        
        if (len == 0) continue;
        
        _tprintf(_T("\n"));
        
        // 处理命令
        if (HandleCommand(input)) {
            continue;
        }
        
        // AI 对话
        PrintUser(input);
        
        LPCTSTR response = GenerateResponse(input);
        PrintAssistant(response);
    }
    
    // 清理
    ShutdownPlugins();
    CloseDatabase();
    FreeSkills();
    FreeContext();
    RestoreTerminal();
    
    return 0;
}
