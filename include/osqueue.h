#ifndef OSQUEUE_H
#define OSQUEUE_H

#include <pcb.h>
#include <queue> // C++ for now

struct ComparePCB {
    bool operator()(PCB const& p1, PCB const& p2) {
        // Return "true" if "p1" should come after "p2"
        return p1.cpuBurstLength > p2.cpuBurstLength;
    }
};

class OS_WaitingQueue {
public:
    std::queue<PCB> SysQueue;
};

class OS_SJF_ReadyQueue {
public:
    std::priority_queue<PCB, std::vector<PCB>, ComparePCB> SysQueue;

    OS_SJF_ReadyQueue() {
        srand(static_cast<unsigned int>(time(NULL)));
        for (int i = 0; i < 20; i++) {
            PCB pcb;
            pcb.processID = i;
            pcb.cpuBurstLength = rand() % 10 + 1;
            pcb.cpuBurstRemaining = rand() % 100 + 1;
            pcb.ioBurstLength = rand() % 10 + 1;
            pcb.ioBurstRemaining = rand() % 100 + 1;
            SysQueue.push(pcb);
        }
    }
};

class OS_FCFS_ReadyQueue {
public:
    std::queue<PCB> SysQueue;
    OS_FCFS_ReadyQueue() {
        srand(static_cast<unsigned int>(time(NULL)));
        for (int i = 0; i < 20; i++) {
            PCB pcb;
            pcb.processID = i;
            pcb.cpuBurstLength = rand() % 10 + 1;
            pcb.cpuBurstRemaining = rand() % 100 + 1;
            pcb.ioBurstLength = rand() % 10 + 1;
            pcb.ioBurstRemaining = rand() % 100 + 1;
            SysQueue.push(pcb);
        }
    }
};

#endif // OSQUEUE_H