#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>
#include <stddef.h>

// Signature for builtin handlers
typedef void (*sshell_builtin_handler)(int argc, char *argv[]);

void sshell_exit(int argc, char *argv[]);
void sshell_echo(int argc, char *argv[]);
void sshell_type(int argc, char *argv[]);
void sshell_cd(int argc, char *argv[]);
void sshell_pwd(int argc, char *argv[]);
void sshell_help(int argc, char *argv[]);
void sshell_version(int argc, char *argv[]);

// Lookup builtin by name; returns NULL if not found
sshell_builtin_handler sshell_get_builtin(const char *command);
bool sshell_is_builtin(const char *command);

// Get builtin count and names for iteration (e.g., tab completion)
size_t sshell_get_builtin_count();
const char *sshell_get_builtin_name(size_t index);

#endif // BUILTIN_H