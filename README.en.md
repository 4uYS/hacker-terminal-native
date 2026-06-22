# Hacker Terminal v2.1 (Native Win32 x64)

> An AI assistant built for terminal tinkering

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform: Windows](https://img.shields.io/badge/Platform-Windows-0078D4.svg)]()
[![Version: v2.1](https://img.shields.io/badge/Version-v2.1-orange.svg)]()

[English](README.en.md) | [中文](README.md)

</div>

---

## 📖 Introduction

Hacker Terminal is an AI assistant built for terminal tinkering, implemented with pure native Win32 API with zero third-party library dependencies. It perfectly combines AI capabilities with terminal operations, supporting Shell command execution, GUI simulation, skill system, plugin extension, and many other rich features.

### Why Choose Hacker Terminal?

- 🚀 **Zero Dependencies**: Pure Win32 API implementation, no third-party libraries required
- 💻 **Ultra-Lightweight**: Single-file exe, only ~250KB in size
- 🎨 **Hacker Style**: Colorful terminal output, cool interface
- 🔒 **Safe & Controllable**: 0-10 level permission control, dangerous command detection
- 🔌 **Highly Extensible**: Skill system + plugin system, unlimited possibilities

---

## ✨ Features

### Core Features

| Feature | Description |
|---------|-------------|
| **Zero Dependencies** | Pure Win32 API implementation, no third-party libraries |
| **Ultra-Lightweight** | Single-file exe, only ~250KB |
| **Hacker Style** | Colorful terminal output, cool interface |
| **Permission Levels** | 0-10 level permission control, safe and controllable |
| **GUI Simulation** | Supports mouse clicks, keyboard input, screenshots, etc. |
| **Skill System** | Extensible skill plugin mechanism, 6 built-in practical skills |
| **Context Management** | Memory storage, supports JSON import/export |
| **Interactive Settings** | Terminal settings panel, arrow key operation |
| **Cloud AI** | Supports OpenAI compatible API |
| **Local Inference** | GGUF format local LLM support (framework) |
| **OCR Recognition** | Windows built-in OCR support (framework) |
| **Full Shell** | Dangerous command detection, permission check, pipe capture output |
| **Installation Wizard** | 5-step interactive installation process |
| **Plugin System** | Supports official and user-made plugins, DLL dynamic loading |
| **Local Database** | Lightweight key-value storage, 3 data tables |

### Built-in Skills

| Skill | Description |
|-------|-------------|
| 📁 **File Manager** | Copy, move, delete, rename, batch operations |
| 📝 **Text Processor** | grep, sed, sort, uniq, wc, encoding conversion |
| 🖥️ **System Monitor** | CPU, memory, disk, processes, network connections |
| 🌐 **Network Tools** | ping, curl, wget, nslookup, port scanning |
| 📄 **Document Editor** | Create, edit, format conversion, preview |
| 🎯 **Demo Skill** | Skill development template and example |

---

## 📦 File Structure

```
hacker-terminal-native/
├── src/                    # Source code directory
│   ├── hacker.h           # Main header file (data structures and function declarations)
│   ├── main.c             # Main program entry
│   ├── utils.c            # Utility function library
│   ├── terminal.c         # Terminal interface rendering
│   ├── config.c           # Configuration management
│   ├── gui.c              # GUI control (mouse, keyboard, screenshots)
│   ├── network.c          # Network requests (HTTP/HTTPS)
│   ├── skills.c           # Skill system
│   ├── context.c          # Context management (chat history)
│   ├── ai.c               # AI engine (cloud/local)
│   ├── commands.c         # Command system (slash commands)
│   ├── settings.c         # Settings panel (interactive)
│   ├── shell.c            # Shell command execution engine
│   ├── ocr.c              # OCR recognition module
│   ├── ggml.c             # GGUF local inference engine
│   ├── database.c         # Local database module
│   ├── plugin.c           # Plugin manager
│   └── installer.c        # Installation wizard program
├── resources/              # Resource files
│   └── version.rc         # Version resource (Windows resource file)
├── skills/                 # Skills directory
│   ├── demo-skill/        # Demo skill (development template)
│   ├── file-manager/      # File manager skill
│   ├── text-processor/    # Text processor skill
│   ├── system-monitor/    # System monitor skill
│   ├── network-tools/     # Network tools skill
│   └── doc-editor/        # Document editor skill
├── plugins/                # Plugins directory
│   ├── official/          # Official plugins
│   │   └── hello-world/   # Hello World example plugin
│   ├── user/              # User plugins
│   │   └── template/      # User plugin template
│   └── README.md          # Plugin development guide
├── build.bat              # Compilation script (MinGW-w64)
├── README.md              # Chinese documentation
└── README.en.md           # English documentation
```

---

## 🚀 Quick Start

### Compilation

#### Requirements

- Windows 7 or above
- MinGW-w64 (x86_64-posix-seh) or Visual Studio

#### Using Build Script

```cmd
build.bat
```

After successful compilation, executable files are in the `bin/` directory:
- `HackerTerminal.exe` - Main program
- `HackerInstaller.exe` - Installation wizard

#### Manual Compilation (MinGW-w64)

```bash
gcc -O2 -Wall -mwindows -o bin/HackerTerminal.exe src/*.c -luser32 -lgdi32 -lshell32 -lwininet -lshlwapi -ladvapi32
```

### Running

**First run (recommended)**:

```cmd
HackerInstaller.exe
```

Run the installation wizard, complete initial configuration, and automatically start the main program.

**Direct run**:

```cmd
HackerTerminal.exe
```

---

## 📖 Usage

### Basic Commands

| Command | Description |
|---------|-------------|
| `/help`, `/?` | Show help information |
| `/skills` | List all available skills |
| `/tools` | List all tools |
| `/settings` | Open settings panel (arrow key operation) |
| `/status` | Show current status information |
| `/permission` | View current permission level |
| `/gui` | View GUI tool list |
| `/clear` | Clear chat history |
| `/export <path>` | Export context as JSON file |
| `/import <path>` | Import context from JSON file |
| `/shell <command>` | Execute shell command |
| `/read <path>` | Read file content |
| `/ls [path]` | List directory contents |
| `/write <path> <content>` | Write file content |
| `/calc <expression>` | Math calculation |
| `/sysinfo` | View system information |
| `/ocr <image_path>` | OCR recognize image text |
| `/model <subcommand>` | Local model management |
| `/plugin <subcommand>` | Plugin management |
| `/db <subcommand>` | Database operations |
| `/run-skill <skill>` | Execute specified skill |
| `/read-skill <skill>` | Read skill documentation |
| `/exit` | Exit program |

### Shell Command Execution

Complete shell command execution engine, supports:

- ✅ Dangerous command detection (20 types of dangerous command grading)
- ✅ Permission check (0-10 levels)
- ✅ Dangerous operation confirmation prompt
- ✅ CreateProcess + pipe capture output
- ✅ 30-second timeout protection
- ✅ Exit code display

**Dangerous Command Grading**:

| Level | Command Examples |
|-------|-----------------|
| Level 3 | `rm -rf`, `rm -r`, `del /s /q` |
| Level 5 | `format`, `mkfs`, `fdisk`, `diskpart` |
| Level 7 | `sudo`, `runas`, `chmod 777` |
| Level 8 | `dd if=`, `rm -rf /` |
| Level 10 | `rm -rf /*`, `format c:`, `del /f /s /q c:\` |

### Local Models (GGUF)

Supports GGUF format local large model inference (framework implementation):

```
/model load <model_path>    # Load GGUF model
/model info                 # View model information
/model unload               # Unload model
```

Supported quantization formats: F32, F16, Q4_0, Q4_1, Q5_0, Q5_1, Q8_0

### OCR Recognition

Calls Windows built-in OCR API (framework implementation):

```
/ocr <image_path>           # Recognize text in image file
/ocr screen                 # Full screen recognition
/ocr region x y w h         # Recognize specified screen region
```

Supported formats: PNG, JPG, BMP, GIF, TIFF

### Plugin System

Supports official plugins and user-made plugins, DLL dynamic loading:

```
/plugin                     # Show plugin list
/plugin list                # List all plugins
/plugin info <id>           # View plugin details
/plugin enable <id>         # Enable plugin
/plugin disable <id>        # Disable plugin
/plugin run <id> [params]   # Execute plugin
/plugin install <path>      # Install plugin
/plugin uninstall <id>      # Uninstall plugin
/plugin stats               # Plugin statistics
```

**Plugin Categories**:
- **Official Plugins**: Developed by Ai Zhe Teng Lab official
- **User Plugins**: Developed by users themselves

**Plugin Statuses**:
- Disabled - Plugin installed but not enabled
- Enabled - Plugin enabled but not yet loaded
- Loaded - Plugin DLL loaded into memory
- Error - Plugin loading or execution error

### Local Database

Lightweight key-value storage, based on file system:

```
/db                         # Show database information
/db info                    # Database status information
/db get [table] <key>       # Get value (default settings table)
/db set [table] <key> <value>  # Set value
/db delete [table] <key>    # Delete key
/db clear [table]           # Clear table (default cache table)
```

**Available Tables**:
- `plugins` - Plugin table, stores plugin status
- `settings` - Settings table, stores user settings
- `cache` - Cache table, stores temporary data

Database location: `%APPDATA%\HackerTerminal\database\`

### GUI Operations

| Operation | Description |
|-----------|-------------|
| `click x y` | Click at specified position |
| `move x y` | Move mouse |
| `type <text>` | Input text |
| `hotkey <key>` | Press hotkey |
| `scroll <amount>` | Scroll mouse |
| `position` | Get mouse position |
| `screenshot <path>` | Save screenshot |
| `size` | Get screen size |
| `drag x1 y1 x2 y2` | Drag |

---

## 🔐 Permission Levels

| Level | Name | Description |
|-------|------|-------------|
| 0 | Read-only Mode | Can only read files and system information |
| 1 | View Mode | Can view and analyze |
| 2 | Document Mode | Can create and edit documents |
| 3 | File Management | Can manage files and folders |
| 4 | Development Mode | Can execute development commands |
| 5 | Standard Mode | Can execute most safe commands |
| 6 | Advanced Mode | Can execute system management commands |
| 7 | Administrator Mode | Can execute most system operations |
| 8 | Senior Admin | Can execute high-risk system operations |
| 9 | Almost Full Control | Can execute almost all operations |
| 10 | Full Control | Can execute any operation |

---

## 🎛️ Operation Modes

| Mode | Description |
|------|-------------|
| CLI Mode | Only use command line operations |
| GUI Mode | Only use simulated click operations |
| Hybrid Mode | Combine command line and simulated clicks |

---

## 📋 Installation Wizard

5-step interactive installation process:

1. **Welcome Screen** - Introduce product information
2. **Permission Level Setup** - Select initial permission level
3. **Operation Mode Selection** - Choose CLI/GUI/Hybrid mode
4. **AI Model Configuration** - Select AI backend and model
5. **Complete Installation** - Save configuration and start

Config file location: `%APPDATA%\HackerTerminal\config.ini`

---

## 🔌 Plugin Development

### Plugin Interface Specification

Plugin DLL must export the following 4 functions:

```c
// Get plugin information, return format: name|version|author|description
__declspec(dllexport) LPTSTR GetPluginInfo();

// Initialize plugin
__declspec(dllexport) BOOL InitPlugin();

// Execute plugin function
__declspec(dllexport) LPTSTR Execute(LPTSTR params);

// Shutdown plugin
__declspec(dllexport) BOOL ShutdownPlugin();
```

### Compiling Plugins

**MinGW-w64**:

```bash
gcc -shared -o plugin.dll plugin.c -luser32
```

**MSVC**:

```cmd
cl /LD plugin.c user32.lib
```

### Installing Plugins

```
/plugin install <DLL_file_path>
```

Or manually place `plugin.dll` into the `plugins/user/your_plugin_name/` directory.

See `plugins/README.md` for detailed development guide.

---

## 🛠️ Technical Implementation

### Tech Stack

- **Language**: Pure C (C99)
- **API**: Win32 API
- **Compiler**: MinGW-w64 (GCC)
- **Architecture**: x64 (64-bit)
- **Dependencies**: Zero third-party libraries, only system DLLs

### System DLL Dependencies

| DLL | Description |
|-----|-------------|
| `user32.dll` | GUI operations (mouse, keyboard, windows) |
| `gdi32.dll` | Graphics drawing (screenshots, bitmaps) |
| `shell32.dll` | Shell operations |
| `wininet.dll` | Network requests (HTTP/HTTPS) |
| `shlwapi.dll` | Path operations |
| `advapi32.dll` | Registry/configuration |

### Size Estimation

| Version | Size |
|---------|------|
| Pure feature version | ~300 KB |
| With installation wizard | ~600 KB (2 exes) |
| With local small model | ~50 MB+ (requires additional model download) |

---

## 🎯 Roadmap

### Completed ✅

- ✅ Complete shell command execution
- ✅ Local large model inference (GGUF) - framework
- ✅ Image recognition (Windows OCR) - framework
- ✅ More built-in skills
- ✅ Installation wizard program
- ✅ Plugin system (official/user categories)
- ✅ Local database

### In Development ⏳

- ⏳ Complete GGUF inference implementation
- ⏳ ARM64 architecture support
- ⏳ Multi-language support
- ⏳ More official plugins

### Future Plans 🔮

- [ ] Plugin market integration
- [ ] Skill store
- [ ] Theme system
- [ ] Script automation
- [ ] Remote control functionality

---

## ⚠️ Notes

1. **Safety First**: Use high permission levels with caution to avoid misoperations
2. **GUI Mode**: Do not operate mouse and keyboard while AI is running
3. **Emergency Termination**: Use hotkeys to terminate (GUI mode)
4. **Local Models**: Requires additional GGUF model file download
5. **OCR Feature**: Requires Windows 10 or above
6. **Compilation Environment**: Requires MinGW-w64 or Visual Studio
7. **Plugin Safety**: Only install plugins from trusted sources
8. **Database**: Database files are plain text, do not edit manually

---

## ❓ FAQ

### Q: What's the difference between Hacker Terminal and a regular terminal?

A: Hacker Terminal is not just a terminal, it's also an AI assistant. It has built-in AI capabilities that can understand natural language instructions and automatically execute complex tasks. It also has a skill system and plugin system for unlimited functionality expansion.

### Q: Why implement with pure Win32 API?

A: Pure Win32 API implementation means zero third-party dependencies, small size, fast startup, and good compatibility. No need to install .NET, Python, Java or other runtime environments, just download and use.

### Q: What's the use of permission levels?

A: Permission levels are the core of the security mechanism. By limiting AI's operation permissions, damage from misoperations or malicious instructions can be prevented. You can choose the appropriate permission level according to your use case.

### Q: How to develop my own plugins?

A: Refer to the development guide in `plugins/README.md`. You only need to implement 4 exported functions and compile into a DLL. Supports any language that can compile into DLL, such as C/C++.

### Q: Which AI models are supported?

A: Cloud supports all OpenAI compatible APIs. Local supports GGUF format large models (framework implemented, full functionality under development).

---

## 🤝 Contributing

Issues and Pull Requests are welcome!

### How to Contribute

1. Fork this repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Developing Skills

Refer to the example in `skills/demo-skill/` to develop your own skill packs.

### Developing Plugins

Refer to the development guide in `plugins/README.md` to develop your own plugins.

---

## 📄 License

MIT License

---

<div align="center">

**Made with ❤️ by Ai Zhe Teng Lab**

*Focused on creating interesting and practical tech projects*

</div>
