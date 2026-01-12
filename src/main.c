#include "command.h"
#include "input_buffer.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Shishir Shell - A minimal yet functional shell implementation
 * Built with passion by Shishir Shekhar
 */

static void print_welcome_banner()
{
  printf("\n");
  printf("  ╔═══════════════════════════════════════╗\n");
  printf("  ║   🐚  %s v%s            ║\n", SSHELL_NAME, SSHELL_VERSION);
  printf("  ╠═══════════════════════════════════════╣\n");
  printf("  ║  A modern shell by Shishir Shekhar    ║\n");
  printf("  ║  Type 'help' for commands             ║\n");
  printf("  ╚═══════════════════════════════════════╝\n");
  printf("\n");
}

int main()
{
  // Flush after every printf
  setbuf(stdout, NULL);

  // Show welcome banner
  print_welcome_banner();

  InputBuffer input_buffer = Create_Input_Buffer();

  // Infinite loop to continuously prompt for user commands
  // Empty input (size == 0) breaks the loop (Ctrl+D)
  while (capture_input(&input_buffer)) {
    if (input_buffer.input_size > 0) {
      if (!process_command(input_buffer.input)) {
        printf("%s: command not found\n", input_buffer.input);
      }
    }
  }

  printf("\n");
  free_input_buffer(&input_buffer);
  return EXIT_SUCCESS;
}
