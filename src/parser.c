#include "parser.h"

#include <stdio.h>
#include <string.h>

size_t sshell_parse_argv(char *input, char **argv, size_t max_args)
{
  if (input == NULL || argv == NULL || max_args < 2)
  {
    return 0;
  }

  size_t argc = 0;
  char *p = input;
  bool in_single = false;

  while (*p != '\0')
  {
    // Skip leading spaces
    while (*p == ' ')
    {
      p++;
    }
    if (*p == '\0')
    {
      break;
    }

    if (argc >= max_args - 1)
    {
      break;
    }

    // Start of a new token
    argv[argc++] = p;

    char *out = p;
    while (*p != '\0')
    {
      if (!in_single && *p == '\'')
      {
        in_single = true;
        p++;
        continue;
      }
      if (in_single && *p == '\'')
      {
        in_single = false;
        p++;
        continue;
      }
      if (!in_single && *p == ' ')
      {
        p++; // move past delimiter
        break;
      }

      *out++ = *p++;
    }

    // Terminate this token
    *out = '\0';

    // Skip spaces before next token
    while (*p == ' ')
    {
      p++;
    }
  }

  if (in_single)
  {
    argv[0] = NULL;
    return 0; // Unmatched single quote
  }

  argv[argc] = NULL;
  return argc;
}
