#include "parser.h"

#include <stdio.h>
#include <string.h>

// Skip whitespace and return pointer to next non-whitespace character
static char *skip_whitespace(char *p)
{
  while (*p == ' ' || *p == '\t') {
    p++;
  }
  return p;
}

// Parse a single token from input string
// Handles escape sequences (\) and quoted strings (' and ")
// Modifies input buffer in-place by inserting null terminators
// Returns 0 if unclosed quote is encountered
static int parse_single_token(char **p, char **out)
{
  char quote_char = '\0';

  while (**p != '\0') {
    // Handle escape sequences: \x becomes x
    if (**p == '\\' && *(*p + 1) != '\0') {
      **out = *(*p + 1);
      (*out)++;
      (*p) += 2;
      continue;
    }

    // Handle quote characters
    if (quote_char == '\0') {
      // Not currently in quotes
      if (**p == '\'' || **p == '"') {
        quote_char = **p;
        (*p)++;
        continue;
      }
      // End token on whitespace
      if (**p == ' ' || **p == '\t') {
        (*p)++; // Advance past whitespace before null terminating
        break;
      }
    } else {
      // Currently in quotes
      if (**p == quote_char) {
        quote_char = '\0';
        (*p)++;
        continue;
      }
    }

    // Regular character: copy to output
    **out = **p;
    (*out)++;
    (*p)++;
  }

  // Terminate token
  **out = '\0';

  // Check for unclosed quotes
  if (quote_char != '\0') {
    return 0; // Unclosed quote error
  }

  return 1; // Success
}

size_t sshell_parse_argv(char *input, char **argv, size_t max_args)
{
  if (input == NULL || argv == NULL || max_args < 2) {
    return 0;
  }

  size_t argc = 0;
  char *p = input;

  while (*p != '\0') {
    // Skip leading whitespace
    p = skip_whitespace(p);

    if (*p == '\0') {
      break;
    }

    if (argc >= max_args - 1) {
      break; // Buffer full
    }

    // Mark token start and parse it
    char *token_start = p;
    char *out = p;

    if (!parse_single_token(&p, &out)) {
      argv[0] = NULL;
      return 0; // Unclosed quote
    }

    argv[argc++] = token_start;

    // Skip trailing whitespace before next token
    p = skip_whitespace(p);
  }

  argv[argc] = NULL;
  return argc;
}
