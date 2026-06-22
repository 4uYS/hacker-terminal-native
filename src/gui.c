/*
 * GUI 控制模块
 * 纯 Win32 API 实现键鼠模拟、截图
 */

#include "hacker.h"

static BOOL guiAvailable = FALSE;

// 初始化GUI控制
BOOL InitGuiControl() {
    // 检查是否有桌面交互权限
    HWINSTA hWinStation = GetProcessWindowStation();
    if (hWinStation == NULL) {
        guiAvailable = FALSE;
        return FALSE;
    }
    
    guiAvailable = TRUE;
    return TRUE;
}

// GUI是否可用
BOOL GuiIsAvailable() {
    return guiAvailable && (g_config.permission_level >= 3);
}

// 解析参数
static int ParseIntParam(LPCTSTR params, int index) {
    TCHAR* buf = StrDup(params);
    TCHAR* token = _tcstok(buf, _T(" ,"));
    
    int i = 0;
    while (token && i < index) {
        token = _tcstok(NULL, _T(" ,"));
        i++;
    }
    
    int result = 0;
    if (token) {
        result = _ttoi(token);
    }
    
    free(buf);
    return result;
}

// 解析字符串参数
static void ParseStrParam(LPCTSTR params, int index, TCHAR* buffer, int maxLen) {
    TCHAR* buf = StrDup(params);
    TCHAR* token = _tcstok(buf, _T(" ,"));
    
    int i = 0;
    while (token && i < index) {
        token = _tcstok(NULL, _T(" ,"));
        i++;
    }
    
    if (token) {
        _tcsncpy(buffer, token, maxLen - 1);
        buffer[maxLen - 1] = 0;
    } else {
        buffer[0] = 0;
    }
    
    free(buf);
}

