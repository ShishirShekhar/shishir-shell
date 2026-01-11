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

- **Single Quote Support** - Preserve whitespace and special characters
- **External Command Execution** - Execute programs from PATH
- **Smart Command Parsing** - Parse and tokenize user input
- **Interactive Prompt** - Branded `sshell>` prompt with welcome banner

## Installation

### Option 1: Download Pre-built Binary

Download the latest release from [GitHub Releases](../../releases):

```bash
# Download for your platform (macOS/Linux)
wget https://github.com/ShishirShekhar/shishir-shell/releases/download/v1.0.1/sshell
chmod +x sshell

# macOS users: Remove quarantine flag
xattr -d com.apple.quarantine sshell 2>/dev/null || true

./sshell
```

**macOS Security Note:** If you see "cannot be opened because the developer cannot be verified":
- Right-click the `sshell` file → "Open" → Click "Open" in the dialog
- Or run: `xattr -d com.apple.quarantine sshell`
- Or build from source (recommended for security)

**If output looks like gibberish:** You are likely printing the binary instead of executing it. Run it directly with `./sshell` (or `sshell` if installed), and do not pipe/redirect the binary into the shell.

**If it opens in a text editor:** Finder treats it like a document when double-clicked. Instead, run it from Terminal:

```bash
chmod +x sshell   # once, if needed
./sshell          # run from the folder where you downloaded it
```

### Option 2: Build from Source

#### Prerequisites
- GCC compiler
- Make
- C99 standard library

#### Compile

```bash
make
```

### Run

```bash
make run
```

Or directly:

```bash
./sshell
```

### Option 3: Windows via WSL

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

sshell> echo '  spaces  preserved  '
  spaces  preserved  

sshell> type echo
echo is a shell builtin

sshell> type ls
ls is /bin/ls

sshell> pwd
/Users/username/project

sshell> cd ..

sshell> help
[displays all commands]

sshell> version
🐚 Shishir Shell v1.0.1

sshell> exit
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
- [x] Directory navigation (cd, pwd)
- [ ] Double quote parsing with variable expansion
- [ ] Pipes and redirection (`|`, `>`, `<`)
- [ ] Background processes (`&`)
- [ ] Command history (arrow keys)
- [ ] Tab completion
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
