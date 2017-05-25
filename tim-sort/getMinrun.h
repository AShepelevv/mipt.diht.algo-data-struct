#ifndef getMinrun_h
#define getMinrun_h

const int minSizeForInsertSort = 64;

unsigned int getMinRun(unsigned int sizeOfArray, unsigned int minSize = minSizeForInsertSort) {
    unsigned int minRunLength = 0;
    while (sizeOfArray >= minSizeForInsertSort) {
        minRunLength |= sizeOfArray & 1;
        sizeOfArray >>= 1;
    }
    return sizeOfArray + minRunLength;
}

void print (int* begin, int* end) {
    for (int i = 0; i < end - begin; ++i) {
        std::cout << begin[i] << " ";
    }
    std::cout << std::endl;
}

#endif /* getMinrun_h */
