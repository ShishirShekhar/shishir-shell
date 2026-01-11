#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdbool.h>

// Execute external command
// Returns: true on success, false on failure
bool sshell_execute_external(const char *command, char *argv[]);

#endif // EXECUTOR_H
