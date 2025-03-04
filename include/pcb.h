#ifndef PCB_H
#define PCB_H
#include <vector>
#include <map>
#include <iostream>

const int MAX_PHYS_MEMORY = 10; // 10*128KB = 1280KB of memory

class PCB
{
public:
    int processID;
    int cpuBurstLength;
    int cpuBurstRemaining;
    int ioBurstLength;
    int ioBurstRemaining;
    int physMemoryUsed;
    std::vector<int> virtualMemory;
    std::map<int, int> pageTable;
    PCB(int pid)
    {
        this->processID = pid;
        this->cpuBurstLength = rand() % 10 + 1;
        this->cpuBurstRemaining = rand() % 30 + 1;
        this->ioBurstLength = rand() % 10 + 1;
        this->ioBurstRemaining = rand() % 30 + 1;

        virtualMemory.resize(MAX_PHYS_MEMORY * 2, 0);
        this->physMemoryUsed = rand() % MAX_PHYS_MEMORY + 1; // Because the page table only allocates what's used.

        std::vector<bool> usedFrames(MAX_PHYS_MEMORY, false);

        for (int i = 0; i < this->physMemoryUsed; i++)
        {
            virtualMemory[i] = 1;
            int frame = rand() % MAX_PHYS_MEMORY;
            if (usedFrames[frame] == false)
            {
                pageTable.insert(std::pair<int, int>(i, frame));
                usedFrames[frame] = true;
            }
            else
            {
                while (usedFrames[frame] == true)
                {
                    frame = (frame + 1) % MAX_PHYS_MEMORY;
                }
                pageTable.insert(std::pair<int, int>(i, frame));
                usedFrames[frame] = true;
            }
        }
        std::cout << "Page Table for Process " << pid << ":" << std::endl;
        for (const auto &entry : pageTable)
        {
            std::cout << "Virtual Page " << entry.first << " -> Physical Frame " << entry.second << std::endl;
        }
    }
};
#endif // PCB_H