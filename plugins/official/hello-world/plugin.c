/*
 * Hacker Terminal 官方示例插件 - Hello World
 * 
 * 这是一个最简单的插件示例，演示插件开发的基本结构
 * 
 * 编译命令 (MinGW-w64):
 *   gcc -shared -o plugin.dll plugin.c -luser32
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

// DLL 入口
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

// 获取插件信息
// 返回格式: name|version|author|description
__declspec(dllexport) LPTSTR GetPluginInfo() {
    static TCHAR info[512];
    _stprintf(info, _T("Hello World|1.0.0|爱折腾实验室|官方示例插件 - 向世界问好"));
    return info;
}

// 初始化插件
__declspec(dllexport) BOOL InitPlugin() {
    // 在这里执行初始化操作
    return TRUE;
}

// 执行插件功能
__declspec(dllexport) LPTSTR Execute(LPTSTR params) {
    static TCHAR result[4096];
    
    if (params && _tcslen(params) > 0) {
        _stprintf(result, 
            _T("╔══════════════════════════════════════╗\n")
            _T("║     Hello World 插件                ║\n")
            _T("╠══════════════════════════════════════╣\n")
            _T("║  你好，%s！                       ║\n")
            _T("║                                      ║\n")
            _T("║  这是一个官方示例插件                 ║\n")
            _T("║  欢迎来到 Hacker Terminal 插件世界！ ║\n")
            _T("╚══════════════════════════════════════╝\n"),
            params);
    } else {
        _stprintf(result, 
            _T("╔══════════════════════════════════════╗\n")
            _T("║     Hello World 插件                ║\n")
            _T("╠══════════════════════════════════════╣\n")
            _T("║  你好，世界！                       ║\n")
            _T("║                                      ║\n")
            _T("║  这是一个官方示例插件                 ║\n")
            _T("║  欢迎来到 Hacker Terminal 插件世界！ ║\n")
            _T("╚══════════════════════════════════════╝\n"));
    }
    
    return result;
}

// 关闭插件
__declspec(dllexport) BOOL ShutdownPlugin() {
    // 在这里执行清理操作
    return TRUE;
}
