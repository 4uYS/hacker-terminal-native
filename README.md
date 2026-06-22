# Hacker Terminal v2.1 (原生 Win32 x64 版)

> 爱折腾实验室 · 豆包 AI Agent Lab 出品

专为终端折腾打造的 AI 助手，纯原生 Win32 API 实现，零第三方库依赖。

---

## ✨ 特性

- 🚀 **零依赖**：纯 Win32 API 实现，不依赖任何第三方库
- 💻 **超轻量**：单文件 exe，体积仅 ~250KB
- 🎨 **Hacker 风格**：终端彩色输出，酷炫界面
- 🔒 **权限分级**：0-10 级权限控制，安全可控
- 🖱️ **GUI 模拟**：支持鼠标点击、键盘输入、截图等操作
- 📦 **技能系统**：可扩展的技能插件机制，内置 5 个实用技能
- 💾 **上下文管理**：内存存储，支持 JSON 导入导出
- ⚙️ **交互式设置**：终端设置面板，方向键操作
- 🌐 **云端 AI**：支持 OpenAI 兼容 API
- 🧠 **本地推理**：GGUF 格式本地大模型支持（框架）
- 👁️ **OCR 识图**：Windows 内置 OCR 支持（框架）
- 💻 **完整 Shell**：危险命令检测、权限检查、管道捕获输出
- 📋 **安装向导**：5 步交互式安装流程
- 🔌 **插件系统**：支持官方插件和用户自制插件，DLL 动态加载
- 🗄️ **本地数据库**：轻量级键值存储，3 个数据表

---

## 📦 文件结构

```
hacker-terminal-native/
├── src/                    # 源代码
│   ├── hacker.h           # 主头文件
│   ├── main.c             # 主程序入口
│   ├── utils.c            # 工具函数
│   ├── terminal.c         # 终端界面
│   ├── config.c           # 配置管理
│   ├── gui.c              # GUI 控制
│   ├── network.c          # 网络请求
│   ├── skills.c           # 技能系统
│   ├── context.c          # 上下文管理
│   ├── ai.c               # AI 引擎
│   ├── commands.c         # 命令系统
│   ├── settings.c         # 设置面板
│   ├── shell.c            # Shell 命令执行引擎
│   ├── ocr.c              # OCR 识图模块
│   ├── ggml.c             # GGUF 本地推理引擎
│   ├── database.c         # 本地数据库模块
│   ├── plugin.c           # 插件管理器
│   └── installer.c        # 安装向导程序
├── resources/              # 资源文件
│   └── version.rc         # 版本资源
├── skills/                 # 技能目录
│   ├── demo-skill/        # 示例技能
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
├── build.bat              # 编译脚本
└── README.md              # 说明文档
```

---

## 🚀 快速开始

### 编译

1. 安装 MinGW-w64 (x86_64-posix-seh)
2. 运行编译脚本：

```cmd
build.bat
```

3. 编译成功后，可执行文件在 `bin/` 目录：
   - `HackerTerminal.exe` - 主程序
   - `HackerInstaller.exe` - 安装向导

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
| `/help`, `/?` | 显示帮助 |
| `/skills` | 列出所有技能 |
| `/tools` | 列出所有工具 |
| `/settings` | 打开设置面板 |
| `/status` | 显示状态信息 |
| `/permission` | 查看权限等级 |
| `/gui` | 查看 GUI 工具 |
| `/clear` | 清空对话历史 |
| `/export <路径>` | 导出上下文为 JSON |
| `/import <路径>` | 从 JSON 导入上下文 |
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
| `/run-skill <技能>` | 执行技能 |
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
- **官方插件**：由爱折腾实验室官方开发
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

### 内置技能

#### 1. 文件管理 (file-manager)

- 复制、移动、删除、重命名
- 创建目录、查看大小
- 批量重命名

#### 2. 文本处理 (text-processor)

- grep 查找、sed 替换
- sort 排序、uniq 去重
- wc 统计、iconv 编码转换
- merge 合并、split 拆分

#### 3. 系统监控 (system-monitor)

- CPU、内存、磁盘使用
- 进程列表、网络连接
- 系统信息、结束进程
- 可视化进度条显示

#### 4. 网络工具 (network-tools)

- ping、curl、wget
- nslookup、portscan
- ifconfig、tracert
- speedtest、myip

#### 5. 文档编辑器 (doc-editor)

- 创建、编辑、查看文档
- 格式转换（Markdown/HTML/CSV/JSON）
- 合并、搜索替换、预览
- 中文字数统计

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

## 🛠️ 技术栈

- **语言**：纯 C (C99)
- **API**：Win32 API
- **编译器**：MinGW-w64 (GCC)
- **架构**：x64 (64位)
- **依赖**：零第三方库，仅系统 DLL

### 系统 DLL 依赖

- `user32.dll` - GUI 操作
- `gdi32.dll` - 图形绘制
- `shell32.dll` - Shell 操作
- `wininet.dll` - 网络请求
- `shlwapi.dll` - 路径操作
- `advapi32.dll` - 注册表/配置

---

## 📊 体积预估

| 版本 | 体积 |
|------|------|
| 纯功能版 | ~300 KB |
| 含安装向导 | ~600 KB (2个exe) |
| 含本地小模型 | ~50 MB+ (需额外下载模型) |

---

## 🎯 路线图

- ✅ 完整的 shell 命令执行
- ✅ 本地大模型推理 (GGUF) - 框架
- ✅ 识图功能 (Windows OCR) - 框架
- ✅ 更多内置技能
- ✅ 安装向导程序
- ✅ 插件系统（官方/用户分类）
- ✅ 本地数据库
- ⏳ 完整的 GGUF 推理实现
- ⏳ ARM64 架构支持
- ⏳ 多语言支持
- ⏳ 更多官方插件

---

## 📝 注意事项

1. **安全第一**：高权限等级请谨慎使用，避免误操作
2. **GUI 模式**：AI 运行时请勿操作鼠标键盘
3. **紧急终止**：可使用快捷键终止（GUI 模式）
4. **本地模型**：需要额外下载 GGUF 模型文件
5. **OCR 功能**：需要 Windows 10 及以上系统
6. **编译环境**：需要 MinGW-w64 或 Visual Studio
7. **插件安全**：请仅安装可信来源的插件
8. **数据库**：数据库文件为纯文本，请勿手动编辑

---

## 📄 许可证

MIT License

---

## 👥 关于

**爱折腾实验室 · 豆包 AI Agent Lab**
