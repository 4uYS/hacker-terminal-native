# Hacker Terminal v2.1 (Native Win32 x64)

> AI Agent Lab (AAL) / 爱折腾AI实验室

An AI assistant built for terminal tinkerers, implemented with pure native Win32 API with zero third-party dependencies.

[中文](README.md) | English

---

## ✨ Features

- 🚀 **Zero Dependencies**: Pure Win32 API implementation, no third-party libraries
- 💻 **Ultra Lightweight**: Single file exe, only ~250KB
- 🎨 **Hacker Style**: Terminal color output, cool interface
- 🔒 **Permission Levels**: 0-10 level permission control, safe and controllable
- 🖱️ **GUI Simulation**: Supports mouse click, keyboard input, screenshot, etc.
- 📦 **Skill System**: Extensible skill plugin mechanism with 5 built-in practical skills
- 💾 **Context Management**: Memory storage, supports JSON import/export
- ⚙️ **Interactive Settings**: Terminal settings panel, arrow key operation
- 🌐 **Cloud AI**: Supports OpenAI compatible API
- 🧠 **Local Inference**: GGUF format local LLM support (framework)
- 👁️ **OCR Recognition**: Windows built-in OCR support (framework)
- 💻 **Full Shell**: Dangerous command detection, permission check, pipe capture output
- 📋 **Installation Wizard**: 5-step interactive installation process
- 🔌 **Plugin System**: Supports official plugins and user-made plugins, DLL dynamic loading
- 🗄️ **Local Database**: Lightweight key-value storage with 3 data tables

---

## 📦 File Structure

```
hacker-terminal-native/
├── src/                    # Source code
│   ├── hacker.h           # Main header
│   ├── main.c             # Main program entry
│   ├── utils.c            # Utility functions
│   ├── terminal.c         # Terminal interface
│   ├── config.c           # Configuration management
│   ├── gui.c              # GUI control
│   ├── network.c          # Network requests
│   ├── skills.c           # Skill system
│   ├── context.c          # Context management
│   ├── ai.c               # AI engine
│   ├── commands.c         # Command system
│   ├── settings.c         # Settings panel
│   ├── shell.c            # Shell command execution engine
│   ├── ocr.c              # OCR recognition module
│   ├── ggml.c             # GGUF local inference engine
│   ├── database.c         # Local database module
│   ├── plugin.c           # Plugin manager
│   └── installer.c        # Installation wizard
├── resources/              # Resource files
│   └── version.rc         # Version resource
├── skills/                 # Skills directory
│   ├── demo-skill/        # Demo skill
│   ├── file-manager/      # File management skill
│   ├── text-processor/    # Text processing skill
│   ├── system-monitor/    # System monitoring skill
│   ├── network-tools/     # Network tools skill
│   └── doc-editor/        # Document editor skill
├── plugins/                # Plugins directory
│   ├── official/          # Official plugins
│   │   └── hello-world/   # Hello World example plugin
│   ├── user/              # User plugins
│   │   └── template/      # User plugin template
│   └── README.md          # Plugin development guide
├── build.bat              # Build script
└── README.md              # Documentation
```

---

## 🚀 Quick Start

### Compilation

1. Install MinGW-w64 (x86_64-posix-seh)
2. Run the build script:

```cmd
build.bat
```

3. After successful compilation, executables are in `bin/` directory:
   - `HackerTerminal.exe` - Main program
   - `HackerInstaller.exe` - Installation wizard

### Running

**First run (recommended)**:

```cmd
HackerInstaller.exe
```

Run the installation wizard, complete initial configuration and automatically start the main program.

**Direct run**:

```cmd
HackerTerminal.exe
```

---

## 🎯 Usage

### Basic Commands

- `/help` - Show help information
- `/settings` - Open settings panel
- `/clear` - Clear screen
- `/exit` - Exit program
- `/save` - Save context
- `/load` - Load context
- `/skill` - Skill management
- `/plugin` - Plugin management

### Skill System

Built-in skills:

1. **File Manager** - File operations, directory browsing
2. **Text Processor** - Text processing, format conversion
3. **System Monitor** - System status monitoring
4. **Network Tools** - Network diagnosis, testing tools
5. **Document Editor** - Document creation and editing

### Plugin System

- Supports official plugins and user-made plugins
- DLL dynamic loading
- Plugin market integration

---

## 📊 Size Estimation

| Version | Size |
|---------|------|
| Pure feature version | ~300 KB |
| With installer | ~600 KB (2 exe) |
| With local small model | ~50 MB+ (additional model download required) |

---

## 🗺️ Roadmap

- ✅ Complete shell command execution
- ✅ Local LLM inference (GGUF) - framework
- ✅ Image recognition (Windows OCR) - framework
- ✅ More built-in skills
- ✅ Installation wizard
- ✅ Plugin system (official/user categories)
- ✅ Local database
- ⏳ Complete GGUF inference implementation
- ⏳ ARM64 architecture support
- ⏳ Multi-language support
- ⏳ More official plugins

---

## ⚠️ Notes

1. **Safety First**: Use high permission levels with caution to avoid misoperations
2. **GUI Mode**: Do not operate mouse and keyboard while AI is running
3. **Emergency Termination**: Use shortcut keys to terminate (GUI mode)
4. **Local Models**: Additional GGUF model files need to be downloaded
5. **OCR Feature**: Requires Windows 10 or above
6. **Build Environment**: Requires MinGW-w64 or Visual Studio
7. **Plugin Safety**: Only install plugins from trusted sources
8. **Database**: Database files are plain text, do not edit manually

---

## 🤝 Contributing

Welcome to contribute code, report issues, or suggest improvements!

### How to Contribute

1. **Submit Issues**: Report bugs, suggest new features
2. **Submit Pull Requests**: Fix bugs, implement new features
3. **Develop Skills**: Create new skills and share them
4. **Develop Plugins**: Create plugins and submit to the market
5. **Improve Documentation**: Improve docs, add examples

### Development Environment

- MinGW-w64 or Visual Studio
- Git
- Windows 10+ (for testing)

---

## 📄 License

MIT License

---

## 👥 About

**AI Agent Lab (AAL) / 爱折腾AI实验室**

We are a group of developers who love technology and enjoy tinkering, dedicated to creating interesting and practical tools and projects with AI technology.

- 🌐 Explore cutting-edge technology
- 🛠️ Build practical tools
- 📚 Share technical experience

---

**Made with ❤️ by AI Agent Lab (AAL) / 爱折腾AI实验室**
