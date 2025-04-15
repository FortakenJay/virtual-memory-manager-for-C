
#include <stdio.h>
#include "part1.h"
#include "part2.h"

int checkTLB(int pageNumber, int offset, int logicalAddress, int tlb[16][2], int accessCount, FILE *outputFile, int physicalMemory[256][256]) {
    for (int j = 0; j < 16; j++) {
        if (tlb[j][0] == pageNumber) {
            printf("Page Number \"%d\" found in TLB!!\n", pageNumber);
            int frameNumber = tlb[j][1];
            char *data = readPhysicalMemory(frameNumber, offset, physicalMemory);
            int physicalAddress = (frameNumber << 8) | offset;
            fprintf(outputFile, "%d Virtual address: %d Physical address: %d Value: %s\n", accessCount, logicalAddress, physicalAddress, data);
            printf("%d Virtual address: %d Physical address: %d Value: %s\n", accessCount, logicalAddress, physicalAddress, data);
            updateTLBCounter(j, tlb);
            return 1;
        }
    }
    return 0;
}

int checkPageTable(int pageNumber, int logicalAddress, int offset, int accessCount, int pageTable[256][2], int physicalMemory[256][256], FILE *outputFile) {
    for (int k = 0; k < 256; k++) {
        if (pageTable[k][0] == pageNumber) {
            printf("Page Number \"%d\" found in page table!!\n", pageNumber);
            int frameNumber = pageTable[k][1];
            char *data = readPhysicalMemory(frameNumber, offset, physicalMemory);
            int physicalAddress = (frameNumber << 8) | offset;
            fprintf(outputFile, "%d Virtual address: %d Physical address: %d Value: %s\n", accessCount, logicalAddress, physicalAddress, data);
            printf("%d Virtual address: %d Physical address: %d Value: %s\n", accessCount, logicalAddress, physicalAddress, data);
            updatePageTableCounter(k, pageTable);
            return 1;
        }
    }
    return 0;
}
