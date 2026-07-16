#include <io/io.hpp>

unsigned long testIdx;


#define ASSERT(x) if (!(x)) {return failedAssertNum;} else {++failedAssertNum;}

int testAlloc() {
    int failedAssertNum = 1;
    {
        int* a = new int;
        delete a;
        int* b = new int;
        ASSERT(a == b);
        delete b;
    }
    {
        int* a = new int;
        int* b = new int;
        delete a;
        delete b;
        int* c = new int;
        int* d = new int;
        ASSERT(a == d);
        ASSERT(b == c);
        delete c;
        delete d;
    }
    {
        char *a = new char[16];
        char *b = new char[32];
        ASSERT(a != b);
        delete[] b;
        delete[] a;
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
