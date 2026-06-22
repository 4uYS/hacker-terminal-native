/*
 * 设置面板模块
 * 纯终端交互式UI
 */

#include "hacker.h"

#define MAX_MENU_ITEMS 20

typedef struct {
    TCHAR label[128];
    int type;  // 0=标题, 1=开关, 2=数值, 3=选择, 4=按钮, 5=返回
    void* value;
    int min_val;
    int max_val;
    int step;
    int options_count;
    LPCTSTR options[10];
} MenuItem;

static int currentIndex = 0;
static int itemCount = 0;
static MenuItem items[MAX_MENU_ITEMS];

// 添加菜单项
static void AddMenuItem(LPCTSTR label, int type, void* value, int min_val, int max_val, int step) {
    if (itemCount >= MAX_MENU_ITEMS) return;
    
    _tcscpy(items[itemCount].label, label);
    items[itemCount].type = type;
    items[itemCount].value = value;
    items[itemCount].min_val = min_val;
    items[itemCount].max_val = max_val;
    items[itemCount].step = step;
    items[itemCount].options_count = 0;
    itemCount++;
}

// 绘制设置面板
static void DrawSettingsPanel() {
    ClearScreen();
    
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n"));
    _tprintf(_T("╔══════════════════════════════════════════════════════════════╗\n"));
    _tprintf(_T("║                    Hacker Terminal 设置                      ║\n"));
    _tprintf(_T("╠══════════════════════════════════════════════════════════════╣\n"));
    _tprintf(_T("║  ↑↓ 切换选项 | 空格 切换开关 | ←→ 调整数值 | Enter 确认    ║\n"));
    _tprintf(_T("╚══════════════════════════════════════════════════════════════╝\n\n"));
    ResetColor();
    
    for (int i = 0; i < itemCount; i++) {
        MenuItem* item = &items[i];
        BOOL isCurrent = (i == currentIndex);
        
        // 标题
        if (item->type == 0) {
            SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
            _tprintf(_T("  %s\n\n"), item->label);
            ResetColor();
            continue;
        }
        
        // 返回按钮
        if (item->type == 5) {
            if (isCurrent) {
                SetColor(BG_BLUE | COLOR_WHITE | FOREGROUND_INTENSITY);
                _tprintf(_T("  %s "), item->label);
                ResetColor();
                _tprintf(_T("\n\n"));
            } else {
                SetColor(COLOR_DARKGRAY);
                _tprintf(_T("  %s\n\n"), item->label);
                ResetColor();
            }
            continue;
        }
        
        // 显示标签
        if (isCurrent) {
            SetColor(COLOR_BLUE | FOREGROUND_INTENSITY);
            _tprintf(_T("▶ "));
            SetColor(BG_BLUE | COLOR_WHITE | FOREGROUND_INTENSITY);
            _tprintf(_T("%-30s"), item->label);
            ResetColor();
        } else {
            SetColor(COLOR_DARKGRAY);
            _tprintf(_T("  "));
            ResetColor();
            _tprintf(_T("%-30s"), item->label);
        }
        
        _tprintf(_T(" "));
        
        // 显示值
        switch (item->type) {
            case 1: // 开关
                if (*(BOOL*)item->value) {
                    if (isCurrent) {
                        SetColor(BG_GREEN | COLOR_WHITE | FOREGROUND_INTENSITY);
                    } else {
                        SetColor(COLOR_GREEN | FOREGROUND_INTENSITY);
                    }
                    _tprintf(_T("[ 开启 ]"));
                } else {
                    if (isCurrent) {
                        SetColor(BG_RED | COLOR_WHITE);
                    } else {
                        SetColor(COLOR_RED);
                    }
                    _tprintf(_T("[ 关闭 ]"));
                }
                break;
                
            case 2: // 数值
                if (isCurrent) {
                    SetColor(BG_CYAN | COLOR_WHITE | FOREGROUND_INTENSITY);
                } else {
                    SetColor(COLOR_CYAN);
                }
                _tprintf(_T(" %d "), *(int*)item->value);
                break;
                
            case 3: // 选择
                if (isCurrent) {
                    SetColor(BG_MAGENTA | COLOR_WHITE | FOREGROUND_INTENSITY);
                } else {
                    SetColor(COLOR_MAGENTA);
                }
                if (*(int*)item->value >= 0 && *(int*)item->value < item->options_count) {
                    _tprintf(_T(" %s "), item->options[*(int*)item->value]);
                }
                break;
        }
        
        ResetColor();
        _tprintf(_T("\n"));
    }
    
    SetColor(COLOR_DARKGRAY);
    _tprintf(_T("\n  按 ESC 或选择「返回」退出设置\n\n"));
    ResetColor();
}