// 执行GUI操作
LPCTSTR GuiExecute(LPCTSTR action, LPCTSTR params) {
    static TCHAR result[1024];
    result[0] = 0;
    
    if (!GuiIsAvailable()) {
        return _T("错误: GUI 控制不可用（权限不足或无桌面权限）");
    }
    
    TCHAR actionLower[64];
    _tcsncpy(actionLower, action, 63);
    actionLower[63] = 0;
    ToLowerCase(actionLower);
    
    // 点击鼠标
    if (StrEquals(actionLower, _T("click"))) {
        int x = ParseIntParam(params, 0);
        int y = ParseIntParam(params, 1);
        int button = ParseIntParam(params, 2);  // 0=左, 1=右, 2=中
        int clicks = ParseIntParam(params, 3);
        if (clicks <= 0) clicks = 1;
        
        SetCursorPos(x, y);
        Sleep(50);
        
        DWORD downFlag = MOUSEEVENTF_LEFTDOWN;
        DWORD upFlag = MOUSEEVENTF_LEFTUP;
        
        if (button == 1) {
            downFlag = MOUSEEVENTF_RIGHTDOWN;
            upFlag = MOUSEEVENTF_RIGHTUP;
        } else if (button == 2) {
            downFlag = MOUSEEVENTF_MIDDLEDOWN;
            upFlag = MOUSEEVENTF_MIDDLEUP;
        }
        
        for (int i = 0; i < clicks; i++) {
            mouse_event(downFlag, 0, 0, 0, 0);
            Sleep(20);
            mouse_event(upFlag, 0, 0, 0, 0);
            Sleep(50);
        }
        
        _stprintf(result, _T("点击完成: (%d, %d)"), x, y);
        return result;
    }
    
    // 移动鼠标
    if (StrEquals(actionLower, _T("move"))) {
        int x = ParseIntParam(params, 0);
        int y = ParseIntParam(params, 1);
        int duration = ParseIntParam(params, 2);
        if (duration <= 0) duration = 200;
        
        // 平滑移动
        int curX, curY;
        GuiGetPosition(&curX, &curY);
        
        int steps = duration / 20;
        if (steps < 1) steps = 1;
        
        for (int i = 1; i <= steps; i++) {
            int nx = curX + (x - curX) * i / steps;
            int ny = curY + (y - curY) * i / steps;
            SetCursorPos(nx, ny);
            Sleep(20);
        }
        
        _stprintf(result, _T("移动完成: (%d, %d)"), x, y);
        return result;
    }
    
    // 输入文本
    if (StrEquals(actionLower, _T("type"))) {
        TCHAR text[512];
        ParseStrParam(params, 0, text, 512);
        
        // 去掉引号
        if (text[0] == '"') {
            _tcscpy(text, text + 1);
            TCHAR* lastQuote = _tcsrchr(text, '"');
            if (lastQuote) *lastQuote = 0;
        }
        
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        
        for (int i = 0; text[i]; i++) {
            // 发送字符
            input.ki.wVk = 0;
            input.ki.wScan = text[i];
            input.ki.dwFlags = KEYEVENTF_UNICODE;
            SendInput(1, &input, sizeof(INPUT));
            
            Sleep(30);
            
            input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));
            
            Sleep(30);
        }
        
        _stprintf(result, _T("输入完成: %s"), text);
        return result;
    }
    
    // 快捷键
    if (StrEquals(actionLower, _T("hotkey"))) {
        TCHAR keys[256];
        _tcscpy(keys, params);
        
        // 解析按键组合
        BOOL ctrl = FALSE, shift = FALSE, alt = FALSE, win = FALSE;
        TCHAR mainKey[32] = {0};
        
        TCHAR* token = _tcstok(keys, _T(" +"));
        while (token) {
            TCHAR keyLower[32];
            _tcsncpy(keyLower, token, 31);
            keyLower[31] = 0;
            ToLowerCase(keyLower);
            
            if (StrEquals(keyLower, _T("ctrl")) || StrEquals(keyLower, _T("control"))) {
                ctrl = TRUE;
            } else if (StrEquals(keyLower, _T("shift"))) {
                shift = TRUE;
            } else if (StrEquals(keyLower, _T("alt"))) {
                alt = TRUE;
            } else if (StrEquals(keyLower, _T("win")) || StrEquals(keyLower, _T("windows"))) {
                win = TRUE;
            } else {
                _tcscpy(mainKey, keyLower);
            }
            
            token = _tcstok(NULL, _T(" +"));
        }
        
        // 转换虚拟键码
        WORD vk = 0;
        if (_tcslen(mainKey) == 1) {
            vk = toupper(mainKey[0]);
        } else if (StrEquals(mainKey, _T("enter")) || StrEquals(mainKey, _T("return"))) {
            vk = VK_RETURN;
        } else if (StrEquals(mainKey, _T("space"))) {
            vk = VK_SPACE;
        } else if (StrEquals(mainKey, _T("tab"))) {
            vk = VK_TAB;
        } else if (StrEquals(mainKey, _T("escape")) || StrEquals(mainKey, _T("esc"))) {
            vk = VK_ESCAPE;
        } else if (StrEquals(mainKey, _T("backspace"))) {
            vk = VK_BACK;
        } else if (StrEquals(mainKey, _T("delete")) || StrEquals(mainKey, _T("del"))) {
            vk = VK_DELETE;
        } else if (StrEquals(mainKey, _T("s"))) {
            vk = 'S';
        } else if (StrEquals(mainKey, _T("c"))) {
            vk = 'C';
        } else if (StrEquals(mainKey, _T("v"))) {
            vk = 'V';
        } else if (StrEquals(mainKey, _T("a"))) {
            vk = 'A';
        } else if (StrEquals(mainKey, _T("z"))) {
            vk = 'Z';
        } else if (StrEquals(mainKey, _T("x"))) {
            vk = 'X';
        } else if (StrEquals(mainKey, _T("y"))) {
            vk = 'Y';
        } else if (StrEquals(mainKey, _T("w"))) {
            vk = 'W';
        } else if (StrEquals(mainKey, _T("r"))) {
            vk = 'R';
        } else if (StrEquals(mainKey, _T("l"))) {
            vk = 'L';
        } else if (StrEquals(mainKey, _T("f4"))) {
            vk = VK_F4;
        } else if (StrEquals(mainKey, _T("f5"))) {
            vk = VK_F5;
        } else {
            vk = toupper(mainKey[0]);
        }
        
        // 按下修饰键
        if (ctrl) keybd_event(VK_CONTROL, 0, 0, 0);
        if (shift) keybd_event(VK_SHIFT, 0, 0, 0);
        if (alt) keybd_event(VK_MENU, 0, 0, 0);
        if (win) keybd_event(VK_LWIN, 0, 0, 0);
        
        Sleep(50);
        
        // 按下和释放主键
        if (vk) {
            keybd_event(vk, 0, 0, 0);
            Sleep(50);
            keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
        }
        
        Sleep(50);
        
        // 释放修饰键
        if (win) keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
        if (alt) keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
        if (shift) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        if (ctrl) keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
        
        _stprintf(result, _T("快捷键执行完成"));
        return result;
    }
    
    // 按下按键
    if (StrEquals(actionLower, _T("press"))) {
        TCHAR keyName[32];
        ParseStrParam(params, 0, keyName, 32);
        int presses = ParseIntParam(params, 1);
        if (presses <= 0) presses = 1;
        
        ToLowerCase(keyName);
        
        WORD vk = 0;
        if (_tcslen(keyName) == 1) {
            vk = toupper(keyName[0]);
        } else if (StrEquals(keyName, _T("enter")) || StrEquals(keyName, _T("return"))) {
            vk = VK_RETURN;
        } else if (StrEquals(keyName, _T("space"))) {
            vk = VK_SPACE;
        } else if (StrEquals(keyName, _T("tab"))) {
            vk = VK_TAB;
        } else if (StrEquals(keyName, _T("escape")) || StrEquals(keyName, _T("esc"))) {
            vk = VK_ESCAPE;
        } else if (StrEquals(keyName, _T("backspace"))) {
            vk = VK_BACK;
        } else if (StrEquals(keyName, _T("delete")) || StrEquals(keyName, _T("del"))) {
            vk = VK_DELETE;
        } else if (StrEquals(keyName, _T("up"))) {
            vk = VK_UP;
        } else if (StrEquals(keyName, _T("down"))) {
            vk = VK_DOWN;
        } else if (StrEquals(keyName, _T("left"))) {
            vk = VK_LEFT;
        } else if (StrEquals(keyName, _T("right"))) {
            vk = VK_RIGHT;
        } else {
            vk = toupper(keyName[0]);
        }
        
        for (int i = 0; i < presses; i++) {
            keybd_event(vk, 0, 0, 0);
            Sleep(30);
            keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
            Sleep(50);
        }
        
        _stprintf(result, _T("按键完成: %s (%d次)"), keyName, presses);
        return result;
    }
    
    // 滚动鼠标
    if (StrEquals(actionLower, _T("scroll"))) {
        int amount = ParseIntParam(params, 0);
        
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, amount * WHEEL_DELTA, 0);
        
        _stprintf(result, _T("滚动完成: %d"), amount);
        return result;
    }
    
    // 获取鼠标位置
    if (StrEquals(actionLower, _T("position"))) {
        int x, y;
        GuiGetPosition(&x, &y);
        _stprintf(result, _T("鼠标位置: (%d, %d)"), x, y);
        return result;
    }
    
    // 获取屏幕大小
    if (StrEquals(actionLower, _T("size"))) {
        int w, h;
        GuiGetSize(&w, &h);
        _stprintf(result, _T("屏幕大小: %d x %d"), w, h);
        return result;
    }
    
    // 截图
    if (StrEquals(actionLower, _T("screenshot"))) {
        TCHAR path[MAX_PATH];
        ParseStrParam(params, 0, path, MAX_PATH);
        
        if (path[0] == 0) {
            _stprintf(path, _T("screenshot_%d.bmp"), GetTickCount());
        }
        
        if (GuiScreenshot(path)) {
            _stprintf(result, _T("截图已保存: %s"), path);
        } else {
            _stprintf(result, _T("截图失败"));
        }
        return result;
    }
    
    // 拖拽
    if (StrEquals(actionLower, _T("drag"))) {
        int sx = ParseIntParam(params, 0);
        int sy = ParseIntParam(params, 1);
        int ex = ParseIntParam(params, 2);
        int ey = ParseIntParam(params, 3);
        int duration = ParseIntParam(params, 4);
        if (duration <= 0) duration = 500;
        
        // 移动到起点
        SetCursorPos(sx, sy);
        Sleep(100);
        
        // 按下左键
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        Sleep(100);
        
        // 平滑移动
        int steps = duration / 20;
        if (steps < 1) steps = 1;
        
        for (int i = 1; i <= steps; i++) {
            int nx = sx + (ex - sx) * i / steps;
            int ny = sy + (ey - sy) * i / steps;
            SetCursorPos(nx, ny);
            Sleep(20);
        }
        
        // 释放左键
        Sleep(100);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        
        _stprintf(result, _T("拖拽完成: (%d,%d) -> (%d,%d)"), sx, sy, ex, ey);
        return result;
    }
    
    _stprintf(result, _T("未知操作: %s"), action);
    return result;
}

// 获取鼠标位置
void GuiGetPosition(int* x, int* y) {
    POINT pt;
    GetCursorPos(&pt);
    if (x) *x = pt.x;
    if (y) *y = pt.y;
}

// 获取屏幕大小
void GuiGetSize(int* width, int* height) {
    if (width) *width = GetSystemMetrics(SM_CXSCREEN);
    if (height) *height = GetSystemMetrics(SM_CYSCREEN);
}

// 截图（保存为BMP）
BOOL GuiScreenshot(LPCTSTR path) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
    
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
    
    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);
    
    // 保存为BMP
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;
    
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    
    DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
    
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);
    
    GetDIBits(hScreenDC, hBitmap, 0, (UINT)bmp.bmHeight, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    
    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42; // "BM"
    
    DWORD dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    CloseHandle(hFile);
    
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    
    return TRUE;
}
