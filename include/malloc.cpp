union BlockHeader {
    BlockHeader* next;
    int bucketIdx;
};

constexpr int kNumLists = 28;
constexpr int kSmallestBlockSize = 16;
//constexpr int kLargestBlockSize = kSmallestBlockSize << (kNumLists - 1);

BlockHeader* listHeads[kNumLists];
void* usedHeapEnd;

void* malloc(int request) {
    request += sizeof(BlockHeader);

    int bucketIdx = 0;
    for(int size = kSmallestBlockSize; size < request; size *= 2) {
        ++bucketIdx;
    }

    if (void* result = listHeads[bucketIdx]) {
        listHeads[bucketIdx] = listHeads[bucketIdx]->next;
        return result;
    }
    else {
        result = ((BlockHeader*) usedHeapEnd) + 1;
        ((BlockHeader*) usedHeapEnd)->bucketIdx = bucketIdx;
        usedHeapEnd = (char*)usedHeapEnd + (kSmallestBlockSize << bucketIdx);
        return result;
    }
}

void free(void* area) {
    BlockHeader* block = ((BlockHeader*) area) - 1;
    block->next = listHeads[block->bucketIdx];
    listHeads[block->bucketIdx] = block;
}
