#ifndef OSQUEUE_H
#define OSQUEUE_H

#include <pcb.h>
#include <queue> // C++ for now

struct ComparePCB {
    bool operator()(PCB const& p1, PCB const& p2) {
        // Return "true" if "p1" should come after "p2"
        return p1.cpuBurstRemaining > p2.cpuBurstRemaining;
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
        for (int i = 0; i < 10; i++) {
            PCB pcb(i);
            SysQueue.push(pcb);
        }
    }
};

class OS_FCFS_ReadyQueue {
public:
    std::queue<PCB> SysQueue;
    OS_FCFS_ReadyQueue() {
        for (int i = 0; i < 10; i++) {
            PCB pcb(i);
            SysQueue.push(pcb);
        }
    }
};

#endif // OSQUEUE_H