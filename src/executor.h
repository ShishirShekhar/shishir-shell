#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdbool.h>

// Execute external command with optional stdout/stderr redirection targets (may
// be NULL) Returns: true on success, false on failure
bool sshell_execute_external(const char *command, char *argv[],
                             const char *out_file, const char *err_file);

#endif // EXECUTOR_H
