#ifndef shell_h
#define shell_h

#include <string.h>
#include <stdlib.h>
#include "process1.h"

void shell(int schedulerChoice) { //Default scheduler is SJF
    if (schedulerChoice == 1) {
        std::cout<< "Initiating background processes.\n";
        SJFProcesses();
    }
    else if (schedulerChoice == 2) {
        std::cout << "Initiating background processes.\n";
        FCFSProcesses();
    }
    while (1) {
        printf("JayOS> ");
        char command[50];
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        //Check command contents
        printf("DEBUG: Command entered: %s\n", command);

        if (strcmp(command, "exit") == 0) {
            printf("Shutting down JaySH...\n");
            //sleep(1);
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
            printf("4. SJF - Shortest Job First Scheduling Demo\n");
            printf("5. FCFS - First Come First Serve Scheduling Demo\n");
        }

        else if (strcmp(command, "SJF") == 0) {
            SJFProcesses();
        }

        else if (strcmp(command, "FCFS") == 0) {
            FCFSProcesses();
        }
        else {
            printf("Command not found. Type 'help' to see available commands.\n");
        }
    }
}

#endif // shell_h