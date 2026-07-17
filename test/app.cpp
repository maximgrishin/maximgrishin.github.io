#include <io/io.hpp>

unsigned long testIdx;


#define ASSERT(x) if (!(x)) {return failedAssertNum;} else {++failedAssertNum;}

int testAlloc() {
    int failedAssertNum = 1;
    {
        void* a = operator new(4);
        operator delete(a);
        void* b = operator new(4);
        ASSERT(a == b);
        operator delete(b);
    }
    {
        void* a = operator new(4);
        void* b = operator new(4);
        operator delete(a);
        operator delete(b);
        void* c = operator new(4);
        void* d = operator new(4);
        ASSERT(a == d);
        ASSERT(b == c);
        operator delete(c);
        operator delete(d);
    }
    {
        void *a = operator new[](16);
        void *b = operator new[](32);
        ASSERT(a != b);
        operator delete[](b);
        operator delete[](a);
    }
    return 0;
}

int (*tests[])() = {
    testAlloc,
};

void onframe() {
    if (testIdx < sizeof(tests)/sizeof(tests[0])) {
        io::cls();
        int result = tests[testIdx]();
        if (result == 0) {
            io::cset(10, testIdx, 'O', io::White, io::Green);
            io::cset(11, testIdx, 'K', io::White, io::Green);
        }
        else {
            io::cset(10, testIdx, '0'+result, io::Red, io::Black);
        }
        ++testIdx;
    }
}

void init() {
    io::onframe(onframe);
}
