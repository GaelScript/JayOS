#ifndef shell_h
#define shell_h

#include <string.h>
#include <stdlib.h>
#include "process1.h"


struct ProcessControlBlock ShellPCB = {1, 64};


void shell() {
    while (1) {
        printf("JayOS> ");
        char command[50];
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        //Check command contents
        printf("DEBUG: Command entered: %s\n", command);

        if (strcmp(command, "exit") == 0) {
            printf("Shutting down JaySH...\n");
            sleep(1);
            break; // Exit the loop
        }

        else if (strcmp(command, "IO") == 0) {
            inputOutputProcess(); //For future reference: this essentialy creates a child process, and the parent process (shell) waits for the child process to finish
        }

        else if (strcmp(command, "help") == 0) {
            printf("Commands available:\n");
            printf("1. IO - Input/Output process\n");
            printf("2. help - Display available commands\n");
            printf("3. exit - Exit JayOS\n");
        }

        else {
            printf("Command not found. Type 'help' to see available commands.\n");
        }
    }
}

#endif // shell_h