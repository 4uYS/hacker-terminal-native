/*
 * 命令系统模块
 */

#include "hacker.h"

// 处理命令
BOOL HandleCommand(LPCTSTR cmd) {
    if (!cmd || cmd[0] != '/') {
        return FALSE;
    }
    
    TCHAR cmdLower[256];
    _tcsncpy(cmdLower, cmd, 255);
    cmdLower[255] = 0;
    ToLowerCase(cmdLower);
    
    // /help
    if (StartsWith(cmdLower, _T("/help")) || StartsWith(cmdLower, _T("/?"))) {
        ShowHelp();
        return TRUE;
    }
    
    // /skills
    if (StartsWith(cmdLower, _T("/skills")) || StartsWith(cmdLower, _T("/skill"))) {
        ShowSkills();
        return TRUE;
    }
    
    // /tools
    if (StartsWith(cmdLower, _T("/tools")) || StartsWith(cmdLower, _T("/tool"))) {
        ShowTools();
        return TRUE;
    }
    
    // /settings
    if (StartsWith(cmdLower, _T("/settings")) || 
        StartsWith(cmdLower, _T("/setting")) ||
        StartsWith(cmdLower, _T("/config")) ||
        StartsWith(cmdLower, _T("/cfg"))) {
        OpenSettings();
        return TRUE;
    }
    
    // /status
    if (StartsWith(cmdLower, _T("/status")) || StartsWith(cmdLower, _T("/info"))) {
        ShowStatus();
        return TRUE;
    }
    
    // /permission
    if (StartsWith(cmdLower, _T("/permission")) || 
        StartsWith(cmdLower, _T("/perm")) ||
        StartsWith(cmdLower, _T("/level"))) {
        ShowPermission();
        return TRUE;
    }
    
    // /gui
    if (StartsWith(cmdLower, _T("/gui")) || 
        StartsWith(cmdLower, _T("/mouse")) ||
        StartsWith(cmdLower, _T("/click"))) {
        ShowGuiInfo();
        return TRUE;
    }
    
    // /gui-action
    if (StartsWith(cmdLower, _T("/gui-action")) || StartsWith(cmdLower, _T("/ga "))) {
        LPCTSTR params = _tcschr(cmd, ' ');
        if (params) {
            params++;
            TCHAR action[64];
            TCHAR actionParams[512];
            
            TCHAR* space = _tcschr(params, ' ');
            if (space) {
                int len = space - params;
                _tcsncpy(action, params, len);
                action[len] = 0;
                _tcscpy(actionParams, space + 1);
            } else {
                _tcscpy(action, params);
                actionParams[0] = 0;
            }
            
            PrintGui(action);
            LPCTSTR result = GuiExecute(action, actionParams);
            PrintResult(result);
        } else {
            PrintError(_T("用法: /gui-action <操作> [参数]"));
        }
        return TRUE;
    }
    
    // /clear
    if (StartsWith(cmdLower, _T("/clear")) || StartsWith(cmdLower, _T("/cls"))) {
        ClearContext();
        PrintSuccess(_T("对话历史已清空"));
        return TRUE;
    }
    
    // /export
    if (StartsWith(cmdLower, _T("/export"))) {
        LPCTSTR path = _tcschr(cmd, ' ');
        if (path) {
            path++;
            int count = ExportContext(path);
            if (count > 0) {
                TCHAR msg[256];
                _stprintf(msg, _T("已导出 %d 条消息到: %s"), count, path);
                PrintSuccess(msg);
            } else {
                PrintError(_T("导出失败"));
            }
        } else {
            PrintError(_T("用法: /export <路径>"));
        }
        return TRUE;
    }
    
    // /import
    if (StartsWith(cmdLower, _T("/import"))) {
        LPCTSTR path = _tcschr(cmd, ' ');
        if (path) {
            path++;
            int count = ImportContext(path);
            if (count > 0) {
                TCHAR msg[256];
                _stprintf(msg, _T("已导入 %d 条消息"), count);
                PrintSuccess(msg);
            } else {
                PrintError(_T("导入失败"));
            }
        } else {
            PrintError(_T("用法: /import <路径>"));
        }
        return TRUE;
    }
    
    // /shell
    if (StartsWith(cmdLower, _T("/shell")) || 
        StartsWith(cmdLower, _T("/sh ")) ||
        StartsWith(cmdLower, _T("/exec"))) {
        LPCTSTR command = _tcschr(cmd, ' ');
        if (command) {
            command++;
            PrintTool(_T("shell"));
            LPCTSTR result = ExecuteShellCommand(command);
            PrintResult(result);
        } else {
            PrintError(_T("用法: /shell <命令>"));
        }
        return TRUE;
    }
    
    // /read
    if (StartsWith(cmdLower, _T("/read")) || StartsWith(cmdLower, _T("/cat"))) {
        LPCTSTR path = _tcschr(cmd, ' ');
        if (path) {
            path++;
            PrintTool(_T("read_file"));
            LPCTSTR result = ReadFileContent(path);
            PrintResult(result);
        } else {
            PrintError(_T("用法: /read <路径>"));
        }
        return TRUE;
    }
    
    // /ls
    if (StartsWith(cmdLower, _T("/ls")) || 
        StartsWith(cmdLower, _T("/dir")) ||
        StartsWith(cmdLower, _T("/list"))) {
        LPCTSTR path = _tcschr(cmd, ' ');
        TCHAR target[MAX_PATH] = _T(".");
        if (path) {
            _tcscpy(target, path + 1);
        }
        PrintTool(_T("list_dir"));
        LPCTSTR result = ListDirectory(target);
        PrintResult(result);
        return TRUE;
    }
    
    // /calc
    if (StartsWith(cmdLower, _T("/calc")) || StartsWith(cmdLower, _T("/calculate"))) {
        LPCTSTR expr = _tcschr(cmd, ' ');
        if (expr) {
            expr++;
            PrintTool(_T("calculator"));
            TCHAR msg[512];
            _stprintf(msg, _T("计算: %s\n\n(计算器功能待实现)"), expr);
            PrintResult(msg);
        } else {
            PrintError(_T("用法: /calc <表达式>"));
        }
        return TRUE;
    }
    
    // /sysinfo
    if (StartsWith(cmdLower, _T("/sysinfo")) || StartsWith(cmdLower, _T("/system"))) {
        PrintTool(_T("system_info"));
        LPCTSTR result = GetSystemInfoText();
        PrintResult(result);
        return TRUE;
    }
    
    // /write
    if (StartsWith(cmdLower, _T("/write"))) {
        LPCTSTR rest = _tcschr(cmd, ' ');
        if (rest) {
            rest++;
            TCHAR path[MAX_PATH];
            TCHAR* space = _tcschr(rest, ' ');
            if (space) {
                int len = space - rest;
                _tcsncpy(path, rest, len);
                path[len] = 0;
                LPCTSTR content = space + 1;
                
                PrintTool(_T("write_file"));
                LPCTSTR result = WriteFileContent(path, content);
                PrintResult(result);
            } else {
                PrintError(_T("用法: /write <路径> <内容>"));
            }
        } else {
            PrintError(_T("用法: /write <路径> <内容>"));
        }
        return TRUE;
    }
    
    // /ocr
    if (StartsWith(cmdLower, _T("/ocr"))) {
        LPCTSTR rest = _tcschr(cmd, ' ');
        if (rest) {
            rest++;
            
            // /ocr screen - 全屏识别
            if (StartsWith(rest, _T("screen")) || StartsWith(rest, _T("full"))) {
                PrintTool(_T("ocr"));
                LPCTSTR result = OcrFullScreen();
                PrintResult(result);
            }
            // /ocr region x y w h - 区域识别
            else if (StartsWith(rest, _T("region"))) {
                LPCTSTR params = _tcschr(rest, ' ');
                if (params) {
                    params++;
                    int x = 0, y = 0, w = 0, h = 0;
                    _stscanf(params, _T("%d %d %d %d"), &x, &y, &w, &h);
                    
                    if (w > 0 && h > 0) {
                        PrintTool(_T("ocr"));
                        LPCTSTR result = OcrScreenRegion(x, y, w, h);
                        PrintResult(result);
                    } else {
                        PrintError(_T("用法: /ocr region <x> <y> <宽> <高>"));
                    }
                } else {
                    PrintError(_T("用法: /ocr region <x> <y> <宽> <高>"));
                }
            }
            // /ocr <图片路径> - 识别图片文件
            else {
                PrintTool(_T("ocr"));
                LPCTSTR result = OcrImageFile(rest);
                PrintResult(result);
            }
        } else {
            SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
            _tprintf(_T("\n  OCR 识图命令:\n"));
            ResetColor();
            _tprintf(_T("  /ocr <图片路径>    识别图片中的文字\n"));
            _tprintf(_T("  /ocr screen        识别全屏文字\n"));
            _tprintf(_T("  /ocr region x y w h  识别指定区域\n\n"));
        }
        return TRUE;
    }
    
    // /read-skill
    if (StartsWith(cmdLower, _T("/read-skill")) || StartsWith(cmdLower, _T("/skill-doc"))) {
        LPCTSTR name = _tcschr(cmd, ' ');
        if (name) {
            name++;
            PrintTool(_T("read_skill_doc"));
            LPCTSTR doc = ReadSkillDoc(name);
            PrintResult(doc);
        } else {
            PrintError(_T("用法: /read-skill <技能名>"));
        }
        return TRUE;
    }
    
    // /run-skill
    if (StartsWith(cmdLower, _T("/run-skill")) || 
        StartsWith(cmdLower, _T("/runs")) ||
        StartsWith(cmdLower, _T("/skill-run")) ||
        StartsWith(cmdLower, _T("/execute-skill"))) {
        LPCTSTR rest = _tcschr(cmd, ' ');
        if (rest) {
            rest++;
            TCHAR name[64];
            TCHAR params[512] = {0};
            
            TCHAR* space = _tcschr(rest, ' ');
            if (space) {
                int len = space - rest;
                _tcsncpy(name, rest, len);
                name[len] = 0;
                _tcscpy(params, space + 1);
            } else {
                _tcscpy(name, rest);
            }
            
            PrintTool(_T("execute_skill"));
            LPCTSTR result = ExecuteSkill(name, params);
            PrintResult(result);
        } else {
            PrintError(_T("用法: /run-skill <技能名> [参数]"));
        }
        return TRUE;
    }
    
    // /plugin
    if (StartsWith(cmdLower, _T("/plugin")) || StartsWith(cmdLower, _T("/plugins")) ||
        StartsWith(cmdLower, _T("/pl "))) {
        LPCTSTR rest = _tcschr(cmd, ' ');
        
        if (!rest) {
            // 无参数，显示插件列表
            PrintTool(_T("plugin_list"));
            LPCTSTR result = GetPluginListText();
            PrintResult(result);
        } else {
            rest++;
            TCHAR subcmd[64];
            TCHAR params[512] = {0};
            
            TCHAR* space = _tcschr(rest, ' ');
            if (space) {
                int len = space - rest;
                _tcsncpy(subcmd, rest, len);
                subcmd[len] = 0;
                _tcscpy(params, space + 1);
            } else {
                _tcscpy(subcmd, rest);
            }
            
            ToLowerCase(subcmd);
            
            // list - 列出插件
            if (_tcscmp(subcmd, _T("list")) == 0) {
                PrintTool(_T("plugin_list"));
                LPCTSTR result = GetPluginListText();
                PrintResult(result);
            }
            // info - 查看详情
            else if (_tcscmp(subcmd, _T("info")) == 0) {
                if (params[0]) {
                    PrintTool(_T("plugin_info"));
                    LPCTSTR result = GetPluginInfoText(params);
                    PrintResult(result);
                } else {
                    PrintError(_T("用法: /plugin info <插件ID>"));
                }
            }
            // enable - 启用
            else if (_tcscmp(subcmd, _T("enable")) == 0) {
                if (params[0]) {
                    if (EnablePlugin(params)) {
                        TCHAR msg[256];
                        _stprintf(msg, _T("插件 '%s' 已启用"), params);
                        PrintSuccess(msg);
                    } else {
                        TCHAR msg[256];
                        _stprintf(msg, _T("启用插件 '%s' 失败"), params);
                        PrintError(msg);
                    }
                } else {
                    PrintError(_T("用法: /plugin enable <插件ID>"));
                }
            }
            // disable - 禁用
            else if (_tcscmp(subcmd, _T("disable")) == 0) {
                if (params[0]) {
                    if (DisablePlugin(params)) {
                        TCHAR msg[256];
                        _stprintf(msg, _T("插件 '%s' 已禁用"), params);
                        PrintSuccess(msg);
                    } else {
                        TCHAR msg[256];
                        _stprintf(msg, _T("禁用插件 '%s' 失败"), params);
                        PrintError(msg);
                    }
                } else {
                    PrintError(_T("用法: /plugin disable <插件ID>"));
                }
            }
            // run - 执行
            else if (_tcscmp(subcmd, _T("run")) == 0 || _tcscmp(subcmd, _T("exec")) == 0) {
                if (params[0]) {
                    // 分离插件ID和参数
                    TCHAR plugin_id[64];
                    TCHAR plugin_params[512] = {0};
                    
                    TCHAR* pspace = _tcschr(params, ' ');
                    if (pspace) {
                        int len = pspace - params;
                        _tcsncpy(plugin_id, params, len);
                        plugin_id[len] = 0;
                        _tcscpy(plugin_params, pspace + 1);
                    } else {
                        _tcscpy(plugin_id, params);
                    }
                    
                    PrintTool(_T("plugin_run"));
                    LPTSTR result = ExecutePlugin(plugin_id, plugin_params);
                    PrintResult(result);
                } else {
                    PrintError(_T("用法: /plugin run <插件ID> [参数]"));
                }
            }
            // install - 安装
            else if (_tcscmp(subcmd, _T("install")) == 0) {
                if (params[0]) {
                    if (InstallPlugin(params)) {
                        TCHAR msg[256];
                        _stprintf(msg, _T("插件安装成功: %s"), params);
                        PrintSuccess(msg);
                    } else {
                        TCHAR msg[256];
                        _stprintf(msg, _T("插件安装失败: %s"), params);
                        PrintError(msg);
                    }
                } else {
                    PrintError(_T("用法: /plugin install <DLL路径>"));
                }
            }
            // uninstall - 卸载
            else if (_tcscmp(subcmd, _T("uninstall")) == 0 || _tcscmp(subcmd, _T("remove")) == 0) {
                if (params[0]) {
                    if (UninstallPlugin(params)) {
                        TCHAR msg[256];
                        _stprintf(msg, _T("插件 '%s' 已卸载"), params);
                        PrintSuccess(msg);
                    } else {
                        TCHAR msg[256];
                        _stprintf(msg, _T("卸载插件 '%s' 失败"), params);
                        PrintError(msg);
                    }
                } else {
                    PrintError(_T("用法: /plugin uninstall <插件ID>"));
                }
            }
            // stats - 统计
            else if (_tcscmp(subcmd, _T("stats")) == 0 || _tcscmp(subcmd, _T("stat")) == 0) {
                PrintTool(_T("plugin_stats"));
                LPCTSTR result = GetPluginStatsText();
                PrintResult(result);
            }
            else {
                TCHAR msg[256];
                _stprintf(msg, _T("未知子命令: %s\n输入 /plugin 查看可用命令"), subcmd);
                PrintError(msg);
            }
        }
        return TRUE;
    }
    
    // /db
    if (StartsWith(cmdLower, _T("/db")) || StartsWith(cmdLower, _T("/database"))) {
        LPCTSTR rest = _tcschr(cmd, ' ');
        
        if (!rest) {
            // 无参数，显示数据库信息
            PrintTool(_T("database_info"));
            LPCTSTR result = GetDatabaseInfoText();
            PrintResult(result);
        } else {
            rest++;
            TCHAR subcmd[64];
            TCHAR params[512] = {0};
            
            TCHAR* space = _tcschr(rest, ' ');
            if (space) {
                int len = space - rest;
                _tcsncpy(subcmd, rest, len);
                subcmd[len] = 0;
                _tcscpy(params, space + 1);
            } else {
                _tcscpy(subcmd, rest);
            }
            
            ToLowerCase(subcmd);
            
            // info - 数据库信息
            if (_tcscmp(subcmd, _T("info")) == 0 || _tcscmp(subcmd, _T("status")) == 0) {
                PrintTool(_T("database_info"));
                LPCTSTR result = GetDatabaseInfoText();
                PrintResult(result);
            }
            // get - 获取值
            else if (_tcscmp(subcmd, _T("get")) == 0) {
                if (params[0]) {
                    // 分离表名和键
                    TCHAR table_name[64];
                    TCHAR key[256] = {0};
                    
                    TCHAR* kspace = _tcschr(params, ' ');
                    if (kspace) {
                        int len = kspace - params;
                        _tcsncpy(table_name, params, len);
                        table_name[len] = 0;
                        _tcscpy(key, kspace + 1);
                    } else {
                        _tcscpy(table_name, _T("settings"));
                        _tcscpy(key, params);
                    }
                    
                    int table = 1; // 默认 settings 表
                    if (_tcscmp(table_name, _T("plugins")) == 0) table = 0;
                    else if (_tcscmp(table_name, _T("settings")) == 0) table = 1;
                    else if (_tcscmp(table_name, _T("cache")) == 0) table = 2;
                    
                    LPCTSTR value = DBGetValue(table, key);
                    if (value) {
                        TCHAR msg[512];
                        _stprintf(msg, _T("%s = %s"), key, value);
                        PrintResult(msg);
                    } else {
                        TCHAR msg[256];
                        _stprintf(msg, _T("键 '%s' 不存在"), key);
                        PrintError(msg);
                    }
                } else {
                    PrintError(_T("用法: /db get [表名] <键>"));
                }
            }
            // set - 设置值
            else if (_tcscmp(subcmd, _T("set")) == 0) {
                if (params[0]) {
                    // 分离表名、键和值
                    TCHAR table_name[64];
                    TCHAR key[256];
                    TCHAR value[512] = {0};
                    
                    TCHAR* kspace = _tcschr(params, ' ');
                    if (kspace) {
                        int len = kspace - params;
                        _tcsncpy(table_name, params, len);
                        table_name[len] = 0;
                        
                        LPCTSTR rest2 = kspace + 1;
                        TCHAR* vspace = _tcschr(rest2, ' ');
                        if (vspace) {
                            int klen = vspace - rest2;
                            _tcsncpy(key, rest2, klen);
                            key[klen] = 0;
                            _tcscpy(value, vspace + 1);
                        } else {
                            _tcscpy(key, rest2);
                        }
                    } else {
                        _tcscpy(table_name, _T("settings"));
                        _tcscpy(key, params);
                    }
                    
                    int table = 1; // 默认 settings 表
                    if (_tcscmp(table_name, _T("plugins")) == 0) table = 0;
                    else if (_tcscmp(table_name, _T("settings")) == 0) table = 1;
                    else if (_tcscmp(table_name, _T("cache")) == 0) table = 2;
                    
                    if (DBSetValue(table, key, value)) {
                        TCHAR msg[512];
                        _stprintf(msg, _T("已设置: %s = %s"), key, value);
                        PrintSuccess(msg);
                    } else {
                        PrintError(_T("设置失败"));
                    }
                } else {
                    PrintError(_T("用法: /db set [表名] <键> <值>"));
                }
            }
            // delete - 删除键
            else if (_tcscmp(subcmd, _T("delete")) == 0 || _tcscmp(subcmd, _T("del")) == 0) {
                if (params[0]) {
                    TCHAR table_name[64];
                    TCHAR key[256] = {0};
                    
                    TCHAR* kspace = _tcschr(params, ' ');
                    if (kspace) {
                        int len = kspace - params;
                        _tcsncpy(table_name, params, len);
                        table_name[len] = 0;
                        _tcscpy(key, kspace + 1);
                    } else {
                        _tcscpy(table_name, _T("settings"));
                        _tcscpy(key, params);
                    }
                    
                    int table = 1;
                    if (_tcscmp(table_name, _T("plugins")) == 0) table = 0;
                    else if (_tcscmp(table_name, _T("settings")) == 0) table = 1;
                    else if (_tcscmp(table_name, _T("cache")) == 0) table = 2;
                    
                    if (DBDeleteKey(table, key)) {
                        TCHAR msg[256];
                        _stprintf(msg, _T("已删除键: %s"), key);
                        PrintSuccess(msg);
                    } else {
                        TCHAR msg[256];
                        _stprintf(msg, _T("删除键 '%s' 失败"), key);
                        PrintError(msg);
                    }
                } else {
                    PrintError(_T("用法: /db delete [表名] <键>"));
                }
            }
            // clear - 清空表
            else if (_tcscmp(subcmd, _T("clear")) == 0) {
                TCHAR table_name[64] = _T("cache");
                if (params[0]) {
                    _tcscpy(table_name, params);
                }
                
                int table = 2; // 默认 cache 表
                if (_tcscmp(table_name, _T("plugins")) == 0) table = 0;
                else if (_tcscmp(table_name, _T("settings")) == 0) table = 1;
                else if (_tcscmp(table_name, _T("cache")) == 0) table = 2;
                
                if (DBClearTable(table)) {
                    TCHAR msg[256];
                    _stprintf(msg, _T("表 '%s' 已清空"), table_name);
                    PrintSuccess(msg);
                } else {
                    PrintError(_T("清空表失败"));
                }
            }
            else {
                TCHAR msg[256];
                _stprintf(msg, _T("未知子命令: %s\n输入 /db 查看可用命令"), subcmd);
                PrintError(msg);
            }
        }
        return TRUE;
    }
    
    // /exit
    if (StartsWith(cmdLower, _T("/exit")) || 
        StartsWith(cmdLower, _T("/quit")) ||
        StartsWith(cmdLower, _T("/q"))) {
        PrintInfo(_T("再见！"));
        exit(0);
        return TRUE;
    }
    
    // 未知命令
    TCHAR msg[256];
    _stprintf(msg, _T("未知命令: %s\n输入 /help 查看可用命令"), cmd);
    PrintError(msg);
    return TRUE;
}

