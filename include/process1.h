#ifndef process1_h
#define process1_h

#include <stdio.h>
#include <unistd.h> //Added for sleep() function to work
#include "pcb.h"
#include "osqueue.h"

void SJFProcesses()
{
    printf("DEBUG:\n");
    OS_SJF_ReadyQueue readyQueue;
    OS_WaitingQueue waitingQueue;
    while (!readyQueue.SysQueue.empty())
    {
        PCB shortest = readyQueue.SysQueue.top();
        readyQueue.SysQueue.pop();
        if (shortest.cpuBurstRemaining == 0)
        {
            printf("Process %d has finished executing.\n", shortest.processID);
            continue;
        }
        else if (shortest.cpuBurstRemaining < shortest.cpuBurstLength){
            printf("Process %d is finishing executing with burst length %d.\n", shortest.processID, shortest.cpuBurstRemaining);
            continue;
        }

        printf("Process %d is executing with burst length %d.\n", shortest.processID, shortest.cpuBurstLength);
        shortest.cpuBurstRemaining = shortest.cpuBurstRemaining - shortest.cpuBurstLength;

        // Simulate I/O
        printf("Process %d is waiting for I/O.\n", shortest.processID);
        waitingQueue.SysQueue.push(shortest);
        PCB ioProcess = waitingQueue.SysQueue.front();
        waitingQueue.SysQueue.pop();
        if (ioProcess.ioBurstRemaining == 0)
        {
            printf("Process %d has finished I/O.\n", ioProcess.processID);
            continue;
        }
        else if (ioProcess.ioBurstRemaining < ioProcess.ioBurstLength){
            printf("Process %d is performing I/O with burst length %d.\n", ioProcess.processID, ioProcess.ioBurstRemaining);
            continue;
        }
        printf("Process %d is performing I/O with burst length %d.\n", ioProcess.processID, ioProcess.ioBurstLength);
        ioProcess.ioBurstRemaining = ioProcess.ioBurstRemaining - ioProcess.ioBurstLength;
        readyQueue.SysQueue.push(ioProcess);
    }
    printf("All processes have finished executing.\n");
}

void FCFSProcesses(){
    printf("DEBUG:\n");
    OS_FCFS_ReadyQueue readyQueue;
    OS_WaitingQueue waitingQueue;
    while (!readyQueue.SysQueue.empty())
    {
        PCB first = readyQueue.SysQueue.front();
        readyQueue.SysQueue.pop();
        if (first.cpuBurstRemaining == 0)
        {
            printf("Process %d has finished executing.\n", first.processID);
            continue;
        }
        else if (first.cpuBurstRemaining < first.cpuBurstLength){
            printf("Process %d is finishing executing with burst length %d.\n", first.processID, first.cpuBurstRemaining);
            continue;
        }

        printf("Process %d is executing with burst length %d.\n", first.processID, first.cpuBurstLength);
        first.cpuBurstRemaining = first.cpuBurstRemaining - first.cpuBurstLength;

        // Simulate I/O
        printf("Process %d is waiting for I/O.\n", first.processID);
        waitingQueue.SysQueue.push(first);
        PCB ioProcess = waitingQueue.SysQueue.front();
        waitingQueue.SysQueue.pop();
        if (ioProcess.ioBurstRemaining == 0)
        {
            printf("Process %d has finished I/O.\n", ioProcess.processID);
            continue;
        }
        else if (ioProcess.ioBurstRemaining < ioProcess.ioBurstLength){
            printf("Process %d is performing I/O with burst length %d.\n", ioProcess.processID, ioProcess.ioBurstRemaining);
            continue;
        }
        printf("Process %d is performing I/O with burst length %d.\n", ioProcess.processID, ioProcess.ioBurstLength);
        ioProcess.ioBurstRemaining = ioProcess.ioBurstRemaining - ioProcess.ioBurstLength;
        readyQueue.SysQueue.push(ioProcess);
    }
    printf("All processes have finished executing.\n");
}

// Process simulation: I/O
void inputOutputProcess()
{
    PCB pcb;
    pcb.processID = 84892;
    pcb.memoryAllocated = 120;

    // First, we create process and allocate memory for it
    printf("Process is being created with PID: %d", pcb.processID);
    for (int i = 0; i < 3; ++i)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    printf("%d KB of memory has been allocated for the process.\n", pcb.memoryAllocated);
    printf("Process Control Block is initializing, and we are setting up registers");
    for (int i = 0; i < 3; ++i)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n\n");

    // Process simulation occurs here
    int number;
    printf("Input/Output process has begun.\n");

    printf("Please enter a number:\n");
    scanf("%d", &number);

    // Consumes the leftover newline character
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("%d squared is equal to %d.\n", number, number * number);

    printf("Input/Output process has finished!\n\n");

    printf("Terminating process..\n");
    sleep(1);

    // Releasing memory of process and cleaning up
    int memoryDeAllocated = 120;
    printf("Releasing %d KB of memory", memoryDeAllocated);
    for (int i = 0; i < 3; ++i)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    printf("Cleaning up Process Control Block and resetting registers");
    for (int i = 0; i < 4; ++i)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    printf("Process has terminated successfully!\n");
}

#endif
