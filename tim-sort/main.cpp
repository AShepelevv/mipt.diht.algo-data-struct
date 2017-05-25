#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include <cmath>
#include <string>

#include "timSort.h"
#include "test.h"
#include "inplaceMerge.h"
#include "getMinrun.h"


/*void testMerge () {
    std::srand(int(std::time(NULL)));
    int minValue = 325523;
    int maxSize = 0;
    for (int y = 0; y < 10000; ++y) {
        int size1 = rand() % 10000 + 1;
        int size2 = rand() % 10000 + 1;
        int* testarr1 = new int[size1];
        int* testarr2 = new int[size2];
        int i = 0;
        for (; i < size2; ++i) {
            testarr2[i] = rand() % minValue - minValue / 2;
        }
        for (; i - size2 < size1; ++i) {
            testarr1[i - size2] = rand() % minValue  - minValue / 2;
        }
        std::sort(testarr1, testarr1 + size1);
        std::sort(testarr2, testarr2 + size2);
        int*  standart = new int[size1 + size2];
        int*  test = new int[size1 + size2];
        i = 0;
        for (; i < size1; ++i) {
            test[i] = testarr1[i];
            standart[i] = testarr1[i];
        }
        for (; i - size1 < size2; ++i) {
            test[i] = testarr2[i - size1];
            standart[i] = testarr2[i - size1];
        }
        bool correct = true;
        
        std::inplace_merge(standart, standart + size1, standart + size1 + size2, std::less<int>());
        inplaceMerge(test, test + size1, test + size1 + size2, std::less<int>());
        for (int i = 0; i < size1 + size2; ++i) {
            if (test[i] != standart[i]) {
                std::cout << "FALSE " << size1 << " " << size2 << std::endl;
                maxSize = maxSize < std::max(size1, size2) ? std::max(size1, size2) : maxSize;
                correct = false;
                break;
            }
        }
        if (correct) {
            //std::cout << "TRUE " << size1 << " " << size2 << std::endl;
        } else {
            for (int j = 0; j < size1 + size2; ++j) {
                std::cout << standart[j] << " ";
            }
            std::cout << std::endl;
            for (int j = 0; j < size1 + size2; ++j) {
                std::cout << test[j] << " ";
            }
            std::cout << std::endl;
        }
        //std::cout << std::endl;
    }
    std::cout << maxSize << std::endl;
}

void testInsertSort () {
    std::srand(int(std::time(NULL)));
    int minValue = 10000;
    for (int y = 0; y < 10000; ++y) {
        int size = rand() % 10000 + 1;
        int* test = new int[size];
        int* standart = new int[size];
        for (int j = 0; j < size; ++j) {
            test[j] = rand() % minValue - minValue / 2;
            standart[j] = test[j];
        }
        
        bool correct = true;
        
        std::sort(standart, standart + size);
        insertSort(test, test + size);
        
        for (int j = 0; j < size; ++j) {
            if (test[j] != standart[j]) {
                std::cout << "FALSE" << std::endl;
                correct = false;
                break;
            }
        }
        if (correct) {
        } else {
            for (int j = 0; j < size; ++j) {
                std::cout << standart[j] << " ";
            }
            std::cout << std::endl;
            for (int j = 0; j < size; ++j) {
                std::cout << test[j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

void testMergeSame () {
    std::srand(int(std::time(NULL)));
    int minValue = 3;
    int maxSize = 0;
    for (int y = 0; y < 10000; ++y) {
        int size = rand() % 10000;
        int* testarr = new int[2 * size];
        for (int i = 0; i < size; ++i) {
            testarr[i] = rand() % minValue - minValue / 2;
        }
        for (int i = 0; i < size; ++i) {
            testarr[size + i] = rand() % minValue  - minValue / 2;
        }
        std::sort(testarr, testarr + 2 * size);
        int*  standart = new int[2 * size];
        int*  test = new int[2 * size];
        for (int i = 0; i < 2 * size; ++i) {
            test[i] = testarr[i];
            standart[i] = testarr[i];
        }
        bool correct = true;
        
        std::inplace_merge(standart, standart + size, standart + 2 * size, std::less<int>());
        inplaceMerge(test, test + size, test + 2 * size, std::less<int>());
        for (int i = 0; i < 2 * size; ++i) {
            if (test[i] != standart[i]) {
                std::cout << "FALSE " << size << " " << size << std::endl;
                //maxSize = maxSize < std::max(size1, size2) ? std::max(size, size2) : maxSize;
                correct = false;
                break;
            }
        }
        /*if (correct) {
            //std::cout << "TRUE " << size1 << " " << size2 << std::endl;
        } else {
            for (int j = 0; j < 2 * size; ++j) {
                std::cout << standart[j] << " ";
            }
            std::cout << std::endl;
            for (int j = 0; j < 2 * size; ++j) {
                std::cout << test[j] << " ";
            }
            std::cout << std::endl;
        //}
        std::cout << std::endl;
    }
    std::cout << maxSize << std::endl;
}*/

int main() {
    //testMerge();
    //testMergeSame();
    testTimSort();
}
