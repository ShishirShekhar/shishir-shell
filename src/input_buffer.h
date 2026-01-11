#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H

#include "constants.h"

#include <stdint.h>

// Simple buffer for reading a single input line from stdin
typedef struct {
  char *input;
  uint8_t input_size;
} InputBuffer;

// Allocate and initialize an InputBuffer (caller owns memory)
InputBuffer Create_Input_Buffer();

// Read a line from stdin into the buffer
// Returns the length of the input (excluding newline)
uint8_t capture_input(InputBuffer *input_buffer);

// Free the memory allocated for the input buffer
void free_input_buffer(InputBuffer *input_buffer);

#endif // INPUT_BUFFER_H
