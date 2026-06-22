/*
 * AI 引擎模块
 */

#include "hacker.h"

// 初始化AI
void InitAI() {
    // 添加系统提示词
    AddMessage(_T("system"), GetSystemPrompt());
}

// 获取系统提示词
LPCTSTR GetSystemPrompt() {
    static TCHAR prompt[4096];
    prompt[0] = 0;
    
    SafeAppend(prompt, 4096, _T("你是 Hacker 模型，由爱折腾实验室 · 豆包 AI Agent Lab 研发的终端AI助手。\n\n"));
    SafeAppend(prompt, 4096, _T("你的特点：\n"));
    SafeAppend(prompt, 4096, _T("- 擅长终端操作和系统管理\n"));
    SafeAppend(prompt, 4096, _T("- 风格简洁高效，Hacker 风格\n"));
    SafeAppend(prompt, 4096, _T("- 喜欢折腾各种技术\n\n"));
    
    // 权限信息
    TCHAR permInfo[256];
    _stprintf(permInfo, _T("当前权限等级: Level %d - %s\n\n"), 
        g_config.permission_level, 
        GetPermissionLevelName(g_config.permission_level));
    SafeAppend(prompt, 4096, permInfo);
    
    // 操作模式
    LPCTSTR modeName = _T("未知");
    switch (g_config.operation_mode) {
        case MODE_CLI: modeName = _T("仅命令行"); break;
        case MODE_GUI: modeName = _T("仅模拟点击"); break;
        case MODE_BOTH: modeName = _T("混合模式"); break;
    }
    TCHAR modeInfo[128];
    _stprintf(modeInfo, _T("操作模式: %s\n\n"), modeName);
    SafeAppend(prompt, 4096, modeInfo);
    
    // 可用工具
    SafeAppend(prompt, 4096, _T("可用工具:\n"));
    SafeAppend(prompt, 4096, _T("- shell: 执行终端命令\n"));
    SafeAppend(prompt, 4096, _T("- read_file: 读取文件内容\n"));
    SafeAppend(prompt, 4096, _T("- write_file: 写入文件\n"));
    SafeAppend(prompt, 4096, _T("- list_dir: 列出目录\n"));
    SafeAppend(prompt, 4096, _T("- calculator: 数学计算\n"));
    SafeAppend(prompt, 4096, _T("- system_info: 系统信息\n"));
    
    if (g_config.operation_mode != MODE_CLI) {
        SafeAppend(prompt, 4096, _T("- gui_action: GUI模拟操作（点击、输入、截图等）\n"));
    }
    
    if (g_config.share_skills_list) {
        SafeAppend(prompt, 4096, _T("- list_skills: 列出可用技能\n"));
        SafeAppend(prompt, 4096, _T("- read_skill_doc: 读取技能文档\n"));
        SafeAppend(prompt, 4096, _T("- execute_skill: 执行技能\n"));
    }
    
    SafeAppend(prompt, 4096, _T("\n请用简洁、高效的方式回答用户问题。\n"));
    
    return prompt;
}

// 生成回复
LPCTSTR GenerateResponse(LPCTSTR user_input) {
    static TCHAR response[4096];
    
    // 添加用户消息
    AddMessage(_T("user"), user_input);
    
    // 根据后端选择
    if (StrEquals(g_config.ai_backend, _T("cloud")) && 
        g_config.cloud_api_url[0] != 0 && 
        g_config.cloud_api_key[0] != 0) {
        
        // 云端API模式
        TCHAR messages[8192];
        GetContextMessages(messages, 8192);
        
        LPCTSTR apiResponse = CallCloudAI(messages);
        
        // 简单解析响应（提取content）
        // 实际项目中需要完整的JSON解析
        _tcscpy(response, apiResponse);
        
    } else {
        // 规则模式（降级）
        _stprintf(response, 
            _T("收到你的消息: \"%s\"\n\n")
            _T("当前为规则模式，未配置云端AI API。\n")
            _T("请在设置中配置云端API地址和密钥以启用完整AI功能。\n\n")
            _T("可用命令:\n")
            _T("  /help     - 查看帮助\n")
            _T("  /skills   - 查看技能\n")
            _T("  /settings - 打开设置\n")
            _T("  /status   - 查看状态\n")
            _T("  /clear    - 清空对话\n"),
            user_input
        );
    }
    
    // 添加助手回复
    AddMessage(_T("assistant"), response);
    
    return response;
}
