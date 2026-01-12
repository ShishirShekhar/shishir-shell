#include "command.h"

#include "builtin.h"
#include "constants.h"
#include "executor.h"
#include "parser.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Extract stdout and stderr redirection from argv (supports '>', '>>', '1>',
// '1>>', '2>', '2>>'). Removes the redirection tokens from argv. Returns false
// on syntax error.
static bool extract_redirections(char *argv[], char **out_file, char **err_file,
                                 bool *out_append, bool *err_append)
{
  int w = 0;
  for (int r = 0; argv[r] != NULL; r++) {
    if (strcmp(argv[r], "2>>") == 0) {
      if (argv[r + 1] == NULL) {
        return false; // missing target
      }
      *err_file = argv[r + 1];
      *err_append = true;
      r++; // skip target
      continue;
    } else if (strcmp(argv[r], "2>") == 0) {
      if (argv[r + 1] == NULL) {
        return false; // missing target
      }
      *err_file = argv[r + 1];
      *err_append = false;
      r++; // skip target
      continue;
    } else if (strcmp(argv[r], ">>") == 0 || strcmp(argv[r], "1>>") == 0) {
      if (argv[r + 1] == NULL) {
        return false; // missing target
      }
      *out_file = argv[r + 1];
      *out_append = true;
      r++; // skip target
      continue;
    } else if (strcmp(argv[r], ">") == 0 || strcmp(argv[r], "1>") == 0) {
      if (argv[r + 1] == NULL) {
        return false; // missing target
      }
      *out_file = argv[r + 1];
      *out_append = false;
      r++; // skip target
      continue;
    }
    argv[w++] = argv[r];
  }
  argv[w] = NULL;
  return true;
}

// Run a builtin with optional stdout/stderr redirection; returns success
static bool run_builtin_with_redirect(sshell_builtin_handler builtin, int argc,
                                      char *argv[], char *out_file,
                                      char *err_file, bool out_append,
                                      bool err_append)
{
  int saved_stdout = -1;
  int saved_stderr = -1;
  int fd_out = -1;
  int fd_err = -1;

  if (out_file != NULL) {
    int flags = O_CREAT | O_WRONLY | (out_append ? O_APPEND : O_TRUNC);
    fd_out = open(out_file, flags, 0644);
    if (fd_out < 0) {
      perror("open failed");
      return false;
    }
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0) {
      perror("dup failed");
      close(fd_out);
      return false;
    }
    if (dup2(fd_out, STDOUT_FILENO) < 0) {
      perror("dup2 failed");
      close(fd_out);
      close(saved_stdout);
      return false;
    }
    close(fd_out);
  }

  if (err_file != NULL) {
    int flags = O_CREAT | O_WRONLY | (err_append ? O_APPEND : O_TRUNC);
    fd_err = open(err_file, flags, 0644);
    if (fd_err < 0) {
      perror("open failed");
      if (saved_stdout >= 0) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
      }
      return false;
    }
    saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr < 0) {
      perror("dup failed");
      close(fd_err);
      if (saved_stdout >= 0) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
      }
      return false;
    }
    if (dup2(fd_err, STDERR_FILENO) < 0) {
      perror("dup2 failed");
      close(fd_err);
      close(saved_stderr);
      if (saved_stdout >= 0) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
      }
      return false;
    }
    close(fd_err);
  }

  builtin(argc, argv);

  if (saved_stdout >= 0) {
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
  }

  if (saved_stderr >= 0) {
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);
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
  char *err_file = NULL;
  bool out_append = false;
  bool err_append = false;
  if (!extract_redirections(argv, &out_file, &err_file, &out_append,
                            &err_append)) {
    fprintf(stderr, "redirection syntax error\n");
    return false;
  }

  // Recompute argc after removing redirection tokens
  size_t new_argc = 0;
  while (argv[new_argc] != NULL) {
    new_argc++;
  }

  if (builtin != NULL) {
    return run_builtin_with_redirect(builtin, (int) new_argc, argv, out_file,
                                     err_file, out_append, err_append);
  }

  // Handle external commands
  return sshell_execute_external(command, argv, out_file, err_file, out_append,
                                 err_append);
}
