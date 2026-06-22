/*
 * 终端界面模块
 * 纯 Win32 Console API 实现
 */

#include "hacker.h"

HANDLE hConsole = NULL;
static CONSOLE_SCREEN_BUFFER_INFO csbi;
static WORD defaultColor = 0;

// 初始化终端
void InitTerminal() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    defaultColor = csbi.wAttributes;
    
    // 设置控制台标题
    SetConsoleTitle(APP_NAME);
    
    // 设置代码页为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

// 恢复终端
void RestoreTerminal() {
    if (hConsole) {
        SetConsoleTextAttribute(hConsole, defaultColor);
    }
}

// 设置颜色
void SetColor(WORD color) {
    if (hConsole) {
        SetConsoleTextAttribute(hConsole, color);
    }
}

// 重置颜色
void ResetColor() {
    if (hConsole) {
        SetConsoleTextAttribute(hConsole, defaultColor);
    }
}

// 清屏
void ClearScreen() {
    if (!hConsole) return;
    
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
    
    DWORD cellCount = csbiInfo.dwSize.X * csbiInfo.dwSize.Y;
    COORD homeCoords = { 0, 0 };
    DWORD count;
    
    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, defaultColor, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

// 打印横幅
void PrintBanner() {
    TCHAR levelName[64];
    _tcscpy(levelName, GetPermissionLevelName(g_config.permission_level));
    
    TCHAR modeName[32];
    switch (g_config.operation_mode) {
        case MODE_CLI: _tcscpy(modeName, _T("CLI")); break;
        case MODE_GUI: _tcscpy(modeName, _T("GUI")); break;
        case MODE_BOTH: _tcscpy(modeName, _T("混合")); break;
        default: _tcscpy(modeName, _T("未知")); break;
    }
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T("\n╔══════════════════════════════════════════════════════════════════════════╗\n"));
    _tprintf(_T("║ "));
    
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("%s %s"), APP_CODENAME, APP_VERSION);
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T(" - %s"), APP_AUTHOR);
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T("║\n"));
    _tprintf(_T("╠══════════════════════════════════════════════════════════════════════════╣\n"));
    _tprintf(_T("║  权限: Level %d - %-16s | 模式: %-8s"), 
           g_config.permission_level, levelName, modeName);
    _tprintf(_T("                             ║\n"));
    _tprintf(_T("╠══════════════════════════════════════════════════════════════════════════╣\n"));
    _tprintf(_T("║  "));
    
    SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
    _tprintf(_T("/help"));
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T(" 帮助 | "));
    
    SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
    _tprintf(_T("/skills"));
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T(" 技能 | "));
    
    SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
    _tprintf(_T("/settings"));
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T(" 设置 | "));
    
    SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
    _tprintf(_T("/exit"));
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T(" 退出"));
    _tprintf(_T("                          ║\n"));
    _tprintf(_T("╚══════════════════════════════════════════════════════════════════════════╝\n\n"));
    
    ResetColor();
}

// 打印用户消息
void PrintUser(LPCTSTR text) {
    SetColor(BG_BLUE | COLOR_WHITE | FOREGROUND_INTENSITY);
    _tprintf(_T(" 你 "));
    ResetColor();
    _tprintf(_T(" %s\n\n"), text);
}

// 打印助手消息
void PrintAssistant(LPCTSTR text) {
    SetColor(BG_GREEN | COLOR_WHITE | FOREGROUND_INTENSITY);
    _tprintf(_T(" AI "));
    ResetColor();
    
    // 多行处理
    TCHAR* buffer = StrDup(text);
    TCHAR* line = _tcstok(buffer, _T("\n"));
    if (line) {
        _tprintf(_T(" %s\n"), line);
        line = _tcstok(NULL, _T("\n"));
        while (line) {
            _tprintf(_T("    %s\n"), line);
            line = _tcstok(NULL, _T("\n"));
        }
    }
    free(buffer);
    _tprintf(_T("\n"));
}

// 打印工具调用
void PrintTool(LPCTSTR name) {
    SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
    _tprintf(_T(" 🔧 "));
    ResetColor();
    SetColor(COLOR_YELLOW);
    _tprintf(_T("调用工具: %s\n\n"), name);
    ResetColor();
}

// 打印GUI操作
void PrintGui(LPCTSTR action) {
    SetColor(COLOR_MAGENTA | FOREGROUND_INTENSITY);
    _tprintf(_T(" 🖱️  "));
    ResetColor();
    SetColor(COLOR_MAGENTA);
    _tprintf(_T("GUI操作: %s\n\n"), action);
    ResetColor();
}

// 打印结果
void PrintResult(LPCTSTR text) {
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T(" 📤 "));
    ResetColor();
    SetColor(COLOR_CYAN);
    _tprintf(_T("执行结果:\n"));
    ResetColor();
    
    // 截断过长输出
    TCHAR* buffer = StrDup(text);
    TCHAR* line = _tcstok(buffer, _T("\n"));
    int count = 0;
    while (line && count < 15) {
        SetColor(COLOR_DARKGRAY);
        // 限制每行长度
        TCHAR shortLine[128];
        _tcsncpy(shortLine, line, 120);
        shortLine[120] = 0;
        _tprintf(_T("    %s\n"), shortLine);
        ResetColor();
        line = _tcstok(NULL, _T("\n"));
        count++;
    }
    if (count >= 15) {
        SetColor(COLOR_DARKGRAY);
        _tprintf(_T("    ... (输出过长，已截断)\n"));
        ResetColor();
    }
    free(buffer);
    _tprintf(_T("\n"));
}

