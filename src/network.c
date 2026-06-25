/*
 * 网络请求模块
 * 纯 WinINet API 实现
 */
#include "hacker.h"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

static HINTERNET hInternet = NULL;

// 初始化网络
BOOL InitNetwork() {
    hInternet = InternetOpen(
        _T("HackerTerminal/1.0"),
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0
    );
    
    return (hInternet != NULL);
}

// 解析 URL
static BOOL ParseUrl(LPCTSTR url, TCHAR* host, int host_max, TCHAR* path, int path_max, int* port, BOOL* isHttps) {
    if (!url || !host || !path || !port || !isHttps) return FALSE;
    
    *isHttps = FALSE;
    *port = 80;
    
    // 跳过协议前缀
    LPCTSTR url_start = url;
    
    if (_tcsstr(url, _T("https://")) == url) {
        *isHttps = TRUE;
        *port = 443;
        url_start = url + 8;
    } else if (_tcsstr(url, _T("http://")) == url) {
        url_start = url + 7;
    }
    
    // 提取 host 和 path
    LPCTSTR slash = _tcschr(url_start, '/');
    int host_len;
    
    if (slash) {
        host_len = slash - url_start;
        _tcsncpy(path, slash, path_max - 1);
        path[path_max - 1] = 0;
    } else {
        host_len = _tcslen(url_start);
        _tcscpy(path, _T("/"));
    }
    
    if (host_len >= host_max) host_len = host_max - 1;
    _tcsncpy(host, url_start, host_len);
    host[host_len] = 0;
    
    // 检查端口
    TCHAR* colon = _tcschr(host, ':');
    if (colon) {
        *colon = 0;
        *port = _ttoi(colon + 1);
        if (*port <= 0 || *port > 65535) {
            *port = *isHttps ? 443 : 80;
        }
    }
    
    return TRUE;
}

// HTTP POST JSON 请求
LPCTSTR HttpPostJson(LPCTSTR url, LPCTSTR jsonData, LPCTSTR apiKey) {
    static TCHAR result[16384];
    result[0] = 0;
    
    if (!hInternet) {
        return _T("网络未初始化");
    }
    
    if (!url || !jsonData) {
        return _T("参数错误");
    }
    
    // 解析URL
    TCHAR host[256], path[1024];
    int port = 443;
    BOOL isHttps = FALSE;
    
    if (!ParseUrl(url, host, 256, path, 1024, &port, &isHttps)) {
        return _T("URL解析失败");
    }
    
    // 连接服务器
    HINTERNET hConnect = InternetConnect(
        hInternet,
        host,
        (INTERNET_PORT)port,
        NULL,
        NULL,
        INTERNET_SERVICE_HTTP,
        0,
        0
    );
    
    if (!hConnect) {
        return _T("连接服务器失败");
    }
    
    // 创建请求
    DWORD flags = isHttps ? INTERNET_FLAG_SECURE : 0;
    HINTERNET hRequest = HttpOpenRequest(
        hConnect,
        _T("POST"),
        path,
        NULL,
        NULL,
        NULL,
        flags,
        0
    );
    
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        return _T("创建请求失败");
    }
    
    // 设置请求头
    TCHAR headers[2048];
    _stprintf(headers, 
        _T("Content-Type: application/json\r\n")
        _T("Authorization: Bearer %s\r\n"),
        apiKey ? apiKey : _T("")
    );
    
    HttpAddRequestHeaders(hRequest, headers, _tcslen(headers), HTTP_ADDREQ_FLAG_ADD);
    
    // 转换为 UTF-8
    char* utf8Data = NULL;
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, jsonData, -1, NULL, 0, NULL, NULL);
    if (utf8Len > 0) {
        utf8Data = (char*)malloc(utf8Len);
        if (utf8Data) {
            WideCharToMultiByte(CP_UTF8, 0, jsonData, -1, utf8Data, utf8Len, NULL, NULL);
        }
    }
    
    // 发送请求
    BOOL sendResult = HttpSendRequest(
        hRequest,
        NULL,
        0,
        utf8Data,
        utf8Data ? utf8Len - 1 : 0
    );
    
    if (utf8Data) free(utf8Data);
    
    if (!sendResult) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        return _T("发送请求失败");
    }
    
    // 读取响应（使用更大的初始缓冲区减少 realloc 次数）
    #define INITIAL_BUFFER_SIZE 8192
    #define READ_CHUNK_SIZE 4096
    
    char* response = (char*)malloc(INITIAL_BUFFER_SIZE);
    if (!response) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        return _T("内存分配失败");
    }
    
    int totalLen = 0;
    int bufferSize = INITIAL_BUFFER_SIZE;
    char buffer[READ_CHUNK_SIZE];
    DWORD bytesRead = 0;
    
    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        
        // 确保缓冲区足够大
        if (totalLen + bytesRead + 1 > bufferSize) {
            // 翻倍增长策略，减少 realloc 次数
            int newSize = bufferSize * 2;
            while (newSize < totalLen + bytesRead + 1) {
                newSize *= 2;
            }
            
            char* newBuffer = (char*)realloc(response, newSize);
            if (!newBuffer) {
                free(response);
                InternetCloseHandle(hRequest);
                InternetCloseHandle(hConnect);
                return _T("内存分配失败");
            }
            response = newBuffer;
            bufferSize = newSize;
        }
        
        memcpy(response + totalLen, buffer, bytesRead);
        totalLen += bytesRead;
        response[totalLen] = 0;
    }
    
    // 转换为UTF-16
    if (totalLen > 0) {
        int wLen = MultiByteToWideChar(CP_UTF8, 0, response, totalLen, NULL, 0);
        if (wLen > 0 && wLen < 16000) {
            MultiByteToWideChar(CP_UTF8, 0, response, totalLen, result, wLen);
            result[wLen] = 0;
        } else if (wLen >= 16000) {
            _tcscpy(result, _T("响应内容过长"));
        }
    }
    
    free(response);
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    
    return result;
}

// 调用云端AI
LPCTSTR CallCloudAI(LPCTSTR messages) {
    if (g_config.cloud_api_url[0] == 0 || g_config.cloud_api_key[0] == 0) {
        return _T("请先配置云端API地址和密钥");
    }
    
    if (!messages) {
        return _T("消息内容为空");
    }
    
    // 动态分配请求体，避免缓冲区溢出
    int msg_len = _tcslen(messages);
    int model_len = _tcslen(g_config.cloud_model_name[0] ? g_config.cloud_model_name : _T("gpt-3.5-turbo"));
    
    // 估算需要的缓冲区大小
    int needed = 128 + model_len + msg_len;
    TCHAR* requestBody = (TCHAR*)malloc(needed * sizeof(TCHAR));
    if (!requestBody) {
        return _T("内存分配失败");
    }
    
    _stprintf(requestBody, 
        _T("{\"model\":\"%s\",\"messages\":%s,\"stream\":false}"),
        g_config.cloud_model_name[0] ? g_config.cloud_model_name : _T("gpt-3.5-turbo"),
        messages
    );
    
    LPCTSTR result = HttpPostJson(g_config.cloud_api_url, requestBody, g_config.cloud_api_key);
    
    // 注意：result 是静态缓冲区，不需要释放
    // 但我们需要把结果复制出来，因为 requestBody 释放后 result 还在
    // 实际上 HttpPostJson 返回的是静态缓冲区，所以这里直接返回即可
    
    free(requestBody);
    return result;
}
