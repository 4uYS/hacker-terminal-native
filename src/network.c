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

// HTTP POST JSON 请求
LPCTSTR HttpPostJson(LPCTSTR url, LPCTSTR jsonData, LPCTSTR apiKey) {
    static TCHAR result[8192];
    result[0] = 0;
    
    if (!hInternet) {
        return _T("网络未初始化");
    }
    
    // 解析URL
    TCHAR host[256], path[1024];
    int port = 443;
    BOOL isHttps = FALSE;
    
    TCHAR* urlCopy = StrDup(url);
    
    // 判断是否HTTPS
    if (_tcsstr(urlCopy, _T("https://")) != NULL) {
        isHttps = TRUE;
        _tcscpy(urlCopy, urlCopy + 8);
    } else if (_tcsstr(urlCopy, _T("http://")) != NULL) {
        _tcscpy(urlCopy, urlCopy + 7);
    }
    
    // 提取host和path
    TCHAR* slash = _tcschr(urlCopy, '/');
    if (slash) {
        *slash = 0;
        _tcscpy(host, urlCopy);
        _tcscpy(path, slash);
        *slash = '/';
    } else {
        _tcscpy(host, urlCopy);
        _tcscpy(path, _T("/"));
    }
    
    // 检查端口
    TCHAR* colon = _tcschr(host, ':');
    if (colon) {
        *colon = 0;
        port = _ttoi(colon + 1);
    }
    
    // 连接服务器
    HINTERNET hConnect = InternetConnect(
        hInternet,
        host,
        port,
        NULL,
        NULL,
        INTERNET_SERVICE_HTTP,
        0,
        0
    );
    
    if (!hConnect) {
        free(urlCopy);
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
        free(urlCopy);
        return _T("创建请求失败");
    }
    
    // 设置请求头
    TCHAR headers[1024];
    _stprintf(headers, 
        _T("Content-Type: application/json\r\n")
        _T("Authorization: Bearer %s\r\n"),
        apiKey
    );
    
    HttpAddRequestHeaders(hRequest, headers, _tcslen(headers), HTTP_ADDREQ_FLAG_ADD);
    
    // 发送请求
    char* utf8Data = NULL;
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, jsonData, -1, NULL, 0, NULL, NULL);
    if (utf8Len > 0) {
        utf8Data = (char*)malloc(utf8Len);
        WideCharToMultiByte(CP_UTF8, 0, jsonData, -1, utf8Data, utf8Len, NULL, NULL);
    }
    
    BOOL sendResult = HttpSendRequest(
        hRequest,
        NULL,
        0,
        utf8Data,
        utf8Len ? utf8Len - 1 : 0
    );
    
    if (utf8Data) free(utf8Data);
    
    if (!sendResult) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        free(urlCopy);
        return _T("发送请求失败");
    }
    
    // 读取响应
    char buffer[4096];
    DWORD bytesRead = 0;
    char* response = (char*)malloc(1);
    response[0] = 0;
    int totalLen = 0;
    
    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        response = (char*)realloc(response, totalLen + bytesRead + 1);
        memcpy(response + totalLen, buffer, bytesRead);
        totalLen += bytesRead;
        response[totalLen] = 0;
    }
    
    // 转换为UTF-16
    if (totalLen > 0) {
        int wLen = MultiByteToWideChar(CP_UTF8, 0, response, totalLen, NULL, 0);
        if (wLen > 0 && wLen < 8000) {
            MultiByteToWideChar(CP_UTF8, 0, response, totalLen, result, wLen);
            result[wLen] = 0;
        }
    }
    
    free(response);
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    free(urlCopy);
    
    return result;
}

// 调用云端AI
LPCTSTR CallCloudAI(LPCTSTR messages) {
    if (g_config.cloud_api_url[0] == 0 || g_config.cloud_api_key[0] == 0) {
        return _T("请先配置云端API地址和密钥");
    }
    
    // 构建请求体
    TCHAR requestBody[4096];
    _stprintf(requestBody, 
        _T("{\"model\":\"%s\",\"messages\":%s,\"stream\":false}"),
        g_config.cloud_model_name[0] ? g_config.cloud_model_name : _T("gpt-3.5-turbo"),
        messages
    );
    
    return HttpPostJson(g_config.cloud_api_url, requestBody, g_config.cloud_api_key);
}