// 显示帮助
void ShowHelp() {
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n════════════════════════════════════════════════════════════════\n"));
    _tprintf(_T("  可用命令\n"));
    _tprintf(_T("════════════════════════════════════════════════════════════════\n\n"));
    ResetColor();
    
    struct {
        LPCTSTR cmd;
        LPCTSTR desc;
    } commands[] = {
        {_T("/help, /?"), _T("显示帮助")},
        {_T("/skills, /skill"), _T("列出所有技能")},
        {_T("/tools, /tool"), _T("列出所有工具")},
        {_T("/settings, /config"), _T("打开设置面板")},
        {_T("/status, /info"), _T("显示状态信息")},
        {_T("/permission, /level"), _T("查看权限等级")},
        {_T("/gui, /mouse"), _T("查看 GUI 工具")},
        {_T("/gui-action <操作>"), _T("执行GUI操作")},
        {_T("/clear, /cls"), _T("清空对话历史")},
        {_T("/export <路径>"), _T("导出上下文为JSON")},
        {_T("/import <路径>"), _T("从JSON导入上下文")},
        {_T("/shell <命令>"), _T("执行shell命令")},
        {_T("/read <路径>"), _T("读取文件内容")},
        {_T("/ls [路径]"), _T("列出目录内容")},
        {_T("/calc <表达式>"), _T("数学计算")},
        {_T("/sysinfo"), _T("查看系统信息")},
        {_T("/run-skill <技能>"), _T("执行技能")},
        {_T("/read-skill <技能>"), _T("读取技能文档")},
        {_T("/exit, /quit"), _T("退出程序")},
        {NULL, NULL}
    };
    
    for (int i = 0; commands[i].cmd; i++) {
        SetColor(COLOR_YELLOW);
        _tprintf(_T("  %-25s"), commands[i].cmd);
        ResetColor();
        _tprintf(_T(" %s\n"), commands[i].desc);
    }
    
    _tprintf(_T("\n"));
}

