#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

// Process a single input line: handles builtins and external commands
// Returns true if the command is valid and successfully executed
bool process_command(const char *input);

#endif // COMMAND_H
