# Shishir Shell - Makefile
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./src -std=c99
LDFLAGS = 

# Source files
SRC = src/main.c src/input_buffer.c src/command.c src/builtin.c src/path_utils.c src/parser.c src/executor.c
OBJ = $(SRC:.c=.o)

# Output binary
TARGET = sshell

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	@echo "✓ Build successful: $(TARGET)"

# Compile individual source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
	@echo "✓ Cleaned build artifacts"

# Install to /usr/local/bin
install: $(TARGET)
	mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/
	chmod +x /usr/local/bin/$(TARGET)
	@echo "✓ Installed $(TARGET) to /usr/local/bin/"

# Uninstall from /usr/local/bin
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "✓ Uninstalled $(TARGET)"

# Phony targets (not actual files)
.PHONY: all run clean install uninstall