// 显示状态
void ShowStatus() {
    SetColor(COLOR_CYAN | FOREGROUND_INTENSITY);
    _tprintf(_T("\n════════════════════════════════════════════════════════════════\n"));
    _tprintf(_T("  系统状态\n"));
    _tprintf(_T("════════════════════════════════════════════════════════════════\n\n"));
    ResetColor();
    
    _tprintf(_T("  版本: %s %s\n\n"), APP_NAME, APP_VERSION);
    
    _tprintf(_T("  权限等级: Level %d - %s\n"), 
        g_config.permission_level, 
        GetPermissionLevelName(g_config.permission_level));
    
    LPCTSTR modeName = _T("未知");
    switch (g_config.operation_mode) {
        case MODE_CLI: modeName = _T("仅命令行"); break;
        case MODE_GUI: modeName = _T("仅模拟点击"); break;
        case MODE_BOTH: modeName = _T("混合模式"); break;
    }
    _tprintf(_T("  操作模式: %s\n\n"), modeName);
    
    _tprintf(_T("  AI 后端: %s\n"), g_config.ai_backend);
    if (g_config.cloud_api_url[0]) {
        _tprintf(_T("  API 地址: %s\n"), g_config.cloud_api_url);
    }
    _tprintf(_T("\n"));
    
    _tprintf(_T("  对话历史: %d 条\n"), g_context.count);
    _tprintf(_T("  可用技能: %d 个\n\n"), g_skills.count);
    
    _tprintf(_T("  GUI 控制: %s\n"), GuiIsAvailable() ? _T("可用") : _T("不可用"));
    _tprintf(_T("  配置文件: %s\n\n"), g_config_file);
}

