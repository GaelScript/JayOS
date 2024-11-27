#ifndef shell_h
#define shell_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process1.h"
#include <time.h> //Needed for random
#include <unistd.h> //Needed for sleep

#define NUM_PAGES 16   //Number of pages in the virtual memory
#define NUM_FRAMES 16  //Number of frames
#define PAGE_SIZE 4    //Size of each page

//Mapping virtual page to a physical frame
struct PageTableEntry {
    int frameNumber;
};

struct ProcessControlBlock ShellPCB = {1, 64};

typedef enum { READY, RUNNING, WAITING, FINISHED } State;

typedef struct {
    int R1, R2, R3, R4; 
} Registers;

typedef struct {
    int id;
    int burstTime;
    int remainingTime;
    int arrivalTime;
    int ioTime;
    State state;
    Registers registers; //Registers for each process
    struct PageTableEntry pageTable[NUM_PAGES];  //Each process has a page table
} Process;

#define PROCESSES 5

Process processes[PROCESSES];
int num_processes = 0; 

void assign_registers(Process *proc) {
    //Assign random values to registers
    proc->registers.R1 = rand() % 100;  //Random value between 0-99
    proc->registers.R2 = rand() % 100;
    proc->registers.R3 = rand() % 100;
    proc->registers.R4 = rand() % 100;

    printf("Process %d: Registers assigned -> R1: %d, R2: %d, R3: %d, R4: %d\n",
           proc->id, proc->registers.R1, proc->registers.R2, proc->registers.R3, proc->registers.R4);
}

//Function to simulate the process of creating a page table for each process
void create_page_table(Process *proc) {
    for (int i = 0; i < NUM_PAGES; i++) {
        proc->pageTable[i].frameNumber = rand() % NUM_FRAMES;  //Random frame number
    }
}

//Function to simulate translating a virtual address to a physical address
int translate_address(Process *proc, int virtualAddress) {
    int pageNumber = virtualAddress / PAGE_SIZE;  //Get the page number
    int offset = virtualAddress % PAGE_SIZE;     //Get the offset within the page
    
    if (pageNumber < NUM_PAGES) {
        int frameNumber = proc->pageTable[pageNumber].frameNumber;
        int physicalAddress = frameNumber * PAGE_SIZE + offset; //Translate to physical address
        printf("Translation: Virtual Address %d was translated to Physical Address %d (Frame %d)\n", virtualAddress, physicalAddress, frameNumber);
        return physicalAddress;
    } else {
        printf("Invalid Virtual Address: %d\n", virtualAddress);
        return -1;
    }
}

void execute_instruction(Process *proc) {
    //Generate a random instruction (LD or ST)
    char instruction = rand() % 2 == 0 ? 'L' : 'S';  // Load or Store

    //Generate a random virtual address
    int virtualAddress = rand() % (NUM_PAGES * PAGE_SIZE);  //Make sure the virtual address is within bounds

    //Print the instruction and virtual address
    printf("Executing %c instruction with virtual address: %d\n", instruction, virtualAddress);

    //Translate the virtual address to a physical address
    int physicalAddress = translate_address(proc, virtualAddress);

    //Simulate the memory operation (just printing for now)
    if (physicalAddress != -1) {  //Check if the translation was successful
        if (instruction == 'L') {
            printf("Load operation at physical address: %d\n", physicalAddress);
        } else {
            printf("Store operation at physical address: %d\n", physicalAddress);
        }
    } else {
        printf("Translation failed for virtual address %d.\n", virtualAddress);
    }
}

bool check_for_interrupt() {
    return rand() % 10 == 0;  //10% chance of an interrupt happening
}

void handle_interrupt(Process *current_process) {
    //Context switch
    printf("Interrupt occurred! Handling context switch...\n");

    current_process->state = WAITING;
}

void simulate_cpu_burst(Process *current_process) {
    printf("Process %d is running...\n", current_process->id);
    
    //Assign registers
    assign_registers(current_process);

    //Simulate an instruction being executed
    execute_instruction(current_process);

    //Randomly decide if the process goes to waiting (I/O)
    if (rand() % 2 == 0) {
        current_process->state = WAITING;
        printf("Process %d moved to WAITING state.\n", current_process->id);
    } else {
        current_process->state = FINISHED;
        printf("Process %d finished execution.\n", current_process->id);
    }
}

void context_switch(Process **current_process) {
    printf("Context switch: Saving state of Process %d and switching...\n", (*current_process)->id);

    *current_process = &processes[(rand() % num_processes)];
    (*current_process)->state = RUNNING;
    printf("Now running Process %d\n", (*current_process)->id);
}


void reset_processes() {
    processes[0] = (Process){ .id = 1, .burstTime = 8, .remainingTime = 8, .arrivalTime = 0, .ioTime = 4, .state = READY };
    processes[1] = (Process){ .id = 2, .burstTime = 4, .remainingTime = 4, .arrivalTime = 2, .ioTime = -1, .state = READY };
    processes[2] = (Process){ .id = 3, .burstTime = 9, .remainingTime = 9, .arrivalTime = 4, .ioTime = 5, .state = READY };
    processes[3] = (Process){ .id = 4, .burstTime = 5, .remainingTime = 7, .arrivalTime = 1, .ioTime = 1, .state = READY };
    processes[4] = (Process){ .id = 5, .burstTime = 6, .remainingTime = 6, .arrivalTime = 5, .ioTime = 3, .state = READY };
    num_processes = 5;

    //Create page tables for each process
    for (int i = 0; i < num_processes; i++) {
        create_page_table(&processes[i]);
    }
}

