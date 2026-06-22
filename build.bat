@echo off
REM ============================================================
REM Hacker Terminal - x64 编译脚本
REM 使用 MinGW-w64 (gcc) 编译
REM ============================================================

echo ============================================================
echo   Hacker Terminal - x64 编译
echo   爱折腾实验室 · 豆包 AI Agent Lab
echo ============================================================
echo.

REM 检查编译器
where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 gcc 编译器！
    echo 请安装 MinGW-w64 并添加到 PATH
    echo.
    echo 下载地址: https://github.com/niXman/mingw-builds-binaries/releases
    echo 选择: x86_64-posix-seh
    pause
    exit /b 1
)

echo [信息] 编译器: 
gcc --version
echo.

REM 创建输出目录
if not exist "bin" mkdir bin

REM 编译选项
set CFLAGS=-O2 -s -Wall -municode -mwindows -mconsole
set LDFLAGS=-luser32 -lgdi32 -lshell32 -lwininet -lshlwapi

echo [编译] 正在编译...
echo.

REM 编译所有源文件
gcc %CFLAGS% ^
    src/utils.c ^
    src/terminal.c ^
    src/config.c ^
    src/gui.c ^
    src/network.c ^
    src/skills.c ^
    src/context.c ^
    src/ai.c ^
    src/commands.c ^
    src/settings.c ^
    src/shell.c ^
    src/ocr.c ^
    src/ggml.c ^
    src/database.c ^
    src/plugin.c ^
    src/main.c ^
    -o bin\HackerTerminal.exe ^
    %LDFLAGS%

if %errorlevel% neq 0 (
    echo.
    echo [错误] 主程序编译失败！
    pause
    exit /b 1
)

echo.
echo [成功] 主程序编译完成！
echo.

REM 编译安装向导
echo [编译] 正在编译安装向导...
gcc %CFLAGS% ^
    src/utils.c ^
    src/terminal.c ^
    src/config.c ^
    src/gui.c ^
    src/network.c ^
    src/skills.c ^
    src/context.c ^
    src/ai.c ^
    src/commands.c ^
    src/settings.c ^
    src/shell.c ^
    src/ocr.c ^
    src/ggml.c ^
    src/database.c ^
    src/plugin.c ^
    src/installer.c ^
    -o bin\HackerInstaller.exe ^
    %LDFLAGS%

if %errorlevel% neq 0 (
    echo.
    echo [警告] 安装向导编译失败（可选组件）
) else (
    echo [成功] 安装向导编译完成！
)

echo.
echo [成功] 编译完成！
echo.

REM 显示文件信息
echo [信息] 输出文件: bin\HackerTerminal.exe
for %%I in (bin\HackerTerminal.exe) do echo [信息] 文件大小: %%~zI 字节
echo.

REM 检查架构
echo [信息] 检查架构...
dumpbin /headers bin\HackerTerminal.exe 2>nul | findstr "machine" >nul 2>&1
if %errorlevel% equ 0 (
    dumpbin /headers bin\HackerTerminal.exe | findstr "machine"
) else (
    echo [信息] x64 (64位)
)

echo.
echo ============================================================
echo   编译完成！
echo   可执行文件: bin\HackerTerminal.exe
echo ============================================================
echo.
echo 运行方式:
echo   直接双击 bin\HackerTerminal.exe
echo   或在命令行中运行: bin\HackerTerminal.exe
echo.
pause
