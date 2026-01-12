#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdbool.h>

// Execute external command with optional stdout/stderr redirection targets (may
// be NULL) Returns: true on success, false on failure
bool sshell_execute_external(const char *command, char *argv[],
                             const char *out_file, const char *err_file,
                             bool out_append, bool err_append);

#endif // EXECUTOR_H
