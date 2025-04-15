#ifndef PART1_H
#define PART1_H

#include <stdio.h>

int checkTLB(int pageNumber, int offset, int logicalAddress, int tlb[16][2], int accessCount, FILE *outputFile, int physicalMemory[256][256]);
int checkPageTable(int pageNumber, int logicalAddress, int offset, int accessCount, int pageTable[256][2], int physicalMemory[256][256], FILE *outputFile);

#endif