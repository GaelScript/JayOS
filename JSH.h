#ifndef shell_h
#define shell_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process1.h"
#include <time.h> // Needed for random
#include <unistd.h> // Needed for sleep


struct ProcessControlBlock ShellPCB = {1, 64};

typedef enum { READY, RUNNING, WAITING, FINISHED } State;

typedef struct {
    int id;
    int burstTime;
    int remainingTime;
    int arrivalTime;
    int ioTime;
    State state;
} Process;

#define PROCESSES 5

Process processes[PROCESSES];
int num_processes = 0; // Total number of processes

void reset_processes() {
    processes[0] = (Process){ .id = 1, .burstTime = 8, .remainingTime = 8, .arrivalTime = 0, .ioTime = 4, .state = READY };
    processes[1] = (Process){ .id = 2, .burstTime = 4, .remainingTime = 4, .arrivalTime = 2, .ioTime = -1, .state = READY };
    processes[2] = (Process){ .id = 3, .burstTime = 9, .remainingTime = 9, .arrivalTime = 4, .ioTime = 5, .state = READY };
    processes[3] = (Process){ .id = 4, .burstTime = 5, .remainingTime = 7, .arrivalTime = 1, .ioTime = 1, .state = READY };
    processes[4] = (Process){ .id = 5, .burstTime = 6, .remainingTime = 6, .arrivalTime = 5, .ioTime = 3, .state = READY };
    num_processes = 5;
}

void simulate_fcfs() {
    int cycle = 0;
    int completed = 0;
    Process *current_process = NULL;

    while (completed < num_processes) {
        // Check if any process finished I/O 
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == WAITING && cycle >= processes[i].ioTime + 3) { // I/O is 3
                processes[i].state = READY;
                printf("Cycle %d: Process %d moved from WAITING to READY\n", cycle, processes[i].id);
            }
        }

        // Choose the next ready process
        if (!current_process || current_process->state != RUNNING) {
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].state == READY && processes[i].arrivalTime <= cycle) {
                    current_process = &processes[i];
                    current_process->state = RUNNING;
                    printf("Cycle %d: Process %d moved from READY to RUNNING\n", cycle, current_process->id);
                    break;
                }
            }
        }

        // Run the current process
        if (current_process && current_process->state == RUNNING) {
            current_process->remainingTime--;
            printf("Cycle %d: Process %d is running, Remaining Time: %d\n", cycle, current_process->id, current_process->remainingTime);

            // Check if the process needs to go to I/O
            if (current_process->remainingTime == current_process->burstTime - current_process->ioTime && current_process->ioTime > 0) {
                printf("Cycle %d: Process %d goes to I/O\n", cycle, current_process->id);
                current_process->state = WAITING;
                current_process = NULL; // Clear current process if not, last one will not finish
            }

            // When process is done
            else if (current_process && current_process->remainingTime <= 0) {
                current_process->state = FINISHED;
                completed++;
                printf("Cycle %d: Process %d finished\n", cycle, current_process->id);
                current_process = NULL; // Clear current process if not, last one will not finish
            }
        }

        cycle++;
    }
    printf("All processes completed.\n");
}




void simulate_sjf() {
    int cycle = 0;
    int completed = 0;
    Process *current_process = NULL;

    while (completed < num_processes) {
        // Check if any process finished their I/O
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == WAITING && cycle >= processes[i].ioTime + 3) { // I/O time is 3
                processes[i].state = READY;
                printf("Cycle %d: Process %d moved from WAITING to READY\n", cycle, processes[i].id);
            }
        }

        // Choose the shortest job from ready processes (only if no process is running)
        if (!current_process || current_process->state != RUNNING) {
            Process *shortest_job = NULL;
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].state == READY && processes[i].arrivalTime <= cycle) {
                    if (!shortest_job || processes[i].remainingTime < shortest_job->remainingTime) {
                        shortest_job = &processes[i];
                    }
                }
            }
            current_process = shortest_job;
            if (current_process) {
                current_process->state = RUNNING;
                printf("Cycle %d: Process %d moved from READY to RUNNING\n", cycle, current_process->id);
            }
        }

        // Run the current process
        if (current_process && current_process->state == RUNNING) {
            current_process->remainingTime--;
            printf("Cycle %d: Process %d is running, Remaining Time: %d\n", cycle, current_process->id, current_process->remainingTime);

            // Check if process needs I/O
            if (current_process->remainingTime == current_process->burstTime - current_process->ioTime && current_process->ioTime > 0) {
                printf("Cycle %d: Process %d goes to I/O\n", cycle, current_process->id);
                current_process->state = WAITING;
                current_process = NULL; // Clear current process if not, last one will not finish
            }

            // When process it done, mark it as finished
            else if (current_process && current_process->remainingTime <= 0) {
                current_process->state = FINISHED;
                completed++;
                printf("Cycle %d: Process %d finished\n", cycle, current_process->id);
                current_process = NULL; // Clear current process if not, last one will not finish
            }
        }

        cycle++;
    }
    printf("All processes completed.\n");
}

void shell() {
    while (1) {
        printf("JayOS> ");
        char command[50];
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        // Check command contents
        printf("DEBUG: Command entered: %s\n", command);

        if (strcmp(command, "exit") == 0) {
            printf("Shutting down JaySH...\n");
            sleep(1);
            break; // Exit the loop
        }

        else if (strcmp(command, "IO") == 0) {
            inputOutputProcess(); // For future reference: this essentially creates a child process, and the parent process (shell) waits for the child process to finish
        }

        else if (strcmp(command, "FCFS") == 0) {
            reset_processes();  // Reset processes before FCFS
            simulate_fcfs();
        }

        else if (strcmp(command, "SJF") == 0) {
            reset_processes();  // Resets processes before SJF
            simulate_sjf(); 
        }

        else if (strcmp(command, "help") == 0) {
            printf("Commands available:\n");
            printf("1. IO - Input/Output process\n");
            printf("2. FCFS - First Come First Serve\n");
            printf("3. SJF - Shortest Job First\n");
            printf("4. help - Display available commands\n");
            printf("5. exit - Exit JayOS\n");
        }

        else {
            printf("Command not found. Type 'help' to see available commands.\n");
        }
    }
}

#endif // shell_h
