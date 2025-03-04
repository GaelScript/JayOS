#ifndef process1_h
#define process1_h

#include <stdio.h>
#include <unistd.h> //Added for sleep() function to work
#include <vector>
#include "pcb.h"
#include "osqueue.h"

#define MAX_PHYS_MEMORY 10 // 10*128KB = 1280KB of memory

std::vector<int> physicalMemory(MAX_PHYS_MEMORY, 0);

void SJFProcesses()
{
    printf("DEBUG:\n");
    OS_SJF_ReadyQueue readyQueue;
    OS_WaitingQueue waitingQueue;

    // debug output of all the burst lengths for sanity's sake
    std::priority_queue<PCB, std::vector<PCB>, ComparePCB> tempQueue = readyQueue.SysQueue;

    std::cout << "\nInitial Process Burst Times:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    while (!tempQueue.empty())
    {
        PCB process = tempQueue.top();
        std::cout << "PID: " << process.processID
                  << " | CPU Burst Length: " << process.cpuBurstLength
                  << " | CPU Burst Remaining: " << process.cpuBurstRemaining
                  << " | I/O Burst Length: " << process.ioBurstLength
                  << " | I/O Burst Remaining: " << process.ioBurstRemaining << std::endl;
        tempQueue.pop();
    }
    std::cout << "----------------------------\n\n";
    while (!readyQueue.SysQueue.empty())
    {
        PCB shortest = readyQueue.SysQueue.top();
        readyQueue.SysQueue.pop();

        for (auto &pair : shortest.pageTable)
        { // simulates process occupying physical memory.
            physicalMemory[pair.second] = 1;
        }
        std::cout << "\n\nDEBUG: PID: " << shortest.processID << " ENTERING CPU" << std::endl;
        std::cout << "[";
        for (const auto &address : physicalMemory)
        {
            std::cout << address << ", ";
        }
        std::cout << "]\n";

        if (shortest.cpuBurstRemaining == 0)
        {
            printf("Process %d has finished executing.\n", shortest.processID);
            continue;
        }
        else if (shortest.cpuBurstRemaining < shortest.cpuBurstLength)
        {
            printf("Process %d is finishing executing with burst length %d\n", shortest.processID, shortest.cpuBurstRemaining);
            continue;
        }

        printf("Process %d is executing with burst length %d and remaining length %d.\n", shortest.processID, shortest.cpuBurstLength, shortest.cpuBurstRemaining);
        shortest.cpuBurstRemaining = shortest.cpuBurstRemaining - shortest.cpuBurstLength;

        // Simulate I/O
        waitingQueue.SysQueue.push(shortest);
        PCB ioProcess = waitingQueue.SysQueue.front();
        waitingQueue.SysQueue.pop();
        if (ioProcess.ioBurstRemaining == 0)
        {
            readyQueue.SysQueue.push(ioProcess);
            continue;
        }
        else if (ioProcess.ioBurstRemaining < ioProcess.ioBurstLength)
        {
            std::cout << "DEBUG: PID: " << shortest.processID << " LEAVING CPU." << std::endl;

            for (auto &pair : shortest.pageTable)
            { // simulates process releasing physical memory.
                physicalMemory[pair.second] = 0;
            }

            std::cout << "[";
            for (const auto &address : physicalMemory)
            {
                std::cout << address << ", ";
            }
            std::cout << "]\n";
            printf("Process %d is waiting for I/O.\n", shortest.processID);
            printf("Process %d is performing it's final I/O with burst length %d and remaining %d.\n", ioProcess.processID, ioProcess.ioBurstLength, ioProcess.ioBurstRemaining);
            ioProcess.ioBurstRemaining = 0;
            readyQueue.SysQueue.push(ioProcess);
            continue;
        }

        std::cout << "DEBUG: PID: " << shortest.processID << " LEAVING CPU." << std::endl;

        for (auto &pair : shortest.pageTable)
        { // simulates process releasing physical memory.
            physicalMemory[pair.second] = 0;
        }

        std::cout << "[";
        for (const auto &address : physicalMemory)
        {
            std::cout << address << ", ";
        }
        std::cout << "]\n";

        printf("Process %d is waiting for I/O.\n", shortest.processID);
        printf("Process %d is performing I/O with burst length %d and length remaining %d.\n", ioProcess.processID, ioProcess.ioBurstLength, ioProcess.ioBurstRemaining);
        ioProcess.ioBurstRemaining = ioProcess.ioBurstRemaining - ioProcess.ioBurstLength;
        readyQueue.SysQueue.push(ioProcess);
    }
    printf("All processes have finished executing.\n");
}

