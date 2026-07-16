union BlockHeader {
    BlockHeader* next;
    int bucketIdx;
};

namespace std {
    typedef unsigned long size_t;
}

constexpr int kNumLists = 30;
constexpr std::size_t kSmallestBlockSize = sizeof(BlockHeader);
//constexpr std::size_t kLargestBlockSize = kSmallestBlockSize << (kNumLists - 1);

extern unsigned char __heap_base;

BlockHeader* listHeads[kNumLists];
BlockHeader* usedHeapEnd = (BlockHeader*)&__heap_base;

void* malloc(std::size_t request) {
    request += sizeof(BlockHeader);

    int bucketIdx = 0;
    for(std::size_t blockSize = kSmallestBlockSize; blockSize < request; blockSize *= 2) {
        ++bucketIdx;
    }

    BlockHeader* blockHeader = 0;
    if (listHeads[bucketIdx]) {
        blockHeader = listHeads[bucketIdx];
        listHeads[bucketIdx] = listHeads[bucketIdx]->next;
    }
    else {
        blockHeader = usedHeapEnd;
        usedHeapEnd = usedHeapEnd + (1 << bucketIdx);
    }
    blockHeader->bucketIdx = bucketIdx;
    return blockHeader + 1;
}

void free(void* blockArea) {
    if (!blockArea) {
        return;
    }
    BlockHeader* blockHeader = ((BlockHeader*) blockArea) - 1;
    int bucketIdx = blockHeader->bucketIdx;
    BlockHeader* oldListHead = listHeads[bucketIdx];

    listHeads[bucketIdx] = blockHeader;
    blockHeader->next = oldListHead;
}

void* operator new(std::size_t size) {
    return malloc(size);
}

void* operator new[](std::size_t size) {
    return malloc(size);
}

void operator delete(void* ptr, std::size_t) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}
