/*
 * 插件管理器
 * 
 * 支持官方插件和用户自制插件
 * DLL 格式插件，统一接口
 */

#include "hacker.h"

// 插件类型
typedef enum {
    PLUGIN_OFFICIAL = 0,   // 官方插件
    PLUGIN_USER,           // 用户插件
    PLUGIN_TYPE_COUNT
} PluginType;

// 插件状态
typedef enum {
    PLUGIN_DISABLED = 0,   // 已禁用
    PLUGIN_ENABLED,        // 已启用
    PLUGIN_LOADED,         // 已加载
    PLUGIN_ERROR           // 错误
} PluginStatus;

// 插件信息
typedef struct {
    TCHAR name[64];           // 插件名称
    TCHAR id[64];             // 插件ID
    TCHAR version[32];        // 版本号
    TCHAR author[64];         // 作者
    TCHAR description[256];   // 描述
    TCHAR path[MAX_PATH];     // 插件路径
    PluginType type;          // 插件类型
    PluginStatus status;      // 插件状态
    HMODULE hModule;          // DLL 句柄
    
    // 函数指针
    LPTSTR (*GetPluginInfo)();
    BOOL   (*InitPlugin)();
    LPTSTR (*Execute)(LPTSTR params);
    BOOL   (*ShutdownPlugin)();
    
    struct PluginInfo* next;
} PluginInfo;

// 插件管理器
typedef struct {
    PluginInfo* head;
    int count;
    int official_count;
    int user_count;
} PluginManager;

static PluginManager g_plugins;
static TCHAR g_plugins_dir[MAX_PATH];
static BOOL g_plugins_inited = FALSE;

// 获取插件类型名称
static LPCTSTR GetPluginTypeName(PluginType type) {
    switch (type) {
        case PLUGIN_OFFICIAL: return _T("官方");
        case PLUGIN_USER:     return _T("用户");
        default:              return _T("未知");
    }
}

// 获取插件状态名称
static LPCTSTR GetPluginStatusName(PluginStatus status) {
    switch (status) {
        case PLUGIN_DISABLED: return _T("已禁用");
        case PLUGIN_ENABLED:  return _T("已启用");
        case PLUGIN_LOADED:   return _T("已加载");
        case PLUGIN_ERROR:    return _T("错误");
        default:              return _T("未知");
    }
}

// 获取插件目录
static void GetPluginDir(PluginType type, TCHAR* path, int max_len) {
    if (type == PLUGIN_OFFICIAL) {
        _stprintf(path, _T("%s\\official"), g_plugins_dir);
    } else {
        _stprintf(path, _T("%s\\user"), g_plugins_dir);
    }
}

// 从数据库加载插件状态
static void LoadPluginStates() {
    // 从 plugins 表读取状态
    LPCTSTR all_keys = DBGetAllKeys(0);  // TABLE_PLUGINS = 0
    if (!all_keys || _tcslen(all_keys) == 0) return;
    
    // 解析所有键
    TCHAR keys_copy[8192];
    _tcscpy(keys_copy, all_keys);
    
    TCHAR* token = _tcstok(keys_copy, _T(","));
    while (token) {
        // 查找插件
        PluginInfo* plugin = g_plugins.head;
        while (plugin) {
            if (_tcscmp(plugin->id, token) == 0) {
                LPCTSTR status_str = DBGetValue(0, token);
                if (status_str && _tcscmp(status_str, _T("enabled")) == 0) {
                    plugin->status = PLUGIN_ENABLED;
                } else {
                    plugin->status = PLUGIN_DISABLED;
                }
                break;
            }
            plugin = (PluginInfo*)plugin->next;
        }
        token = _tcstok(NULL, _T(","));
    }
}

// 保存插件状态到数据库
static void SavePluginState(PluginInfo* plugin) {
    TCHAR status_str[32];
    if (plugin->status == PLUGIN_ENABLED || plugin->status == PLUGIN_LOADED) {
        _tcscpy(status_str, _T("enabled"));
    } else {
        _tcscpy(status_str, _T("disabled"));
    }
    DBSetValue(0, plugin->id, status_str);  // TABLE_PLUGINS = 0
}

