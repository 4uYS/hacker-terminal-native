/*
 * Shell 命令执行模块
 * 纯 Win32 API 实现
 */

#include "hacker.h"

// 危险命令列表（需要高权限）
typedef struct {
    LPCTSTR command;
    int required_level;
} DangerousCommand;

static DangerousCommand dangerous_cmds[] = {
    {_T("rm -rf"), 3},
    {_T("rm -r"), 3},
    {_T("del /s /q"), 3},
    {_T("deltree"), 3},
    {_T("format"), 5},
    {_T("mkfs"), 5},
    {_T("fdisk"), 5},
    {_T("diskpart"), 5},
    {_T("sudo"), 7},
    {_T("runas"), 7},
    {_T("chmod 777"), 7},
    {_T("chown root"), 7},
    {_T("takeown"), 7},
    {_T("dd if="), 8},
    {_T("rm -rf /"), 8},
    {_T("rm -rf /*"), 10},
    {_T("del /f /s /q c:\\"), 10},
    {_T("format c:"), 10},
    {_T("rd /s /q c:\\"), 10},
    {NULL, 0}
};

// 检查命令是否危险
int CheckDangerousCommand(LPCTSTR cmd) {
    TCHAR cmdLower[1024];
    _tcsncpy(cmdLower, cmd, 1023);
    cmdLower[1023] = 0;
    ToLowerCase(cmdLower);
    
    for (int i = 0; dangerous_cmds[i].command; i++) {
        if (_tcsstr(cmdLower, dangerous_cmds[i].command) != NULL) {
            return dangerous_cmds[i].required_level;
        }
    }
    
    return 0;
}

// 检查是否可以执行命令
BOOL CanExecuteCommand(LPCTSTR cmd, int* required_level) {
    int level = CheckDangerousCommand(cmd);
    if (required_level) *required_level = level;
    
    if (level == 0) return TRUE;
    return (g_config.permission_level >= level);
}

// 执行命令并捕获输出
LPCTSTR ExecuteShellCommand(LPCTSTR command) {
    static TCHAR result[8192];
    result[0] = 0;
    
    // 权限检查
    int required_level = 0;
    if (!CanExecuteCommand(command, &required_level)) {
        _stprintf(result, 
            _T("[权限不足] 该命令需要 Level %d 权限\n")
            _T("当前权限: Level %d - %s\n")
            _T("请在设置中提升权限等级后再试"),
            required_level,
            g_config.permission_level,
            GetPermissionLevelName(g_config.permission_level)
        );
        return result;
    }
    
    // 危险操作确认
    if (required_level >= 3 && g_config.confirm_dangerous) {
        _stprintf(result, 
            _T("[警告] 这是一个危险操作！\n")
            _T("需要权限等级: Level %d\n")
            _T("命令: %s\n\n")
            _T("请确认你知道自己在做什么。\n")
            _T("(可在设置中关闭危险操作确认)"),
            required_level,
            command
        );
        return result;
    }
    
    // 创建管道用于捕获输出
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    
    HANDLE hStdOutRead = NULL;
    HANDLE hStdOutWrite = NULL;
    
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &saAttr, 0)) {
        _tcscpy(result, _T("创建管道失败"));
        return result;
    }
    
    // 确保读取端不被继承
    SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);
    
    // 创建进程
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = hStdOutWrite;
    siStartInfo.hStdOutput = hStdOutWrite;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    siStartInfo.wShowWindow = SW_HIDE;
    
    // 构建命令行
    TCHAR cmdLine[2048];
    _stprintf(cmdLine, _T("cmd.exe /c %s"), command);
    
    BOOL bSuccess = CreateProcess(
        NULL,
        cmdLine,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &siStartInfo,
        &piProcInfo
    );
    
    if (!bSuccess) {
        CloseHandle(hStdOutRead);
        CloseHandle(hStdOutWrite);
        _stprintf(result, _T("执行失败，错误码: %d"), GetLastError());
        return result;
    }
    
    // 关闭写入端
    CloseHandle(hStdOutWrite);
    
    // 读取输出
    char buffer[4096];
    DWORD bytesRead = 0;
    char* output = (char*)malloc(1);
    output[0] = 0;
    int totalLen = 0;
    
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        output = (char*)realloc(output, totalLen + bytesRead + 1);
        memcpy(output + totalLen, buffer, bytesRead);
        totalLen += bytesRead;
        output[totalLen] = 0;
        
        // 限制最大输出
        if (totalLen > 8000) break;
    }
    
    // 等待进程结束
    WaitForSingleObject(piProcInfo.hProcess, 30000);  // 30秒超时
    
    // 获取退出码
    DWORD exitCode = 0;
    GetExitCodeProcess(piProcInfo.hProcess, &exitCode);
    
    // 转换为UTF-16
    if (totalLen > 0) {
        int wLen = MultiByteToWideChar(CP_ACP, 0, output, totalLen, NULL, 0);
        if (wLen > 0 && wLen < 8000) {
            MultiByteToWideChar(CP_ACP, 0, output, totalLen, result, wLen);
            result[wLen] = 0;
        }
    }
    
    // 添加退出码信息
    if (totalLen == 0) {
        _stprintf(result, _T("(命令执行完成，无输出)\n退出码: %d"), exitCode);
    } else if (exitCode != 0) {
        TCHAR exitInfo[128];
        _stprintf(exitInfo, _T("\n[退出码: %d]"), exitCode);
        if (_tcslen(result) + _tcslen(exitInfo) < 8190) {
            _tcscat(result, exitInfo);
        }
    }
    
    // 清理
    free(output);
    CloseHandle(hStdOutRead);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    
    return result;
}

