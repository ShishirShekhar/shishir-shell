#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdbool.h>

// Execute external command with optional stdout redirection target (out_file may be NULL)
// Returns: true on success, false on failure
bool sshell_execute_external(const char *command, char *argv[], const char *out_file);

#endif // EXECUTOR_H