// 扫描插件目录
static int ScanPluginDir(PluginType type) {
    TCHAR dir_path[MAX_PATH];
    GetPluginDir(type, dir_path, MAX_PATH);
    EnsureDirExists(dir_path);
    
    int count = 0;
    
    // 查找所有子目录
    TCHAR search_path[MAX_PATH];
    _stprintf(search_path, _T("%s\\*"), dir_path);
    
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(search_path, &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }
    
    do {
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            _tcscmp(findData.cFileName, _T(".")) != 0 &&
            _tcscmp(findData.cFileName, _T("..")) != 0) {
            
            // 检查是否有 plugin.dll
            TCHAR plugin_dll[MAX_PATH];
            _stprintf(plugin_dll, _T("%s\\%s\\plugin.dll"), dir_path, findData.cFileName);
            
            if (GetFileAttributes(plugin_dll) != INVALID_FILE_ATTRIBUTES) {
                // 创建插件信息
                PluginInfo* plugin = (PluginInfo*)malloc(sizeof(PluginInfo));
                ZeroMemory(plugin, sizeof(PluginInfo));
                
                _tcscpy(plugin->name, findData.cFileName);
                _tcscpy(plugin->id, findData.cFileName);
                _tcscpy(plugin->path, plugin_dll);
                plugin->type = type;
                plugin->status = PLUGIN_DISABLED;
                plugin->hModule = NULL;
                plugin->next = NULL;
                
                // 默认版本和描述
                _tcscpy(plugin->version, _T("1.0.0"));
                _tcscpy(plugin->author, _T("未知"));
                _stprintf(plugin->description, _T("%s插件"), GetPluginTypeName(type));
                
                // 添加到列表
                if (g_plugins.head == NULL) {
                    g_plugins.head = plugin;
                } else {
                    PluginInfo* curr = g_plugins.head;
                    while (curr->next) curr = (PluginInfo*)curr->next;
                    curr->next = (struct PluginInfo*)plugin;
                }
                
                g_plugins.count++;
                if (type == PLUGIN_OFFICIAL) {
                    g_plugins.official_count++;
                } else {
                    g_plugins.user_count++;
                }
                
                count++;
            }
        }
    } while (FindNextFile(hFind, &findData));
    
    FindClose(hFind);
    return count;
}

// 初始化插件管理器
BOOL InitPlugins() {
    if (g_plugins_inited) return TRUE;
    
    ZeroMemory(&g_plugins, sizeof(g_plugins));
    
    // 获取插件目录
    GetAppDir(g_plugins_dir);
    _tcscat(g_plugins_dir, _T("\\plugins"));
    EnsureDirExists(g_plugins_dir);
    
    // 确保子目录存在
    TCHAR official_dir[MAX_PATH];
    TCHAR user_dir[MAX_PATH];
    GetPluginDir(PLUGIN_OFFICIAL, official_dir, MAX_PATH);
    GetPluginDir(PLUGIN_USER, user_dir, MAX_PATH);
    EnsureDirExists(official_dir);
    EnsureDirExists(user_dir);
    
    // 扫描官方插件
    ScanPluginDir(PLUGIN_OFFICIAL);
    
    // 扫描用户插件
    ScanPluginDir(PLUGIN_USER);
    
    // 从数据库加载状态
    LoadPluginStates();
    
    g_plugins_inited = TRUE;
    return TRUE;
}

// 查找插件
PluginInfo* FindPlugin(LPCTSTR id) {
    PluginInfo* plugin = g_plugins.head;
    while (plugin) {
        if (_tcscmp(plugin->id, id) == 0) {
            return plugin;
        }
        plugin = (PluginInfo*)plugin->next;
    }
    return NULL;
}