// 获取系统信息
LPCTSTR GetSystemInfoText() {
    static TCHAR result[2048];
    
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&ms);
    
    TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD nameSize = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(computerName, &nameSize);
    
    TCHAR userName[256];
    DWORD userSize = 256;
    GetUserName(userName, &userSize);
    
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    
    _stprintf(result,
        _T("系统信息:\n")
        _T("──────────────────────────────\n")
        _T("  计算机名: %s\n")
        _T("  用户名: %s\n")
        _T("  操作系统: Windows %d.%d\n")
        _T("  处理器架构: %d\n")
        _T("  处理器数量: %d\n")
        _T("  内存总量: %llu MB\n")
        _T("  可用内存: %llu MB\n")
        _T("  内存使用率: %d%%\n")
        _T("  页面文件: %llu MB\n")
        _T("──────────────────────────────"),
        computerName,
        userName,
        osvi.dwMajorVersion,
        osvi.dwMinorVersion,
        si.wProcessorArchitecture,
        si.dwNumberOfProcessors,
        ms.ullTotalPhys / 1024 / 1024,
        ms.ullAvailPhys / 1024 / 1024,
        ms.dwMemoryLoad,
        ms.ullTotalPageFile / 1024 / 1024
    );
    
    return result;
}

// 读取文件内容
LPCTSTR ReadFileContent(LPCTSTR path) {
    static TCHAR result[8192];
    result[0] = 0;
    
    HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        _stprintf(result, _T("无法打开文件: %s\n错误码: %d"), path, GetLastError());
        return result;
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        _tcscpy(result, _T("无法获取文件大小"));
        return result;
    }
    
    // 限制读取大小
    if (fileSize > 8000) fileSize = 8000;
    
    char* buffer = (char*)malloc(fileSize + 1);
    DWORD bytesRead = 0;
    ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
    buffer[bytesRead] = 0;
    
    CloseHandle(hFile);
    
    // 尝试UTF-8转换
    int wLen = MultiByteToWideChar(CP_UTF8, 0, buffer, bytesRead, NULL, 0);
    if (wLen > 0 && wLen < 8000) {
        MultiByteToWideChar(CP_UTF8, 0, buffer, bytesRead, result, wLen);
        result[wLen] = 0;
    } else {
        // 尝试ANSI
        wLen = MultiByteToWideChar(CP_ACP, 0, buffer, bytesRead, NULL, 0);
        if (wLen > 0 && wLen < 8000) {
            MultiByteToWideChar(CP_ACP, 0, buffer, bytesRead, result, wLen);
            result[wLen] = 0;
        }
    }
    
    free(buffer);
    return result;
}

// 列出目录
LPCTSTR ListDirectory(LPCTSTR path) {
    static TCHAR result[8192];
    result[0] = 0;
    
    TCHAR searchPath[MAX_PATH];
    _stprintf(searchPath, _T("%s\\*"), path);
    
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath, &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        _stprintf(result, _T("无法打开目录: %s"), path);
        return result;
    }
    
    TCHAR line[512];
    int fileCount = 0;
    int dirCount = 0;
    
    SafeAppend(result, 8192, _T("目录列表: "));
    SafeAppend(result, 8192, path);
    SafeAppend(result, 8192, _T("\n──────────────────────────────\n"));
    
    do {
        if (_tcscmp(findData.cFileName, _T(".")) == 0 ||
            _tcscmp(findData.cFileName, _T("..")) == 0) {
            continue;
        }
        
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            SetColor(COLOR_BLUE | FOREGROUND_INTENSITY);
            _stprintf(line, _T("  [DIR]  %s\n"), findData.cFileName);
            dirCount++;
        } else {
            ULONGLONG fileSize = ((ULONGLONG)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
            if (fileSize > 1024 * 1024) {
                _stprintf(line, _T("  %6.1f MB  %s\n"), (float)fileSize / 1024 / 1024, findData.cFileName);
            } else if (fileSize > 1024) {
                _stprintf(line, _T("  %6.1f KB  %s\n"), (float)fileSize / 1024, findData.cFileName);
            } else {
                _stprintf(line, _T("  %6lld B   %s\n"), (long long)fileSize, findData.cFileName);
            }
            fileCount++;
        }
        
        SafeAppend(result, 8192, line);
        
    } while (FindNextFile(hFind, &findData) && _tcslen(result) < 7500);
    
    FindClose(hFind);
    
    _stprintf(line, _T("──────────────────────────────\n共 %d 个目录, %d 个文件\n"), dirCount, fileCount);
    SafeAppend(result, 8192, line);
    
    return result;
}

// 写入文件
LPCTSTR WriteFileContent(LPCTSTR path, LPCTSTR content) {
    static TCHAR result[1024];
    
    // 权限检查
    if (g_config.permission_level < 2) {
        _stprintf(result, _T("[权限不足] 写入文件需要 Level 2 权限"));
        return result;
    }
    
    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        _stprintf(result, _T("无法创建文件: %s\n错误码: %d"), path, GetLastError());
        return result;
    }
    
    // 转换为UTF-8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content, -1, NULL, 0, NULL, NULL);
    char* utf8Content = (char*)malloc(utf8Len);
    WideCharToMultiByte(CP_UTF8, 0, content, -1, utf8Content, utf8Len, NULL, NULL);
    
    DWORD bytesWritten = 0;
    WriteFile(hFile, utf8Content, utf8Len - 1, &bytesWritten, NULL);
    
    CloseHandle(hFile);
    free(utf8Content);
    
    _stprintf(result, _T("文件已写入: %s\n大小: %d 字节"), path, bytesWritten);
    return result;
}
