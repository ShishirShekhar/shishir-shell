#include "command.h"

#include "builtin.h"
#include "constants.h"
#include "executor.h"
#include "parser.h"

#include <string.h>

bool process_command(const char *input)
{
  if (input == NULL || strlen(input) == 0) {
    return false;
  }

  // Make a mutable copy for parsing
  char input_copy[MAX_INPUT_BUFFER];
  strncpy(input_copy, input, MAX_INPUT_BUFFER - 1);
  input_copy[MAX_INPUT_BUFFER - 1] = '\0';

  char *argv[MAX_ARGV_COUNT];
  size_t argc = sshell_parse_argv(input_copy, argv, MAX_ARGV_COUNT);

  if (argc == 0) {
    return false;
  }

  char *command = argv[0];

  // Handle builtins via lookup table
  sshell_builtin_handler builtin = sshell_get_builtin(command);
  if (builtin != NULL) {
    builtin((int) argc, argv);
    return true;
  }

  // Handle external commands
  return sshell_execute_external(command, argv);
}
