#ifndef process1_h
#define process1_h

#include <stdio.h> 
#include <unistd.h> //Added for sleep() function to work

struct ProcessControlBlock{
    int processID;
    int memoryAllocated;
};

//Process simulation: I/O
void inputOutputProcess(){
    struct ProcessControlBlock pcb;
    pcb.processID = 2;
    pcb.memoryAllocated = 120;

    //First, we create process and allocate memory for it
    printf("Process is being created with PID: %d", pcb.processID);
        for (int i = 0; i < 3; ++i) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    printf("%d KB of memory has been allocated for the process.\n", pcb.memoryAllocated);
    printf("Process Control Block is initializing, and we are setting up registers");
        for (int i = 0; i < 3; ++i) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n\n");

    //Process simulation occurs here
    int number;
    printf("Input/Output process has begun.\n");

    printf("Please enter a number:\n");
    scanf("%d", &number);

    // Consumes the leftover newline character
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("%d squared is equal to %d.\n", number, number * number);

    printf("Input/Output process has finished!\n\n");

    printf("Terminating process..\n");
    sleep(1);

    //Releasing memory of process and cleaning up
    int memoryDeAllocated = 120;
    printf("Releasing %d KB of memory", memoryDeAllocated);
    for (int i = 0; i < 3; ++i) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    printf("Cleaning up Process Control Block and resetting registers");
    for (int i = 0; i < 4; ++i) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    printf("Process has terminated successfully!\n");
}


#endif