// 加载插件 DLL
BOOL LoadPlugin(LPCTSTR id) {
    PluginInfo* plugin = FindPlugin(id);
    if (!plugin) return FALSE;
    
    if (plugin->status == PLUGIN_LOADED) return TRUE;
    
    // 加载 DLL
    plugin->hModule = LoadLibrary(plugin->path);
    if (!plugin->hModule) {
        plugin->status = PLUGIN_ERROR;
        return FALSE;
    }
    
    // 获取函数地址
    plugin->GetPluginInfo = (LPTSTR (*)())GetProcAddress(plugin->hModule, "GetPluginInfo");
    plugin->InitPlugin = (BOOL (*)())GetProcAddress(plugin->hModule, "InitPlugin");
    plugin->Execute = (LPTSTR (*)(LPTSTR))GetProcAddress(plugin->hModule, "Execute");
    plugin->ShutdownPlugin = (BOOL (*)())GetProcAddress(plugin->hModule, "ShutdownPlugin");
    
    // 初始化插件
    if (plugin->InitPlugin) {
        if (!plugin->InitPlugin()) {
            FreeLibrary(plugin->hModule);
            plugin->hModule = NULL;
            plugin->status = PLUGIN_ERROR;
            return FALSE;
        }
    }
    
    // 读取插件信息
    if (plugin->GetPluginInfo) {
        LPTSTR info = plugin->GetPluginInfo();
        if (info) {
            // 简单解析：name|version|author|description
            TCHAR* info_copy = _tcsdup(info);
            TCHAR* token = _tcstok(info_copy, _T("|"));
            int field = 0;
            
            while (token && field < 4) {
                switch (field) {
                    case 0: _tcscpy(plugin->name, token); break;
                    case 1: _tcscpy(plugin->version, token); break;
                    case 2: _tcscpy(plugin->author, token); break;
                    case 3: _tcscpy(plugin->description, token); break;
                }
                field++;
                token = _tcstok(NULL, _T("|"));
            }
            free(info_copy);
        }
    }
    
    plugin->status = PLUGIN_LOADED;
    return TRUE;
}

// 卸载插件
BOOL UnloadPlugin(LPCTSTR id) {
    PluginInfo* plugin = FindPlugin(id);
    if (!plugin) return FALSE;
    
    if (plugin->status != PLUGIN_LOADED) return TRUE;
    
    // 关闭插件
    if (plugin->ShutdownPlugin) {
        plugin->ShutdownPlugin();
    }
    
    // 释放 DLL
    if (plugin->hModule) {
        FreeLibrary(plugin->hModule);
        plugin->hModule = NULL;
    }
    
    plugin->status = PLUGIN_ENABLED;
    return TRUE;
}

// 启用插件
BOOL EnablePlugin(LPCTSTR id) {
    PluginInfo* plugin = FindPlugin(id);
    if (!plugin) return FALSE;
    
    if (plugin->status == PLUGIN_DISABLED) {
        plugin->status = PLUGIN_ENABLED;
        SavePluginState(plugin);
    }
    
    return TRUE;
}

// 禁用插件
BOOL DisablePlugin(LPCTSTR id) {
    PluginInfo* plugin = FindPlugin(id);
    if (!plugin) return FALSE;
    
    // 先卸载
    if (plugin->status == PLUGIN_LOADED) {
        UnloadPlugin(id);
    }
    
    plugin->status = PLUGIN_DISABLED;
    SavePluginState(plugin);
    
    return TRUE;
}

// 执行插件
LPTSTR ExecutePlugin(LPCTSTR id, LPCTSTR params) {
    static TCHAR result[4096];
    
    PluginInfo* plugin = FindPlugin(id);
    if (!plugin) {
        _stprintf(result, _T("错误：找不到插件 '%s'"), id);
        return result;
    }
    
    if (plugin->status == PLUGIN_DISABLED) {
        _stprintf(result, _T("错误：插件 '%s' 已禁用"), id);
        return result;
    }
    
    // 如果未加载，先加载
    if (plugin->status == PLUGIN_ENABLED) {
        if (!LoadPlugin(id)) {
            _stprintf(result, _T("错误：插件 '%s' 加载失败"), id);
            return result;
        }
    }
    
    if (plugin->Execute) {
        return plugin->Execute(params);
    } else {
        _stprintf(result, _T("错误：插件 '%s' 没有 Execute 函数"), id);
        return result;
    }
}

