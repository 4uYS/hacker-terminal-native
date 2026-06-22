# Hacker Terminal v2.1 (原生 Win32 x64 版)

> 专为终端折腾打造的 AI 助手

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform: Windows](https://img.shields.io/badge/Platform-Windows-0078D4.svg)]()
[![Version: v2.1](https://img.shields.io/badge/Version-v2.1-orange.svg)]()

[English](README.en.md) | [中文](README.md)

</div>

---

## 📖 项目简介

Hacker Terminal 是一款专为终端折腾打造的 AI 助手，采用纯原生 Win32 API 实现，零第三方库依赖。它将 AI 能力与终端操作完美结合，支持 Shell 命令执行、GUI 模拟操作、技能系统、插件扩展等丰富功能。

### 为什么选择 Hacker Terminal？

- 🚀 **零依赖**：纯 Win32 API 实现，不依赖任何第三方库
- 💻 **超轻量**：单文件 exe，体积仅 ~250KB
- 🎨 **Hacker 风格**：终端彩色输出，酷炫界面
- 🔒 **安全可控**：0-10 级权限控制，危险命令检测
- 🔌 **高度可扩展**：技能系统 + 插件系统，无限可能

---

## ✨ 特性

### 核心特性

| 特性 | 说明 |
|------|------|
| **零依赖** | 纯 Win32 API 实现，不依赖任何第三方库 |
| **超轻量** | 单文件 exe，体积仅 ~250KB |
| **Hacker 风格** | 终端彩色输出，酷炫界面 |
| **权限分级** | 0-10 级权限控制，安全可控 |
| **GUI 模拟** | 支持鼠标点击、键盘输入、截图等操作 |
| **技能系统** | 可扩展的技能插件机制，内置 6 个实用技能 |
| **上下文管理** | 内存存储，支持 JSON 导入导出 |
| **交互式设置** | 终端设置面板，方向键操作 |
| **云端 AI** | 支持 OpenAI 兼容 API |
| **本地推理** | GGUF 格式本地大模型支持（框架） |
| **OCR 识图** | Windows 内置 OCR 支持（框架） |
| **完整 Shell** | 危险命令检测、权限检查、管道捕获输出 |
| **安装向导** | 5 步交互式安装流程 |
| **插件系统** | 支持官方插件和用户自制插件，DLL 动态加载 |
| **本地数据库** | 轻量级键值存储，3 个数据表 |

### 内置技能

| 技能 | 说明 |
|------|------|
| 📁 **文件管理** | 复制、移动、删除、重命名、批量操作 |
| 📝 **文本处理** | grep、sed、sort、uniq、wc、编码转换 |
| 🖥️ **系统监控** | CPU、内存、磁盘、进程、网络连接 |
| 🌐 **网络工具** | ping、curl、wget、nslookup、端口扫描 |
| 📄 **文档编辑器** | 创建、编辑、格式转换、预览 |
| 🎯 **示例技能** | 技能开发模板和示例 |

---

## 📦 文件结构

```
hacker-terminal-native/
├── src/                    # 源代码目录
│   ├── hacker.h           # 主头文件（数据结构和函数声明）
│   ├── main.c             # 主程序入口
│   ├── utils.c            # 工具函数库
│   ├── terminal.c         # 终端界面渲染
│   ├── config.c           # 配置管理
│   ├── gui.c              # GUI 控制（鼠标、键盘、截图）
│   ├── network.c          # 网络请求（HTTP/HTTPS）
│   ├── skills.c           # 技能系统
│   ├── context.c          # 上下文管理（对话历史）
│   ├── ai.c               # AI 引擎（云端/本地）
│   ├── commands.c         # 命令系统（斜杠命令）
│   ├── settings.c         # 设置面板（交互式）
│   ├── shell.c            # Shell 命令执行引擎
│   ├── ocr.c              # OCR 识图模块
│   ├── ggml.c             # GGUF 本地推理引擎
│   ├── database.c         # 本地数据库模块
│   ├── plugin.c           # 插件管理器
│   └── installer.c        # 安装向导程序
├── resources/              # 资源文件
│   └── version.rc         # 版本资源（Windows 资源文件）
├── skills/                 # 技能目录
│   ├── demo-skill/        # 示例技能（开发模板）
│   ├── file-manager/      # 文件管理技能
│   ├── text-processor/    # 文本处理技能
│   ├── system-monitor/    # 系统监控技能
│   ├── network-tools/     # 网络工具技能
│   └── doc-editor/        # 文档编辑器技能
├── plugins/                # 插件目录
│   ├── official/          # 官方插件
│   │   └── hello-world/   # Hello World 示例插件
│   ├── user/              # 用户插件
│   │   └── template/      # 用户插件模板
│   └── README.md          # 插件开发指南
├── build.bat              # 编译脚本（MinGW-w64）
├── README.md              # 中文说明文档
└── README.en.md           # 英文说明文档
```

---

## 🚀 快速开始

### 编译

#### 环境要求

- Windows 7 及以上系统
- MinGW-w64 (x86_64-posix-seh) 或 Visual Studio

#### 使用编译脚本

```cmd
build.bat
```

编译成功后，可执行文件在 `bin/` 目录：
- `HackerTerminal.exe` - 主程序
- `HackerInstaller.exe` - 安装向导

#### 手动编译（MinGW-w64）

```bash
gcc -O2 -Wall -mwindows -o bin/HackerTerminal.exe src/*.c -luser32 -lgdi32 -lshell32 -lwininet -lshlwapi -ladvapi32
```

### 运行

**首次运行（推荐）**：

```cmd
HackerInstaller.exe
```

运行安装向导，完成初始配置后自动启动主程序。

**直接运行**：

```cmd
HackerTerminal.exe
```

---

## 📖 使用说明

### 基础命令

| 命令 | 说明 |
|------|------|
| `/help`, `/?` | 显示帮助信息 |
| `/skills` | 列出所有可用技能 |
| `/tools` | 列出所有工具 |
| `/settings` | 打开设置面板（方向键操作） |
| `/status` | 显示当前状态信息 |
| `/permission` | 查看当前权限等级 |
| `/gui` | 查看 GUI 工具列表 |
| `/clear` | 清空对话历史 |
| `/export <路径>` | 导出上下文为 JSON 文件 |
| `/import <路径>` | 从 JSON 文件导入上下文 |
| `/shell <命令>` | 执行 shell 命令 |
| `/read <路径>` | 读取文件内容 |
| `/ls [路径]` | 列出目录内容 |
| `/write <路径> <内容>` | 写入文件内容 |
| `/calc <表达式>` | 数学计算 |
| `/sysinfo` | 查看系统信息 |
| `/ocr <图片路径>` | OCR 识别图片文字 |
| `/model <子命令>` | 本地模型管理 |
| `/plugin <子命令>` | 插件管理 |
| `/db <子命令>` | 数据库操作 |
| `/run-skill <技能>` | 执行指定技能 |
| `/read-skill <技能>` | 读取技能文档 |
| `/exit` | 退出程序 |

### Shell 命令执行

完整的 shell 命令执行引擎，支持：

- ✅ 危险命令检测（20种危险命令分级）
- ✅ 权限检查（0-10级）
- ✅ 危险操作确认提示
- ✅ CreateProcess + 管道捕获输出
- ✅ 30秒超时保护
- ✅ 退出码显示

**危险命令分级**：

| 等级 | 命令示例 |
|------|---------|
| 3级 | `rm -rf`, `rm -r`, `del /s /q` |
| 5级 | `format`, `mkfs`, `fdisk`, `diskpart` |
| 7级 | `sudo`, `runas`, `chmod 777` |
| 8级 | `dd if=`, `rm -rf /` |
| 10级 | `rm -rf /*`, `format c:`, `del /f /s /q c:\` |

### 本地模型 (GGUF)

支持 GGUF 格式本地大模型推理（框架实现）：

```
/model load <模型路径>    # 加载 GGUF 模型
/model info               # 查看模型信息
/model unload             # 卸载模型
```

支持的量化格式：F32, F16, Q4_0, Q4_1, Q5_0, Q5_1, Q8_0

### OCR 识图

调用 Windows 内置 OCR API（框架实现）：

```
/ocr <图片路径>           # 识别图片文件中的文字
/ocr screen               # 全屏识别
/ocr region x y w h       # 识别指定屏幕区域
```

支持格式：PNG, JPG, BMP, GIF, TIFF

### 插件系统

支持官方插件和用户自制插件，DLL 动态加载：

```
/plugin                   # 显示插件列表
/plugin list              # 列出所有插件
/plugin info <id>         # 查看插件详情
/plugin enable <id>       # 启用插件
/plugin disable <id>      # 禁用插件
/plugin run <id> [参数]   # 执行插件
/plugin install <路径>    # 安装插件
/plugin uninstall <id>    # 卸载插件
/plugin stats             # 插件统计
```

**插件分类**：
- **官方插件**：由爱折腾的实验室官方开发
- **用户插件**：由用户自行开发

**插件状态**：
- 已禁用 - 插件已安装但未启用
- 已启用 - 插件已启用，但尚未加载
- 已加载 - 插件 DLL 已加载到内存
- 错误 - 插件加载或执行出错

### 本地数据库

轻量级键值存储，基于文件系统：

```
/db                       # 显示数据库信息
/db info                  # 数据库状态信息
/db get [表名] <键>       # 获取值（默认 settings 表）
/db set [表名] <键> <值>  # 设置值
/db delete [表名] <键>    # 删除键
/db clear [表名]          # 清空表（默认 cache 表）
```

**可用表**：
- `plugins` - 插件表，存储插件状态
- `settings` - 设置表，存储用户设置
- `cache` - 缓存表，存储临时数据

数据库位置：`%APPDATA%\HackerTerminal\database\`

### GUI 操作

| 操作 | 说明 |
|------|------|
| `click x y` | 点击指定位置 |
| `move x y` | 移动鼠标 |
| `type <文本>` | 输入文本 |
| `hotkey <按键>` | 按下快捷键 |
| `scroll <数量>` | 滚动鼠标 |
| `position` | 获取鼠标位置 |
| `screenshot <路径>` | 截图保存 |
| `size` | 获取屏幕大小 |
| `drag x1 y1 x2 y2` | 拖拽 |

---

## 🔐 权限等级

| 等级 | 名称 | 说明 |
|------|------|------|
| 0 | 只读模式 | 只能读取文件和系统信息 |
| 1 | 查看模式 | 可以查看和分析 |
| 2 | 文档模式 | 可以创建和编辑文档 |
| 3 | 文件管理 | 可以管理文件和文件夹 |
| 4 | 开发模式 | 可以执行开发命令 |
| 5 | 标准模式 | 可以执行大部分安全命令 |
| 6 | 高级模式 | 可以执行系统管理命令 |
| 7 | 管理员模式 | 可以执行大部分系统操作 |
| 8 | 高级管理员 | 可以执行高风险系统操作 |
| 9 | 几乎完全控制 | 几乎可以执行所有操作 |
| 10 | 完全控制 | 可以执行任意操作 |

---

## 🎛️ 操作模式

| 模式 | 说明 |
|------|------|
| CLI 模式 | 仅使用命令行操作 |
| GUI 模式 | 仅使用模拟点击操作 |
| 混合模式 | 命令行和模拟点击结合使用 |

---

## 📋 安装向导

5 步交互式安装流程：

1. **欢迎界面** - 介绍产品信息
2. **权限等级设置** - 选择初始权限等级
3. **操作模式选择** - 选择 CLI/GUI/混合模式
4. **AI 模型配置** - 选择 AI 后端和模型
5. **完成安装** - 保存配置并启动

配置文件位置：`%APPDATA%\HackerTerminal\config.ini`

---

## 🔌 插件开发

### 插件接口规范

插件 DLL 必须导出以下 4 个函数：

```c
// 获取插件信息，返回格式：name|version|author|description
__declspec(dllexport) LPTSTR GetPluginInfo();

// 初始化插件
__declspec(dllexport) BOOL InitPlugin();

// 执行插件功能
__declspec(dllexport) LPTSTR Execute(LPTSTR params);

// 关闭插件
__declspec(dllexport) BOOL ShutdownPlugin();
```

### 编译插件

**MinGW-w64**：

```bash
gcc -shared -o plugin.dll plugin.c -luser32
```

**MSVC**：

```cmd
cl /LD plugin.c user32.lib
```

### 安装插件

```
/plugin install <DLL文件路径>
```

或手动将 `plugin.dll` 放入 `plugins/user/你的插件名/` 目录。

详细开发指南请参考 `plugins/README.md`。

---

## 🛠️ 技术实现

### 技术栈

- **语言**：纯 C (C99)
- **API**：Win32 API
- **编译器**：MinGW-w64 (GCC)
- **架构**：x64 (64位)
- **依赖**：零第三方库，仅系统 DLL

### 系统 DLL 依赖

| DLL | 说明 |
|-----|------|
| `user32.dll` | GUI 操作（鼠标、键盘、窗口） |
| `gdi32.dll` | 图形绘制（截图、位图） |
| `shell32.dll` | Shell 操作 |
| `wininet.dll` | 网络请求（HTTP/HTTPS） |
| `shlwapi.dll` | 路径操作 |
| `advapi32.dll` | 注册表/配置 |

### 体积预估

| 版本 | 体积 |
|------|------|
| 纯功能版 | ~300 KB |
| 含安装向导 | ~600 KB (2个exe) |
| 含本地小模型 | ~50 MB+ (需额外下载模型) |

---

## 🎯 路线图

### 已完成 ✅

- ✅ 完整的 shell 命令执行
- ✅ 本地大模型推理 (GGUF) - 框架
- ✅ 识图功能 (Windows OCR) - 框架
- ✅ 更多内置技能
- ✅ 安装向导程序
- ✅ 插件系统（官方/用户分类）
- ✅ 本地数据库

### 开发中 ⏳

- ⏳ 完整的 GGUF 推理实现
- ⏳ ARM64 架构支持
- ⏳ 多语言支持
- ⏳ 更多官方插件

### 未来计划 🔮

- [ ] 插件市场集成
- [ ] 技能商店
- [ ] 主题系统
- [ ] 脚本自动化
- [ ] 远程控制功能

---

## ⚠️ 注意事项

1. **安全第一**：高权限等级请谨慎使用，避免误操作
2. **GUI 模式**：AI 运行时请勿操作鼠标键盘
3. **紧急终止**：可使用快捷键终止（GUI 模式）
4. **本地模型**：需要额外下载 GGUF 模型文件
5. **OCR 功能**：需要 Windows 10 及以上系统
6. **编译环境**：需要 MinGW-w64 或 Visual Studio
7. **插件安全**：请仅安装可信来源的插件
8. **数据库**：数据库文件为纯文本，请勿手动编辑

---

## ❓ 常见问题

### Q: Hacker Terminal 和普通终端有什么区别？

A: Hacker Terminal 不仅是一个终端，更是一个 AI 助手。它内置了 AI 能力，可以理解自然语言指令，自动执行复杂任务。同时它还拥有技能系统和插件系统，可以无限扩展功能。

### Q: 为什么用纯 Win32 API 实现？

A: 纯 Win32 API 实现意味着零第三方依赖，体积小，启动快，兼容性好。不需要安装 .NET、Python、Java 等运行时环境，下载即用。

### Q: 权限等级有什么用？

A: 权限等级是安全机制的核心。通过限制 AI 的操作权限，可以防止误操作或恶意指令造成的损害。你可以根据使用场景选择合适的权限等级。

### Q: 如何开发自己的插件？

A: 参考 `plugins/README.md` 中的开发指南。你只需要实现 4 个导出函数，编译成 DLL 即可。支持 C/C++ 等任何能编译成 DLL 的语言。

### Q: 支持哪些 AI 模型？

A: 云端支持所有 OpenAI 兼容的 API。本地支持 GGUF 格式的大模型（框架已实现，完整功能开发中）。

---

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

### 贡献方式

1. Fork 本仓库
2. 创建你的特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交你的更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

### 开发技能

参考 `skills/demo-skill/` 中的示例，开发自己的技能包。

### 开发插件

参考 `plugins/README.md` 中的开发指南，开发自己的插件。

---

## 📄 许可证

MIT License

---

<div align="center">

**Made with ❤️ by 爱折腾的实验室**

*专注于创造有趣且实用的技术项目*

</div>
