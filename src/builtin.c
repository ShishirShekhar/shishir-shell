#include "builtin.h"

#include "constants.h"
#include "path_utils.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Builtin handlers
void sshell_exit(int argc __attribute__((unused)),
                 char *argv[] __attribute__((unused)))
{
  exit(EXIT_SUCCESS);
}

void sshell_echo(int argc, char *argv[])
{
  // Print all args after the command, separated by single spaces
  for (int i = 1; i < argc; i++)
  {
    fputs(argv[i], stdout);
    if (i + 1 < argc)
    {
      fputc(' ', stdout);
    }
  }
  fputc('\n', stdout);
}

void sshell_type(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("type: missing operand\n");
    return;
  }

  char *arg = argv[1];

  if (sshell_is_builtin(arg)) {
    printf("%s is a shell builtin\n", arg);
    return;
  }

  char resolved_path[MAX_PATH_BUFFER];

  // Check if the command exists somewhere on PATH
  if (sshell_find_in_path(arg, resolved_path, sizeof(resolved_path))) {
    printf("%s is %s\n", arg, resolved_path);
    return;
  }

  printf("%s: not found\n", arg);
}

void sshell_cd(int argc, char *argv[])
{
  const char *arg = (argc < 2) ? NULL : argv[1];

  // If no argument is provided, change to home directory
  if (arg == NULL || strlen(arg) == 0 || strcmp(arg, "~") == 0) {
    const char *home = getenv("HOME");
    if (home == NULL) {
      printf("cd: HOME not set\n");
      return;
    }
    arg = home;
  }

  // Attempt to change directory
  if (chdir(arg) != 0) {
    printf("cd: %s: No such file or directory\n", arg);
  }
}

void sshell_pwd(int argc __attribute__((unused)),
                char *argv[] __attribute__((unused)))
{
  char cwd[MAX_PATH_BUFFER];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
  } else {
    perror("getcwd() error");
  }
}

void sshell_help(int argc __attribute__((unused)),
                 char *argv[] __attribute__((unused)))
{
  printf("\nShishir Shell (sshell) - Built-in Commands:\n");
  printf("  exit          Exit the shell\n");
  printf("  echo <text>   Print text to stdout\n");
  printf("  type <cmd>    Show command type (builtin or path)\n");
  printf("  cd <dir>      Change current directory to <dir>\n");
  printf("  pwd           Print current working directory\n");
  printf("  help          Show this help message\n");
  printf("  version       Show version information\n");
  printf("\nExternal commands are executed from PATH.\n\n");
}

void sshell_version(int argc __attribute__((unused)),
                    char *argv[] __attribute__((unused)))
{
  printf("\n");
  printf("  🐚 %s v%s\n", SSHELL_NAME, SSHELL_VERSION);
  printf("  Built with ❤️  by %s\n", SSHELL_AUTHOR);
  printf("  GitHub: %s\n", SSHELL_GITHUB);
  printf("\n");
}

typedef struct {
  const char *name;
  sshell_builtin_handler handler;
} BuiltinEntry;

// Simple builtin registry; add new builtins here
static const BuiltinEntry BUILTINS[] = {
  {"exit", sshell_exit},       {"echo", sshell_echo}, {"type", sshell_type},
  {"cd", sshell_cd},           {"pwd", sshell_pwd},   {"help", sshell_help},
  {"version", sshell_version},
};

sshell_builtin_handler sshell_get_builtin(const char *command)
{
  if (command == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < sizeof(BUILTINS) / sizeof(BUILTINS[0]); i++) {
    if (strcmp(command, BUILTINS[i].name) == 0) {
      return BUILTINS[i].handler;
    }
  }
  return NULL;
}

bool sshell_is_builtin(const char *command)
{
  return sshell_get_builtin(command) != NULL;
}