// 获取插件列表文本
LPCTSTR GetPluginListText() {
    static TCHAR result[8192];
    result[0] = 0;
    
    _stprintf(result, _T("插件列表 (共 %d 个)\n\n"), g_plugins.count);
    
    // 官方插件
    _tcscat(result, _T("【官方插件】\n"));
    PluginInfo* plugin = g_plugins.head;
    BOOL has_official = FALSE;
    
    while (plugin) {
        if (plugin->type == PLUGIN_OFFICIAL) {
            has_official = TRUE;
            TCHAR line[512];
            _stprintf(line, _T("  %-20s v%-10s [%s] %s\n"),
                      plugin->name,
                      plugin->version,
                      GetPluginStatusName(plugin->status),
                      plugin->description);
            _tcscat(result, line);
        }
        plugin = (PluginInfo*)plugin->next;
    }
    
    if (!has_official) {
        _tcscat(result, _T("  (暂无官方插件)\n"));
    }
    
    // 用户插件
    _tcscat(result, _T("\n【用户插件】\n"));
    plugin = g_plugins.head;
    BOOL has_user = FALSE;
    
    while (plugin) {
        if (plugin->type == PLUGIN_USER) {
            has_user = TRUE;
            TCHAR line[512];
            _stprintf(line, _T("  %-20s v%-10s [%s] %s\n"),
                      plugin->name,
                      plugin->version,
                      GetPluginStatusName(plugin->status),
                      plugin->description);
            _tcscat(result, line);
        }
        plugin = (PluginInfo*)plugin->next;
    }
    
    if (!has_user) {
        _tcscat(result, _T("  (暂无用户插件)\n"));
    }
    
    _tcscat(result, _T("\n使用 /plugin <命令> 管理插件\n"));
    _tcscat(result, _T("  list    - 列出所有插件\n"));
    _tcscat(result, _T("  info <id> - 查看插件详情\n"));
    _tcscat(result, _T("  enable <id> - 启用插件\n"));
    _tcscat(result, _T("  disable <id> - 禁用插件\n"));
    _tcscat(result, _T("  run <id> [参数] - 执行插件\n"));
    _tcscat(result, _T("  install <路径> - 安装插件\n"));
    _tcscat(result, _T("  uninstall <id> - 卸载插件\n"));
    
    return result;
}

// 获取插件详情
LPCTSTR GetPluginInfoText(LPCTSTR id) {
    static TCHAR result[2048];
    
    PluginInfo* plugin = FindPlugin(id);
    if (!plugin) {
        _stprintf(result, _T("错误：找不到插件 '%s'"), id);
        return result;
    }
    
    _stprintf(result,
        _T("插件名称: %s\n")
        _T("插件 ID: %s\n")
        _T("版本: %s\n")
        _T("作者: %s\n")
        _T("类型: %s\n")
        _T("状态: %s\n\n")
        _T("描述: %s\n\n")
        _T("路径: %s\n"),
        plugin->name,
        plugin->id,
        plugin->version,
        plugin->author,
        GetPluginTypeName(plugin->type),
        GetPluginStatusName(plugin->status),
        plugin->description,
        plugin->path);
    
    return result;
}

