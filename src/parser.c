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

// Parse content within single quotes: everything literal, no escapes
// Advances both p and out, returns success/failure
static int parse_single_quote(char **p, char **out)
{
  (*p)++; // Skip opening quote

  while (**p != '\0' && **p != '\'') {
    **out = **p;
    (*out)++;
    (*p)++;
  }

  if (**p != '\'') {
    return 0; // Unclosed single quote
  }

  (*p)++; // Skip closing quote
  return 1;
}

// Parse content within double quotes: literal except for backslash escapes
// Advances both p and out, returns success/failure
static int parse_double_quote(char **p, char **out)
{
  (*p)++; // Skip opening quote

  while (**p != '\0' && **p != '"') {
    if (**p == '\\' && *(*p + 1) != '\0') {
      char next_char = *(*p + 1);
      if (next_char == '"' || next_char == '\\' || next_char == '$' ||
          next_char == '`' || next_char == '\n') {
        **out = next_char;
        (*out)++;
        (*p) += 2;
        continue;
      }
    }
    **out = **p;
    (*out)++;
    (*p)++;
  }

  if (**p != '"') {
    return 0; // Unclosed double quote
  }

  (*p)++; // Skip closing quote
  return 1;
}

// Parse unquoted token: copy chars until whitespace or quote, handle escapes
// Advances both p and out
static void parse_unquoted(char **p, char **out)
{
  while (**p != '\0' && **p != ' ' && **p != '\t' && **p != '\'' &&
         **p != '"') {
    // Stop before any redirection operator
    if (**p == '>') {
      return;
    }
    if ((**p == '1' || **p == '2') && *(*p + 1) == '>') {
      return;
    }

    if (**p == '\\' && *(*p + 1) != '\0') {
      **out = *(*p + 1);
      (*out)++;
      (*p) += 2;
    } else {
      **out = **p;
      (*out)++;
      (*p)++;
    }
  }
}

// Try to parse a redirection operator as a token
// Returns the number of characters in the operator (0 if not a redirection)
static int try_parse_redirection(char **p, char **out, char *token_start)
{
  // Check for 1>> or 2>>
  if ((**p == '1' || **p == '2') && *(*p + 1) == '>' && *(*p + 2) == '>') {
    if (*out == token_start) {
      **out = **p;
      (*out)++;
      **out = '>';
      (*out)++;
      **out = '>';
      (*out)++;
      (*p) += 3;
      return 3;
    }
    return -1; // Signal to break but don't consume
  }

  // Check for 1> or 2>
  if ((**p == '1' || **p == '2') && *(*p + 1) == '>') {
    if (*out == token_start) {
      **out = **p;
      (*out)++;
      **out = '>';
      (*out)++;
      (*p) += 2;
      return 2;
    }
    return -1;
  }

  // Check for >>
  if (**p == '>' && *(*p + 1) == '>') {
    if (*out == token_start) {
      **out = '>';
      (*out)++;
      **out = '>';
      (*out)++;
      (*p) += 2;
      return 2;
    }
    return -1;
  }

  // Check for >
  if (**p == '>') {
    if (*out == token_start) {
      **out = '>';
      (*out)++;
      (*p)++;
      return 1;
    }
    return -1;
  }

  return 0; // Not a redirection operator
}

// Parse a single token from input string
// Handles different quote types and escape sequences
// Modifies input buffer in-place by inserting null terminators
// Returns 0 if unclosed quote is encountered
static int parse_single_token(char **p, char **out)
{
  char *token_start = *out;

  while (**p != '\0') {
    // Handle single quotes
    if (**p == '\'') {
      if (!parse_single_quote(p, out)) {
        return 0; // Unclosed quote
      }
      continue;
    }

    // Handle double quotes
    if (**p == '"') {
      if (!parse_double_quote(p, out)) {
        return 0; // Unclosed quote
      }
      continue;
    }

    // Handle redirection operators as separate tokens
    int redir_result = try_parse_redirection(p, out, token_start);
    if (redir_result != 0) {
      break; // Either parsed operator or need to break for next token
    }

    // Handle whitespace - end of token
    if (**p == ' ' || **p == '\t') {
      (*p)++; // Advance past whitespace
      break;
    }

    // Parse unquoted content
    parse_unquoted(p, out);
  }

  // Terminate token
  **out = '\0';
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

    // Move past any internal terminators we just wrote (but stop at final NUL)
    while (*p == '\0' && *(p + 1) != '\0') {
      p++;
    }
    p = skip_whitespace(p);
  }

  argv[argc] = NULL;
  return argc;
}
