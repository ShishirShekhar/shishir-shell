#include "command.h"

#include "builtin.h"
#include "constants.h"
#include "executor.h"
#include "parser.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Extract stdout redirection from argv (supports '>' and '1>').
// Removes the redirection tokens from argv. Returns false on syntax error.
static bool extract_stdout_redirection(char *argv[], char **out_file)
{
  int w = 0;
  for (int r = 0; argv[r] != NULL; r++) {
    if ((strcmp(argv[r], ">") == 0 || strcmp(argv[r], "1>") == 0)) {
      if (argv[r + 1] == NULL) {
        return false; // missing target
      }
      *out_file = argv[r + 1];
      r++; // skip target
      continue;
    }
    argv[w++] = argv[r];
  }
  argv[w] = NULL;
  return true;
}

// Run a builtin with optional stdout redirection; returns success
static bool run_builtin_with_redirect(sshell_builtin_handler builtin, int argc,
                                      char *argv[], char *out_file)
{
  int saved_stdout = -1;
  int fd = -1;

  if (out_file != NULL) {
    fd = open(out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
      perror("open failed");
      return false;
    }
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0) {
      perror("dup failed");
      close(fd);
      return false;
    }
    if (dup2(fd, STDOUT_FILENO) < 0) {
      perror("dup2 failed");
      close(fd);
      close(saved_stdout);
      return false;
    }
    close(fd);
  }

  builtin(argc, argv);

  if (saved_stdout >= 0) {
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
  }

  return true;
}

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

  char *out_file = NULL;
  if (!extract_stdout_redirection(argv, &out_file)) {
    fprintf(stderr, "redirection syntax error\n");
    return false;
  }

  // Recompute argc after removing redirection tokens
  size_t new_argc = 0;
  while (argv[new_argc] != NULL) {
    new_argc++;
  }

  if (builtin != NULL) {
    return run_builtin_with_redirect(builtin, (int) new_argc, argv, out_file);
  }

  // Handle external commands
  return sshell_execute_external(command, argv, out_file);
}
