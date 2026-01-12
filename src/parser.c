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
  char quote_char = '\0'; // Track which quote type we're in (or '\0' if none)

  while (*p != '\0')
  {
    // Skip leading whitespace
    while (*p == ' ' || *p == '\t')
    {
      p++;
    }

    if (*p == '\0')
    {
      break;
    }

    if (argc >= max_args - 1)
    {
      break; // Buffer full
    }

    // Start of a new token
    char *token_start = p;
    char *out = p;

    // Parse token, handling quotes and escape sequences
    while (*p != '\0')
    {
      // Handle escape sequences
      if (*p == '\\' && *(p + 1) != '\0')
      {
        // Copy the escaped character literally
        *out++ = *(p + 1);
        p += 2;
        continue;
      }

      // Handle quote characters
      if (quote_char == '\0')
      {
        // Not currently in quotes
        if (*p == '\'' || *p == '"')
        {
          quote_char = *p;
          p++;
          continue;
        }
        // End token on whitespace
        if (*p == ' ' || *p == '\t')
        {
          p++; // Advance past whitespace before null terminating
          break;
        }
      }
      else
      {
        // Currently in quotes
        if (*p == quote_char)
        {
          quote_char = '\0';
          p++;
          continue;
        }
      }

      // Regular character: copy to output
      *out++ = *p++;
    }

    // Terminate token
    *out = '\0';

    // Check for unclosed quotes
    if (quote_char != '\0')
    {
      argv[0] = NULL;
      return 0; // Unclosed quote
    }

    argv[argc++] = token_start;

    // Skip trailing whitespace
    while (*p == ' ' || *p == '\t')
    {
      p++;
    }
  }

  argv[argc] = NULL;
  return argc;
}
