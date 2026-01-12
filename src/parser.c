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

// Parse unquoted token: copy chars until whitespace, handle escapes
// Advances both p and out
static void parse_unquoted(char **p, char **out)
{
  while (**p != '\0' && **p != ' ' && **p != '\t' && **p != '\'' &&
         **p != '"') {
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

// Parse a single token from input string
// Handles different quote types and escape sequences
// Modifies input buffer in-place by inserting null terminators
// Returns 0 if unclosed quote is encountered
static int parse_single_token(char **p, char **out)
{
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

    // Skip trailing whitespace before next token
    p = skip_whitespace(p);
  }

  argv[argc] = NULL;
  return argc;
}
