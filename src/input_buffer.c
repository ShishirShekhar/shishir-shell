#include "input_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InputBuffer Create_Input_Buffer()
{
  InputBuffer input_buffer;

  input_buffer.input = calloc(MAX_INPUT_BUFFER, sizeof(char));
  input_buffer.input_size = 0;

  return input_buffer;
}

uint8_t capture_input(InputBuffer *input_buffer)
{
  // Read a line from stdin (up to MAX_INPUT_BUFFER - 1 chars)
  fgets(input_buffer->input, MAX_INPUT_BUFFER, stdin);
  // Strip trailing newline added by fgets (if present)
  input_buffer->input_size = strlen(input_buffer->input) - 1;
  // Replace newline with string terminator; for "\n"-only lines this sets index
  // 0
  input_buffer->input[input_buffer->input_size] = '\0';

  return input_buffer->input_size;
}

void free_input_buffer(InputBuffer *input_buffer)
{
  free(input_buffer->input);
  input_buffer->input = NULL;
  input_buffer->input_size = 0;
}
