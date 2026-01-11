#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <stdbool.h>
#include <stddef.h>

// Look up a command in PATH; writes the resolved path into out_path when found
bool sshell_find_in_path(const char *command, char *out_path,
                         size_t out_path_size);

#endif // PATH_UTILS_H
