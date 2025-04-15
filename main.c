#include <stdio.h>
#include <stdlib.h>
#include "part1.h"
#include "part2.h"

int main() {
    FILE *addressFile = fopen("addresses.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");

    int physicalMemory[256][256] = {0};
    int tlb[16][2] = {{-1, -1}};
    int pageTable[256][2] = {{-1, -1}};
    int pageFaultCounter = 0;
    int tlbHitCounter = 0;
    int addressReadCounter = 0;

    int logicalAddress;
    while (fscanf(addressFile, "%d", &logicalAddress) != EOF) {
        int offset = logicalAddress & 255;
        int pageNumber = (logicalAddress >> 8) & 255;
        printf("Logical address is: %d\nPageNumber is: %d\nOffset: %d\n", logicalAddress, pageNumber, offset);
        addressReadCounter++;

        int tlbHit = checkTLB(pageNumber, offset, logicalAddress, tlb, addressReadCounter, outputFile, physicalMemory);

        if (tlbHit == 1) {
            tlbHitCounter++;
        }

        if (tlbHit != 1) {
            int pageTableHit = checkPageTable(pageNumber, logicalAddress, offset, addressReadCounter, pageTable, physicalMemory, outputFile);
            if (pageTableHit != 1) {
                printf("This is a page fault!\n");
                pageFaultHandler(pageNumber, tlb, pageTable, physicalMemory);
                pageFaultCounter++;
                checkTLB(pageNumber, offset, logicalAddress, tlb, addressReadCounter, outputFile, physicalMemory);
            }
        }
    }

    float pageFaultRate = (float)pageFaultCounter / addressReadCounter;
    float tlbHitRate = (float)tlbHitCounter / addressReadCounter;

    fprintf(outputFile,
        "Number of translated address: %d\nNumber of page fault: %d\nPage fault rate: %.3f\nNumber of TLB hits: %d\nTLB hit rate: %.3f\n",
        addressReadCounter, pageFaultCounter, pageFaultRate, tlbHitCounter, tlbHitRate);

    printf("Number of translated address: %d\nNumber of page fault: %d\nPage fault rate: %.3f\nNumber of TLB hits: %d\nTLB hit rate: %.3f\n",
        addressReadCounter, pageFaultCounter, pageFaultRate, tlbHitCounter, tlbHitRate);

    fclose(addressFile);
    fclose(outputFile);
    return 0;
}
