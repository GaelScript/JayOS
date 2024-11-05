#ifndef PCB_H
#define PCB_H
typedef struct {
    int processID;
    int memoryAllocated;
    int cpuBurstLength;
    int cpuBurstRemaining;
    int ioBurstLength;
    int ioBurstRemaining;
}PCB;
#endif // PCB_H