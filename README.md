# Shishir Shell (sshell)

A minimal yet functional shell implementation written in C, demonstrating core shell concepts and command parsing.

## Features

- **Built-in Commands**:
  - `exit` - Exit the shell
  - `echo <text>` - Print text to stdout
  - `type <cmd>` - Show command type (builtin or external)
  - `cd <dir>` - Change current directory
  - `pwd` - Print current working directory
  - `help` - Show all available commands
  - `version` - Display version and author info

- **Advanced Parsing**:
  - Single quote support - Preserve whitespace and special characters
  - Double quote support - Variable expansion with escape sequences
  - Escape sequences in unquoted and quoted strings

- **I/O Redirection**:
  - `>` or `1>` - Redirect stdout (truncate)
  - `>>` or `1>>` - Redirect stdout (append)
  - `2>` - Redirect stderr (truncate)
  - `2>>` - Redirect stderr (append)

- **Interactive Features**:
  - Tab completion for builtin commands
  - Tab completion for external commands in PATH
  - Command history with arrow keys (↑/↓)
  - Line editing (Ctrl+A, Ctrl+E, Ctrl+K, etc.)
  
- **External Command Execution** - Execute programs from PATH
- **Interactive Prompt** - Branded `sshell>` prompt with welcome banner

## Installation

### Option 1: Quick Install (One-liner)

**macOS/Linux** – Download, extract, and run in one command:

```bash
curl -fsSL https://raw.githubusercontent.com/ShishirShekhar/shishir-shell/main/install.sh | sh
```

**To install globally** (system-wide):

```bash
curl -fsSL https://raw.githubusercontent.com/ShishirShekhar/shishir-shell/main/install.sh | sh -s -- --global
```

Then run `sshell` from anywhere.

### Option 2: Download Pre-built Binary

Download the latest release from [GitHub Releases](../../releases):

```bash
# Visit releases and download the tar.gz for your platform
# Then extract and run:
tar -xzf sshell-*.tar.gz
./sshell
```

### Option 3: Build from Source

#### Prerequisites
- GCC compiler
- Make
- GNU Readline library
- C99 standard library

**Install dependencies:**

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install -y build-essential libreadline-dev
```

On macOS:
```bash
brew install readline
```

#### Compile

```bash
make
```

#### Run

```bash
make run
```

Or directly:

```bash
./sshell
```

### Option 4: Windows via WSL

Windows users: run sshell inside Windows Subsystem for Linux (WSL).

```bash
# In an elevated PowerShell (first-time setup)
wsl --install

# Then inside WSL (Ubuntu or other distro)
sudo apt update && sudo apt install -y build-essential
git clone https://github.com/ShishirShekhar/shishir-shell.git
cd shishir-shell
make
./sshell
```

## Usage

```bash
sshell> echo hello
hello

sshell> echo "hello world"
hello world

sshell> echo '  spaces  preserved  '
  spaces  preserved  

sshell> type echo
echo is a shell builtin

sshell> type ls
ls is /bin/ls

sshell> pwd
/Users/username/project

sshell> cd ..

sshell> echo test > output.txt

sshell> echo more >> output.txt

sshell> cat nonexistent 2> errors.txt

sshell> help
[displays all commands]

sshell> version
🐚 Shishir Shell v1.5.4

sshell> exit
```

**Tab Completion Examples:**
```bash
sshell> ec<TAB>       # completes to "echo"
sshell> e<TAB><TAB>   # shows "echo" and "exit"
sshell> g<TAB>        # shows "git", "gcc", "grep", etc.
```

## Project Structure

```
src/
├── main.c              # Entry point and main loop
├── input_buffer.c/h    # Input reading and buffering
├── command.c/h         # Command processing logic
├── parser.c/h          # Tokenization with quote handling
├── builtin.c/h         # Built-in command handlers
├── executor.c/h        # External command execution
├── path_utils.c/h      # PATH environment variable utilities
├── constants.h         # Configuration constants
└── version.h           # Version and author information
```

## Installation

Install globally to your system:

```bash
make install
```

Then you can run `sshell` from anywhere:

```bash
sshell
```

Uninstall with:

```bash
make uninstall
```

## Development

### Clean Build

```bash
make clean
make
```

### Code Style

- Follow standard C conventions
- Use meaningful variable names
- Add comments for complex logic
- Compile with `-Wall -Wextra -Werror`

## Future Enhancements

- [x] Single quote parsing
- [x] Double quote parsing with escape sequences
- [x] Directory navigation (cd, pwd)
- [x] I/O redirection (`>`, `>>`, `2>`, `2>>`)
- [x] Command history (arrow keys)
- [x] Tab completion for builtins and external commands
- [ ] Pipes (`|`)
- [ ] Input redirection (`<`)
- [ ] Background processes (`&`)
- [ ] Environment variables (`$VAR`)
- [ ] Aliases
- [ ] Job control

## License

MIT License - See [LICENSE](LICENSE) file for details

## Author

Shishir Shekhar

## Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest improvements
- Submit pull requests

---

**Status**: In Development ✓