void simulate_fcfs() {
    int cycle = 0;
    int completed = 0;
    Process *current_process = NULL;

    while (completed < num_processes) {
        //Check if any process finished I/O 
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == WAITING && cycle >= processes[i].ioTime + 3) { // I/O is 3
                processes[i].state = READY;
                printf("Cycle %d: Process %d moved from WAITING to READY\n", cycle, processes[i].id);
            }
        }

        //Choose the next ready process
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

        //Run the current process
        if (current_process && current_process->state == RUNNING) {
            current_process->remainingTime--;
            printf("Cycle %d: Process %d is running, Remaining Time: %d\n", cycle, current_process->id, current_process->remainingTime);

            //Check if the process needs to go to I/O
            if (current_process->remainingTime == current_process->burstTime - current_process->ioTime && current_process->ioTime > 0) {
                printf("Cycle %d: Process %d goes to I/O\n", cycle, current_process->id);
                current_process->state = WAITING;
                current_process = NULL; //Clear current process if not, last one will not finish
            }

            //When process is done
            else if (current_process && current_process->remainingTime <= 0) {
                current_process->state = FINISHED;
                completed++;
                printf("Cycle %d: Process %d finished\n", cycle, current_process->id);
                current_process = NULL; //Clear current process if not, last one will not finish
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
        //Check if any process finished their I/O
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == WAITING && cycle >= processes[i].ioTime + 3) { // I/O time is 3
                processes[i].state = READY;
                printf("Cycle %d: Process %d moved from WAITING to READY\n", cycle, processes[i].id);
            }
        }

        //Choose the shortest job from ready processes (only if no process is running)
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

        //Run the current process
        if (current_process && current_process->state == RUNNING) {
            current_process->remainingTime--;
            printf("Cycle %d: Process %d is running, Remaining Time: %d\n", cycle, current_process->id, current_process->remainingTime);

            //Check if process needs I/O
            if (current_process->remainingTime == current_process->burstTime - current_process->ioTime && current_process->ioTime > 0) {
                printf("Cycle %d: Process %d goes to I/O\n", cycle, current_process->id);
                current_process->state = WAITING;
                current_process = NULL; //Clear current process if not, last one will not finish
            }

            //When process it done, mark it as finished
            else if (current_process && current_process->remainingTime <= 0) {
                current_process->state = FINISHED;
                completed++;
                printf("Cycle %d: Process %d finished\n", cycle, current_process->id);
                current_process = NULL; //Clear current process if not, last one will not finish
            }
        }

        cycle++;
    }
    printf("All processes completed.\n");
}


void shell() {
    Process *current_process = NULL;  //Pointer to the current process running in the shell
    
    while (1) {
        printf("JayOS> ");
        char command[50];
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; //Remove newline character

        //Check command contents
        printf("DEBUG: Command entered: %s\n", command);

        if (strcmp(command, "exit") == 0) {
            printf("Shutting down JaySH...\n");
            sleep(1);
            break; //Exit the loop
        }

        else if (strcmp(command, "IO") == 0) {
            inputOutputProcess(); //For future reference: this essentially creates a child process, and the parent process (shell) waits for the child process to finish
        }

        else if (strcmp(command, "FCFS") == 0) {
            reset_processes();  //Reset processes before FCFS
            simulate_fcfs();
        }

        else if (strcmp(command, "SJF") == 0) {
            reset_processes();  //Resets processes before SJF
            simulate_sjf(); 
        }

        //Simulating a CPU burst with memory operations (LD/ST)
        else if (strcmp(command, "simulate_burst") == 0) {
            reset_processes();
            if (num_processes <= 0) {
                printf("Error: No processes available to run.\n");
                continue;  //Skip this command if no processes are available
            }

            if (current_process == NULL) {
                //Select a process to run if none is running
                current_process = &processes[rand() % num_processes];
                current_process->state = RUNNING;
                printf("Selected Process %d to start execution.\n", current_process->id);
            }

            simulate_cpu_burst(current_process);

            //Check for interrupt during execution
            if (check_for_interrupt()) {
                handle_interrupt(current_process);
                context_switch(&current_process); //Switch to another process if interrupted
            }
        }

        else if (strcmp(command, "help") == 0) {
            printf("Commands available:\n");
            printf("1. IO - Input/Output process\n");
            printf("2. FCFS - First Come First Serve\n");
            printf("3. SJF - Shortest Job First\n");
            printf("4. simulate_burst - Simulate a CPU burst with memory operations (LD/ST)\n");
            printf("5. help - Display available commands\n");
            printf("6. exit - Exit JayOS\n");
        }

        else {
            printf("Command not found. Type 'help' to see available commands.\n");
        }
    }
}

#endif //shell_h
