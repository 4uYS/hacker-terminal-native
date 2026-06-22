/*
 * Hacker Terminal 用户插件模板
 * 
 * 这是一个插件开发模板，你可以基于此开发自己的插件
 * 
 * 【插件开发步骤】
 * 1. 复制此目录，重命名为你的插件名
 * 2. 修改 plugin.c 中的插件信息和功能实现
 * 3. 编译为 plugin.dll
 * 4. 将 plugin.dll 放入 plugins/user/你的插件名/ 目录
 * 5. 在 Hacker Terminal 中使用 /plugin install <路径> 安装
 *    或直接手动放入目录
 * 
 * 【编译命令】
 * MinGW-w64:
 *   gcc -shared -o plugin.dll plugin.c -luser32
 * 
 * MSVC:
 *   cl /LD plugin.c user32.lib
 * 
 * 【插件接口规范】
 * 插件DLL必须导出以下4个函数：
 *   1. GetPluginInfo() - 获取插件信息
 *      返回格式: name|version|author|description
 *   
 *   2. InitPlugin() - 初始化插件
 *      返回: TRUE 成功，FALSE 失败
 *   
 *   3. Execute(params) - 执行插件功能
 *      参数: params - 传入的参数字符串
 *      返回: 执行结果字符串
 *   
 *   4. ShutdownPlugin() - 关闭插件
 *      返回: TRUE 成功，FALSE 失败
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

// ============================================================
// 插件配置 - 请修改这里
// ============================================================

#define PLUGIN_NAME     _T("我的插件")      // 插件名称
#define PLUGIN_VERSION  _T("1.0.0")        // 版本号
#define PLUGIN_AUTHOR   _T("你的名字")      // 作者
#define PLUGIN_DESC     _T("插件描述")      // 描述

// ============================================================
// DLL 入口（一般不需要修改）
// ============================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            // DLL 被加载时调用
            break;
        case DLL_THREAD_ATTACH:
            // 新线程创建时调用
            break;
        case DLL_THREAD_DETACH:
            // 线程退出时调用
            break;
        case DLL_PROCESS_DETACH:
            // DLL 被卸载时调用
            break;
    }
    return TRUE;
}

// ============================================================
// 插件接口实现
// ============================================================

// 获取插件信息
__declspec(dllexport) LPTSTR GetPluginInfo() {
    static TCHAR info[512];
    _stprintf(info, _T("%s|%s|%s|%s"), 
              PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR, PLUGIN_DESC);
    return info;
}

// 初始化插件
__declspec(dllexport) BOOL InitPlugin() {
    // TODO: 在这里添加你的初始化代码
    // 例如：加载配置、初始化资源、连接数据库等
    
    return TRUE;  // 返回 TRUE 表示初始化成功
}

// 执行插件功能
__declspec(dllexport) LPTSTR Execute(LPTSTR params) {
    static TCHAR result[4096];
    
    // TODO: 在这里实现你的插件功能
    // params 是用户传入的参数
    
    // 示例：返回一个简单的结果
    if (params && _tcslen(params) > 0) {
        _stprintf(result, _T("插件执行成功！\n参数: %s"), params);
    } else {
        _stprintf(result, _T("插件执行成功！\n(无参数)"));
    }
    
    return result;
}

// 关闭插件
__declspec(dllexport) BOOL ShutdownPlugin() {
    // TODO: 在这里添加你的清理代码
    // 例如：保存配置、释放资源、关闭连接等
    
    return TRUE;  // 返回 TRUE 表示关闭成功
}
