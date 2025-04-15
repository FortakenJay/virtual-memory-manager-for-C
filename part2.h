#ifndef PART2_H
#define PART2_H

void pageFaultHandler(int pageNumber, int tlb[16][2], int pageTable[256][2], int physicalMemory[256][256]);
void updateTLB(int pageNumber, int frameNumber, int tlb[16][2]);
void updatePageTable(int pageNumber, int frameNumber, int pageTable[256][2]);
void updateTLBCounter(int latestIndex, int tlb[16][2]);
void updatePageTableCounter(int latestIndex, int pageTable[256][2]);
char* readPhysicalMemory(int frameNumber, int offset, int physicalMemory[256][256]);

#endif
