//
// Created by Shepelev Alexey on 19.04.17.
//

#ifndef F_PROBLEM_TEST_H
#define F_PROBLEM_TEST_H

#include "fastallocator.h"
#include <ctime>
#include <list>
#include <memory>
#include <iostream>

const double MICROSEC = 1000000;

template<typename AllocType = std::allocator<int>>
void testAlloc(int cyclesNumber, int elementNumber) {
    List<int, AllocType> testList;
    for (int j = 0; j < cyclesNumber; ++j) {
        for (int i = 0; i < elementNumber; ++i) {
            testList.push_front(i);
        }
        for (int i = 0; i < elementNumber; ++i) {
            testList.pop_front();
        }
    }
}

void testINT(int cyclesNumber, int elementNumber) {
    std::cout << " MY: ";
    time_t testTime = clock();
    testAlloc<FastAllocator<int>>(cyclesNumber, elementNumber);
    std::cout << (clock() - testTime) / MICROSEC << std::endl;

    std::cout << "STL: ";
    testTime = clock();
    testAlloc(cyclesNumber, elementNumber);
    std::cout << (clock() - testTime) / MICROSEC << std::endl;
}

#endif //F_PROBLEM_TEST_H
