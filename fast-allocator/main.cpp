#include <chrono>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <iostream>

#include "fastallocator.h"

using namespace std::chrono;

template <class List>
int test_list(List&& l) {
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 4000000; ++i) {
        l.push_back(i);
    }
    for (int i = 0; i < 2500000; ++i) {
        l.pop_front();
    }
    for (int i = 0; i < 1000000; ++i) {
        l.push_front(i);
    }
    for (int i = 0; i < 2500000; ++i) {
        l.pop_back();
    }
    auto finish = high_resolution_clock::now();
    return duration_cast<milliseconds>(finish - start).count();
}

template <class T, class Alloc>
void test_vector(std::vector<T, Alloc>&& v) {
    // auto start = high_resolution_clock::now();
    v.reserve(5000000);
    v.push_back(1);
    // std::cout << v.front();
    v.resize(2500000);
    // std::cout << v.back();
    v.pop_back();
    v.shrink_to_fit();
    for (int i = 0; i < 5000000; ++i) {
        v.push_back(i);
    }
    for (int i = 0; i < 5000000; ++i) {
        v.pop_back();
    }
    // auto finish = high_resolution_clock::now();
    // std::cout << duration_cast<milliseconds>(finish - start).count() << "ms" << std::endl;
}


int main() {
    auto first = test_list(std::list<int>());
    std::cout << "list: " << first << std::endl;
    auto second = test_list(std::list<int, FastAllocator<int>>());
    std::cout << "list: " << second << std::endl;
    if (first < second) {
        throw std::runtime_error("std::list with FastAllocator expected to be faster than with std::allocator, but there "
                                         "were " + std::to_string(second) + " ms instead of " + std::to_string(first) + "...\n");
    }
    test_vector(std::vector<char>());
    test_vector(std::vector<char, FastAllocator<char>>());
    first = test_list(List<int>());
    std::cout << "list: " << first << std::endl;
    second = test_list(List<int, FastAllocator<int>>());
    std::cout << "list: " << second << std::endl;
    if (first < second) {
        throw std::runtime_error("Custom List with FastAllocator expected to be faster than with std::allocator, but there "
                                         "were " + std::to_string(second) + " milliseconds instead of " + std::to_string(first) + "...\n");
    }
    std::cout << 0;

}