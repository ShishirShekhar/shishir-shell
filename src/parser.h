#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>

// Parse input into argv tokens, handling single quotes.
// - Modifies the input buffer in place (inserts '\0').
// - Strips single quotes from tokens.
// - Returns argc and ensures argv[argc] == NULL.
size_t sshell_parse_argv(char *input, char **argv, size_t max_args);

#endif // PARSER_H