// 显示技能
void ShowSkills() {
    TCHAR buffer[4096];
    ListSkillsText(buffer, 4096);
    PrintResult(buffer);
}

// 显示工具
void ShowTools() {
    SetColor(COLOR_YELLOW | FOREGROUND_INTENSITY);
    _tprintf(_T("\n════════════════════════════════════════════════════════════════\n"));
    _tprintf(_T("  可用工具\n"));
    _tprintf(_T("════════════════════════════════════════════════════════════════\n\n"));
    ResetColor();
    
    struct {
        LPCTSTR name;
        LPCTSTR desc;
    } tools[] = {
        {_T("shell"), _T("执行终端命令")},
        {_T("read_file"), _T("读取文件内容")},
        {_T("write_file"), _T("写入文件")},
        {_T("list_dir"), _T("列出目录内容")},
        {_T("calculator"), _T("数学计算")},
        {_T("system_info"), _T("系统信息")},
        {_T("list_skills"), _T("列出可用技能")},
        {_T("read_skill_doc"), _T("读取技能文档")},
        {_T("execute_skill"), _T("执行技能")},
        {NULL, NULL}
    };
    
    for (int i = 0; tools[i].name; i++) {
        SetColor(COLOR_YELLOW);
        _tprintf(_T("  - %s"), tools[i].name);
        ResetColor();
        _tprintf(_T(": %s\n"), tools[i].desc);
    }
    
    if (g_config.operation_mode != MODE_CLI) {
        SetColor(COLOR_MAGENTA);
        _tprintf(_T("  - gui_action"));
        ResetColor();
        _tprintf(_T(": GUI模拟操作\n"));
    }
    
    _tprintf(_T("\n"));
}

// 打开设置
void OpenSettings() {
    RunSettingsPanel();
}

// 显示权限
void ShowPermission() {
    PrintPermissionInfo();
}

// 显示GUI信息
void ShowGuiInfo() {
    PrintGuiTools();
}