// 处理按键
static int HandleKey() {
    INPUT_RECORD ir;
    DWORD events;
    
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
    
    WaitForSingleObject(hStdin, INFINITE);
    ReadConsoleInput(hStdin, &ir, 1, &events);
    
    SetConsoleMode(hStdin, mode);
    
    if (ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown) {
        return 0;
    }
    
    WORD key = ir.Event.KeyEvent.wVirtualKeyCode;
    
    switch (key) {
        case VK_UP:
            do {
                currentIndex--;
                if (currentIndex < 0) currentIndex = itemCount - 1;
            } while (items[currentIndex].type == 0);
            return 1;
            
        case VK_DOWN:
            do {
                currentIndex++;
                if (currentIndex >= itemCount) currentIndex = 0;
            } while (items[currentIndex].type == 0);
            return 1;
            
        case VK_SPACE:
        case VK_RETURN: {
            MenuItem* item = &items[currentIndex];
            
            if (item->type == 5) {
                // 返回
                return -1;
            }
            
            if (item->type == 1) {
                // 开关切换
                *(BOOL*)item->value = !*(BOOL*)item->value;
                SaveConfig();
                return 1;
            }
            
            if (item->type == 3) {
                // 选择切换
                *(int*)item->value = (*(int*)item->value + 1) % item->options_count;
                SaveConfig();
                return 1;
            }
            
            return 1;
        }
            
        case VK_LEFT: {
            MenuItem* item = &items[currentIndex];
            if (item->type == 2) {
                *(int*)item->value -= item->step;
                if (*(int*)item->value < item->min_val) *(int*)item->value = item->min_val;
                SaveConfig();
            }
            if (item->type == 3) {
                *(int*)item->value = (*(int*)item->value - 1 + item->options_count) % item->options_count;
                SaveConfig();
            }
            return 1;
        }
            
        case VK_RIGHT: {
            MenuItem* item = &items[currentIndex];
            if (item->type == 2) {
                *(int*)item->value += item->step;
                if (*(int*)item->value > item->max_val) *(int*)item->value = item->max_val;
                SaveConfig();
            }
            if (item->type == 3) {
                *(int*)item->value = (*(int*)item->value + 1) % item->options_count;
                SaveConfig();
            }
            return 1;
        }
            
        case VK_ESCAPE:
            return -1;
    }
    
    return 0;
}

// 运行设置面板
void RunSettingsPanel() {
    itemCount = 0;
    currentIndex = 1;
    
    // 构建设置菜单
    AddMenuItem(_T("═══ 基础设置 ═══"), 0, NULL, 0, 0, 0);
    
    AddMenuItem(_T("权限等级"), 2, &g_config.permission_level, 0, 10, 1);
    
    int mode = g_config.operation_mode;
    MenuItem* modeItem = &items[itemCount];
    AddMenuItem(_T("操作模式"), 3, &g_config.operation_mode, 0, 2, 1);
    modeItem->options_count = 3;
    modeItem->options[0] = _T("仅命令行");
    modeItem->options[1] = _T("仅模拟点击");
    modeItem->options[2] = _T("混合模式");
    
    AddMenuItem(_T("═══ 隐私设置 ═══"), 0, NULL, 0, 0, 0);
    
    AddMenuItem(_T("分享系统信息"), 1, &g_config.share_os_info, 0, 0, 0);
    AddMenuItem(_T("分享用户名"), 1, &g_config.share_username, 0, 0, 0);
    AddMenuItem(_T("分享时区信息"), 1, &g_config.share_timezone, 0, 0, 0);
    AddMenuItem(_T("分享技能列表"), 1, &g_config.share_skills_list, 0, 0, 0);
    
    AddMenuItem(_T("═══ 安全设置 ═══"), 0, NULL, 0, 0, 0);
    
    AddMenuItem(_T("危险操作确认"), 1, &g_config.confirm_dangerous, 0, 0, 0);
    
    AddMenuItem(_T("═══ UI 设置 ═══"), 0, NULL, 0, 0, 0);
    
    AddMenuItem(_T("显示欢迎信息"), 1, &g_config.show_welcome, 0, 0, 0);
    
    AddMenuItem(_T("═══ 返回 ═══"), 0, NULL, 0, 0, 0);
    AddMenuItem(_T("返回主界面"), 5, NULL, 0, 0, 0);
    
    // 主循环
    while (TRUE) {
        DrawSettingsPanel();
        int result = HandleKey();
        if (result < 0) break;
    }
    
    SaveConfig();
    ClearScreen();
    PrintSuccess(_T("设置已保存"));
}