// 打印错误
void PrintError(LPCTSTR text) {
    SetColor(COLOR_RED | FOREGROUND_INTENSITY);
    _tprintf(_T(" ✗ "));
    ResetColor();
    SetColor(COLOR_RED);
    _tprintf(_T("%s\n\n"), text);
    ResetColor();
}

// 打印成功
void PrintSuccess(LPCTSTR text) {
    SetColor(COLOR_GREEN | FOREGROUND_INTENSITY);
    _tprintf(_T(" ✓ "));
    ResetColor();
    SetColor(COLOR_GREEN);
    _tprintf(_T("%s\n\n"), text);
    ResetColor();
}

// 打印信息
void PrintInfo(LPCTSTR text) {
    SetColor(COLOR_BLUE | FOREGROUND_INTENSITY);
    _tprintf(_T(" ℹ "));
    ResetColor();
    SetColor(COLOR_BLUE);
    _tprintf(_T("%s\n\n"), text);
    ResetColor();
}

// 打印Hacker风格
void PrintHacker(LPCTSTR text) {
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T(" ⚡ "));
    ResetColor();
    SetColor(COLOR_CYAN);
    _tprintf(_T("%s\n\n"), text);
    ResetColor();
}

// 打印警告
void PrintWarning(LPCTSTR text) {
    SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
    _tprintf(_T(" ⚠ "));
    ResetColor();
    SetColor(COLOR_YELLOW);
    _tprintf(_T("%s\n\n"), text);
    ResetColor();
}

// 打印权限信息
void PrintPermissionInfo() {
    int level = g_config.permission_level;
    LPCTSTR levelName = GetPermissionLevelName(level);
    
    SetColor(COLOR_RED | FOREGROUND_INTENSITY);
    _tprintf(_T("\n════════════════════════════════════════════════════════════════\n"));
    _tprintf(_T("  权限等级\n"));
    _tprintf(_T("════════════════════════════════════════════════════════════════\n\n"));
    ResetColor();
    
    _tprintf(_T("  当前等级: Level %d - %s\n\n"), level, levelName);
    
    // 等级条
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T("  ["));
    for (int i = 0; i <= 10; i++) {
        if (i == level) {
            SetColor(COLOR_GREEN | FOREGROUND_INTENSITY);
            _tprintf(_T("█"));
        } else if (i < level) {
            SetColor(COLOR_DARKGRAY);
            _tprintf(_T("▓"));
        } else {
            SetColor(COLOR_DARKGRAY);
            _tprintf(_T("░"));
        }
    }
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T("] %d/10\n\n"), level);
    ResetColor();
    
    // 各等级说明
    for (int i = 0; i <= 10; i++) {
        LPCTSTR name = GetPermissionLevelName(i);
        if (i == level) {
            SetColor(COLOR_GREEN | FOREGROUND_INTENSITY);
            _tprintf(_T("  ▶ Level %d: %s\n"), i, name);
        } else {
            SetColor(COLOR_DARKGRAY);
            _tprintf(_T("    Level %d: %s\n"), i, name);
        }
        ResetColor();
    }
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T("\n  在设置面板中可以调整权限等级: /settings\n\n"));
    ResetColor();
}

// 打印GUI工具列表
void PrintGuiTools() {
    SetColor(COLOR_MAGENTA | FOREGROUND_INTENSITY);
    _tprintf(_T("\n════════════════════════════════════════════════════════════════\n"));
    _tprintf(_T("  GUI 模拟操作工具\n"));
    _tprintf(_T("════════════════════════════════════════════════════════════════\n\n"));
    ResetColor();
    
    if (!GuiIsAvailable()) {
        PrintError(_T("GUI 控制不可用"));
        return;
    }
    
    struct {
        LPCTSTR action;
        LPCTSTR desc;
        LPCTSTR params;
    } actions[] = {
        {_T("click"),      _T("点击鼠标"),       _T("x, y, button, clicks")},
        {_T("move"),       _T("移动鼠标"),       _T("x, y, duration")},
        {_T("type"),       _T("输入文本"),       _T("text")},
        {_T("hotkey"),     _T("按下快捷键"),     _T("keys")},
        {_T("press"),      _T("按下按键"),       _T("key, presses")},
        {_T("scroll"),     _T("滚动鼠标"),       _T("amount")},
        {_T("position"),   _T("获取鼠标位置"),   _T("无")},
        {_T("screenshot"), _T("截图"),           _T("path")},
        {_T("size"),       _T("获取屏幕大小"),   _T("无")},
        {_T("drag"),       _T("拖拽"),           _T("start_x, start_y, end_x, end_y")},
        {NULL, NULL, NULL}
    };
    
    for (int i = 0; actions[i].action; i++) {
        SetColor(COLOR_MAGENTA);
        _tprintf(_T("  %-15s"), actions[i].action);
        ResetColor();
        _tprintf(_T(" %s\n"), actions[i].desc);
        SetColor(COLOR_DARKGRAY);
        _tprintf(_T("    参数: %s\n\n"), actions[i].params);
        ResetColor();
    }
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T("  使用 /gui-action <操作> 执行\n"));
    _tprintf(_T("  示例: /gui-action position\n\n"));
    ResetColor();
}

// 读取一行输入
int ReadLine(TCHAR* buffer, int max_len) {
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("你: "));
    ResetColor();
    
    if (_fgetts(buffer, max_len, stdin)) {
        Trim(buffer);
        return _tcslen(buffer);
    }
    return -1;
}
