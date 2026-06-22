# 插件开发指南

## 概述

Hacker Terminal 支持插件扩展功能，分为**官方插件**和**用户自制插件**两类。

- **官方插件**：由爱折腾实验室官方开发，位于 `plugins/official/` 目录
- **用户插件**：由用户自行开发，位于 `plugins/user/` 目录

## 插件结构

每个插件是一个独立的目录，包含一个 `plugin.dll` 文件：

```
plugins/
├── official/          # 官方插件目录
│   └── hello-world/
│       └── plugin.dll
└── user/              # 用户插件目录
    └── my-plugin/
        └── plugin.dll
```

## 插件接口规范

插件 DLL 必须导出以下 4 个函数：

### 1. GetPluginInfo()

获取插件基本信息。

```c
__declspec(dllexport) LPTSTR GetPluginInfo();
```

**返回格式**：`name|version|author|description`

示例：
```
Hello World|1.0.0|爱折腾实验室|官方示例插件
```

### 2. InitPlugin()

初始化插件，在插件加载时调用。

```c
__declspec(dllexport) BOOL InitPlugin();
```

**返回值**：
- `TRUE` - 初始化成功
- `FALSE` - 初始化失败

### 3. Execute(params)

执行插件功能。

```c
__declspec(dllexport) LPTSTR Execute(LPTSTR params);
```

**参数**：
- `params` - 用户传入的参数字符串

**返回值**：执行结果字符串

### 4. ShutdownPlugin()

关闭插件，在插件卸载时调用。

```c
__declspec(dllexport) BOOL ShutdownPlugin();
```

**返回值**：
- `TRUE` - 关闭成功
- `FALSE` - 关闭失败

## 编译插件

### 使用 MinGW-w64

```bash
gcc -shared -o plugin.dll plugin.c -luser32
```

### 使用 MSVC

```cmd
cl /LD plugin.c user32.lib
```

## 安装插件

### 方法一：命令安装

在 Hacker Terminal 中执行：

```
/plugin install <DLL文件路径>
```

### 方法二：手动安装

1. 在 `plugins/user/` 目录下创建插件目录
2. 将编译好的 `plugin.dll` 放入该目录
3. 重启 Hacker Terminal 或重新扫描

## 插件命令

| 命令 | 说明 |
|------|------|
| `/plugin` | 显示插件列表 |
| `/plugin list` | 列出所有插件 |
| `/plugin info <id>` | 查看插件详情 |
| `/plugin enable <id>` | 启用插件 |
| `/plugin disable <id>` | 禁用插件 |
| `/plugin run <id> [参数]` | 执行插件 |
| `/plugin install <路径>` | 安装插件 |
| `/plugin uninstall <id>` | 卸载插件 |
| `/plugin stats` | 插件统计 |

## 插件状态

插件有以下几种状态：

- **已禁用** - 插件已安装但未启用
- **已启用** - 插件已启用，但尚未加载到内存
- **已加载** - 插件 DLL 已加载到内存，可以执行
- **错误** - 插件加载或执行出错

## 注意事项

1. 插件名称（目录名）即为插件 ID，必须唯一
2. 插件 DLL 文件名必须为 `plugin.dll`
3. 插件执行时请确保返回的字符串是静态分配的，不要返回栈上指针
4. 建议插件在 `InitPlugin` 中进行初始化，在 `ShutdownPlugin` 中清理资源
5. 用户插件请放在 `plugins/user/` 目录，官方插件放在 `plugins/official/` 目录

## 示例插件

官方提供了 `hello-world` 示例插件，位于 `plugins/official/hello-world/` 目录。

用户插件模板位于 `plugins/user/template/` 目录，可以基于此开发自己的插件。