// 安装插件
BOOL InstallPlugin(LPCTSTR source_path) {
    // 检查源文件
    if (GetFileAttributes(source_path) == INVALID_FILE_ATTRIBUTES) {
        return FALSE;
    }
    
    // 获取插件名（从路径提取）
    TCHAR plugin_name[64];
    TCHAR drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
    _tsplitpath(source_path, drive, dir, fname, ext);
    _tcscpy(plugin_name, fname);
    
    // 创建目标目录
    TCHAR target_dir[MAX_PATH];
    _stprintf(target_dir, _T("%s\\user\\%s"), g_plugins_dir, plugin_name);
    EnsureDirExists(target_dir);
    
    // 复制 plugin.dll
    TCHAR target_dll[MAX_PATH];
    _stprintf(target_dll, _T("%s\\plugin.dll"), target_dir);
    
    if (!CopyFile(source_path, target_dll, FALSE)) {
        return FALSE;
    }
    
    // 重新扫描插件
    // 简化处理：直接添加到列表
    PluginInfo* plugin = (PluginInfo*)malloc(sizeof(PluginInfo));
    ZeroMemory(plugin, sizeof(PluginInfo));
    
    _tcscpy(plugin->name, plugin_name);
    _tcscpy(plugin->id, plugin_name);
    _tcscpy(plugin->path, target_dll);
    plugin->type = PLUGIN_USER;
    plugin->status = PLUGIN_DISABLED;
    plugin->hModule = NULL;
    plugin->next = NULL;
    
    _tcscpy(plugin->version, _T("1.0.0"));
    _tcscpy(plugin->author, _T("用户"));
    _stprintf(plugin->description, _T("用户自制插件"));
    
    if (g_plugins.head == NULL) {
        g_plugins.head = plugin;
    } else {
        PluginInfo* curr = g_plugins.head;
        while (curr->next) curr = (PluginInfo*)curr->next;
        curr->next = (struct PluginInfo*)plugin;
    }
    
    g_plugins.count++;
    g_plugins.user_count++;
    
    SavePluginState(plugin);
    
    return TRUE;
}

// 卸载插件（删除文件）
BOOL UninstallPlugin(LPCTSTR id) {
    PluginInfo* plugin = FindPlugin(id);
    if (!plugin) return FALSE;
    
    // 先卸载 DLL
    if (plugin->status == PLUGIN_LOADED) {
        UnloadPlugin(id);
    }
    
    // 获取插件目录
    TCHAR plugin_dir[MAX_PATH];
    _tcscpy(plugin_dir, plugin->path);
    TCHAR* last_slash = _tcsrchr(plugin_dir, '\\');
    if (last_slash) *last_slash = 0;
    
    // 删除 plugin.dll
    DeleteFile(plugin->path);
    
    // 删除目录（如果为空）
    RemoveDirectory(plugin_dir);
    
    // 从列表移除
    PluginInfo* prev = NULL;
    PluginInfo* curr = g_plugins.head;
    
    while (curr) {
        if (curr == plugin) {
            if (prev) {
                prev->next = curr->next;
            } else {
                g_plugins.head = (PluginInfo*)curr->next;
            }
            
            if (plugin->type == PLUGIN_OFFICIAL) {
                g_plugins.official_count--;
            } else {
                g_plugins.user_count--;
            }
            g_plugins.count--;
            
            free(curr);
            break;
        }
        
        prev = curr;
        curr = (PluginInfo*)curr->next;
    }
    
    // 从数据库删除
    DBDeleteKey(0, id);  // TABLE_PLUGINS = 0
    
    return TRUE;
}

// 获取插件统计
LPCTSTR GetPluginStatsText() {
    static TCHAR result[512];
    
    _stprintf(result,
        _T("插件统计:\n")
        _T("  总计: %d 个\n")
        _T("  官方插件: %d 个\n")
        _T("  用户插件: %d 个\n"),
        g_plugins.count,
        g_plugins.official_count,
        g_plugins.user_count);
    
    return result;
}

// 关闭插件管理器
void ShutdownPlugins() {
    if (!g_plugins_inited) return;
    
    // 卸载所有已加载的插件
    PluginInfo* plugin = g_plugins.head;
    while (plugin) {
        if (plugin->status == PLUGIN_LOADED) {
            if (plugin->ShutdownPlugin) {
                plugin->ShutdownPlugin();
            }
            if (plugin->hModule) {
                FreeLibrary(plugin->hModule);
                plugin->hModule = NULL;
            }
        }
        plugin = (PluginInfo*)plugin->next;
    }
    
    // 释放内存
    plugin = g_plugins.head;
    while (plugin) {
        PluginInfo* next = (PluginInfo*)plugin->next;
        free(plugin);
        plugin = next;
    }
    
    g_plugins.head = NULL;
    g_plugins.count = 0;
    g_plugins.official_count = 0;
    g_plugins.user_count = 0;
    
    g_plugins_inited = FALSE;
}