void FCFSProcesses()
{
    printf("DEBUG:\n");
    OS_FCFS_ReadyQueue readyQueue;
    OS_WaitingQueue waitingQueue;

    // Debug output of all the burst lengths for sanity's sake
    std::queue<PCB> tempQueue = readyQueue.SysQueue;

    std::cout << "\nInitial Process Burst Times:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    while (!tempQueue.empty())
    {
        PCB process = tempQueue.front();
        std::cout << "PID: " << process.processID
                  << " | CPU Burst Length: " << process.cpuBurstLength
                  << " | CPU Burst Remaining: " << process.cpuBurstRemaining
                  << " | I/O Burst Length: " << process.ioBurstLength
                  << " | I/O Burst Remaining: " << process.ioBurstRemaining << std::endl;
        tempQueue.pop();
    }
    std::cout << "----------------------------\n\n";

    while (!readyQueue.SysQueue.empty())
    {
        PCB first = readyQueue.SysQueue.front();
        readyQueue.SysQueue.pop();

        for (auto &pair : first.pageTable)
        { // simulates process occupying physical memory.
            physicalMemory[pair.second] = 1;
        }
        std::cout << "\n\nDEBUG: PID: " << first.processID << " ENTERING CPU" << std::endl;
        std::cout << "[";
        for (const auto &address : physicalMemory)
        {
            std::cout << address << ", ";
        }
        std::cout << "]\n";

        if (first.cpuBurstRemaining == 0)
        {
            printf("Process %d has finished executing.\n", first.processID);
            continue;
        }
        else if (first.cpuBurstRemaining < first.cpuBurstLength)
        {
            printf("Process %d is finishing executing with burst length %d.\n", first.processID, first.cpuBurstRemaining);
            continue;
        }

        printf("Process %d is executing with burst length %d and remaining bursts %d.\n", first.processID, first.cpuBurstLength, first.cpuBurstRemaining);
        first.cpuBurstRemaining = first.cpuBurstRemaining - first.cpuBurstLength;

        // Simulate I/O
        waitingQueue.SysQueue.push(first);
        PCB ioProcess = waitingQueue.SysQueue.front();
        waitingQueue.SysQueue.pop();
        if (ioProcess.ioBurstRemaining == 0)
        {
            readyQueue.SysQueue.push(ioProcess);
            continue;
        }
        else if (ioProcess.ioBurstRemaining < ioProcess.ioBurstLength)
        {
            std::cout << "\n\nDEBUG: PID: " << first.processID << " LEAVING CPU." << std::endl;

            for (auto &pair : first.pageTable)
            { // simulates process releasing physical memory.
                physicalMemory[pair.second] = 0;
            }

            std::cout << "[";
            for (const auto &address : physicalMemory)
            {
                std::cout << address << ", ";
            }
            std::cout << "]\n";

            printf("Process %d is waiting for I/O.\n", first.processID);
            printf("Process %d is finish I/O with burst length %d and remaining %d.\n", ioProcess.processID, ioProcess.cpuBurstLength, ioProcess.ioBurstRemaining);
            ioProcess.ioBurstRemaining = 0;
            readyQueue.SysQueue.push(ioProcess);

            continue;
        }

        std::cout << "\n\nDEBUG: PID: " << first.processID << " LEAVING CPU." << std::endl;

        for (auto &pair : first.pageTable)
        { // simulates process releasing physical memory.
            physicalMemory[pair.second] = 0;
        }

        std::cout << "[";
        for (const auto &address : physicalMemory)
        {
            std::cout << address << ", ";
        }
        std::cout << "]\n";

        printf("Process %d is waiting for I/O.\n", first.processID);
        printf("Process %d is performing I/O with burst length %d and remaining burts %d.\n", ioProcess.processID, ioProcess.ioBurstLength, ioProcess.ioBurstRemaining);
        ioProcess.ioBurstRemaining = ioProcess.ioBurstRemaining - ioProcess.ioBurstLength;
        readyQueue.SysQueue.push(ioProcess);
    }
    printf("All processes have finished executing.\n");
}

// Process simulation: I/O
void inputOutputProcess()
{
    PCB pcb(84892);
    pcb.processID = 84892;

    // First, we create process and allocate memory for it
    printf("Process is being created with PID: %d", pcb.processID);
    for (int i = 0; i < 3; ++i)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    printf("%d KB of memory has been allocated for the process.\n", pcb.physMemoryUsed);
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
