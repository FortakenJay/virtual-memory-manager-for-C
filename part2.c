#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "part2.h"

// Buffer to store a single byte read as a string
char readBuffer[8];

void pageFaultHandler(int pageNumber, int tlb[16][2], int pageTable[256][2], int physicalMemory[256][256]) {
    if (pageNumber >= 256) {
        printf("Page \"%d\" is out of bound!\n", pageNumber);
        return;
    }

    int frameNumber = -1;
    for (int i = 0; i < 256; i++) {
        int isUsed = 0;
        for (int j = 0; j < 256; j++) {
            if (physicalMemory[i][j] != 0) {
                isUsed = 1;
                break;
            }
        }
        if (!isUsed) {
            frameNumber = i;
            break;
        }
    }

    if (frameNumber == -1) frameNumber = 0; // fallback if all used

    FILE *backStore = fopen("BACKING_STORE.bin", "rb");
    if (!backStore) {
        printf("Error opening BACKING_STORE.bin\n");
        exit(1);
    }

    fseek(backStore, pageNumber * 256, SEEK_SET);
    for (int i = 0; i < 256; i++) {
        char byte;
        fread(&byte, sizeof(char), 1, backStore);
        physicalMemory[frameNumber][i] = byte;
    }

    fclose(backStore);

    printf("Found page \"%d\" loaded into frame %d\n", pageNumber, frameNumber);
    updateTLB(pageNumber, frameNumber, tlb);
    updatePageTable(pageNumber, frameNumber, pageTable);
}

void updateTLB(int pageNumber, int frameNumber, int tlb[16][2]) {
    for (int i = 0; i < 15; i++) {
        tlb[i][0] = tlb[i + 1][0];
        tlb[i][1] = tlb[i + 1][1];
    }
    tlb[15][0] = pageNumber;
    tlb[15][1] = frameNumber;
    printf("Updated TLB with page %d -> frame %d\n", pageNumber, frameNumber);
}

void updatePageTable(int pageNumber, int frameNumber, int pageTable[256][2]) {
    for (int i = 0; i < 255; i++) {
        pageTable[i][0] = pageTable[i + 1][0];
        pageTable[i][1] = pageTable[i + 1][1];
    }
    pageTable[255][0] = pageNumber;
    pageTable[255][1] = frameNumber;
    printf("Updated page table with page %d -> frame %d\n", pageNumber, frameNumber);
}

void updateTLBCounter(int latestIndex, int tlb[16][2]) {
    int page = tlb[latestIndex][0];
    int frame = tlb[latestIndex][1];
    for (int i = latestIndex; i < 15; i++) {
        tlb[i][0] = tlb[i + 1][0];
        tlb[i][1] = tlb[i + 1][1];
    }
    tlb[15][0] = page;
    tlb[15][1] = frame;
    printf("Reordered TLB with LRU\n");
}

void updatePageTableCounter(int latestIndex, int pageTable[256][2]) {
    int page = pageTable[latestIndex][0];
    int frame = pageTable[latestIndex][1];
    for (int i = latestIndex; i < 255; i++) {
        pageTable[i][0] = pageTable[i + 1][0];
        pageTable[i][1] = pageTable[i + 1][1];
    }
    pageTable[255][0] = page;
    pageTable[255][1] = frame;
    printf("Reordered page table with LRU\n");
}

char* readPhysicalMemory(int frameNumber, int offset, int physicalMemory[256][256]) {
    sprintf(readBuffer, "%d", physicalMemory[frameNumber][offset]);
    printf("Read data \"%s\" from frame %d offset %d\n", readBuffer, frameNumber, offset);
    return readBuffer;
}
