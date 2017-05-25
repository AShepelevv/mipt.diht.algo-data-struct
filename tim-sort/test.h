#ifndef test_h
#define test_h

#include "point.h"
#include "timSort.h"

const size_t minSize = 10;
const size_t maxSize = 1000000;
const size_t relationSize = 10;
const int maxValue = 10000;

const int mis = 1000000 / CLOCKS_PER_SEC;

template <typename T>
void testTimSortVectorRandom () {
    std::srand(int(std::time(0)));
    for (size_t i = minSize; i <= maxSize; i *= relationSize) {
        std::vector<T> testVector; //= createRandomTestVector<T>(i);
        for (size_t j = 0; j < i; ++j) {
            testVector.push_back(rand() % (2 * maxValue) - maxValue);
        }
        std::vector<T> standartVector = testVector;
        std::cout << "Number of elements: " << i << std::endl;
        int time = int(clock());
        std::sort(standartVector.begin(), standartVector.end(), ComparatorLess<T>());
        std::cout << "std::sort TIME: " << (clock() - time) * mis << " microsec" << std::endl;
        time = int(clock());
        timSort(testVector.begin(), testVector.end(), ComparatorLess<T>());
        std::cout << "timSort TIME:   " << (clock() - time) * mis << " microsec" << std::endl;
        if (testVector == standartVector) {
            std::cout << "Verdict OK" << std::endl;
        } else {
            std::cout << "Verdict: WA" << std::endl;
        }
        std::cout << std::endl;
    }
}

template <typename T>
void testTimSortArrayRandom () {
    std::cout << "ARRAY<INT>" << std::endl;
    std::srand(int(std::time(0)));
    for (size_t i = minSize; i <= maxSize; i *= relationSize) {
        int* testArray = new T[i];
        for (size_t j = 0; j < i; ++j) {
            testArray[j] = rand() % (2 * maxValue) - maxValue;
            
        }
        int* standartArray = new T[i];
        std::copy(testArray, testArray + i, standartArray);
        std::cout << "Number of elements: " << i << std::endl;
        int time = int(clock());
        std::sort(standartArray, standartArray + i, ComparatorLess<T>());
        std::cout << "std::sort TIME: " << (clock() - time) * mis << " microsec" << std::endl;
        time = int(clock());
        timSort(testArray, testArray + i, ComparatorLess<T>());
        std::cout << "timSort TIME:   " << (clock() - time) * mis << " microsec" << std::endl;
        bool verdict = true;
        for (size_t j = 0; j < i; ++j) {
            if (testArray[j] != standartArray[j]) {
                verdict = false;
                break;
            }
        }
        if (verdict) {
            std::cout << "Verdict OK" << std::endl;
        } else {
            std::cout << "Verdict: WA" << std::endl;
        }
        std::cout << std::endl;
    }
}

template <typename T>
void testTimSortArrayOrdered () {
    std::cout << "ORDERED" << std::endl;
    std::srand(int(std::time(0)));
    std::vector<int> orderedLength = {20, 40, 80, 128, 1024};
    for (int y = 0; y < orderedLength.size(); ++y) {
        std::cout << "ORDERED " << orderedLength[y] << std::endl;
        for (size_t i = minSize; i <= maxSize; i *= relationSize) {
            int* testArray = new int[i];
            for (size_t j = 0; j < i; ++j) {
                testArray[j] = int(j % (orderedLength[y])) - orderedLength[y] / 2;
            }
            int* standartArray = new int[i];
            std::copy(testArray, testArray + i, standartArray);
            std::cout << "Number of elements: " << i << std::endl;
            int time = int(clock());
            std::sort(standartArray, standartArray + i, ComparatorLess<T>());
            std::cout << "std::sort TIME: " << (clock() - time) * mis << " microsec" << std::endl;
            time = int(clock());
            timSort(testArray, testArray + i, ComparatorLess<T>());
            std::cout << "timSort TIME:   " << (clock() - time) * mis << " microsec" << std::endl;
            bool verdict = true;
            for (size_t j = 0; j < i; ++j) {
                if (testArray[j] != standartArray[j]) {
                    verdict = false;
                    break;
                }
            }
            if (verdict) {
                std::cout << "Verdict OK" << std::endl;
            } else {
                std::cout << "Verdict: WA" << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

void testTimSortPoint () {
    std::cout << "POINT" << std::endl;
    std::srand(int(std::time(0)));
    for (size_t i = minSize; i <= maxSize; i *= relationSize) {
        Point* testArray = new Point[i];
        for (size_t j = 0; j < i; ++j) {
            testArray[j] = Point(2.1 * (rand() % (2 * maxValue)) - maxValue, 2.1 * (rand() % (2 * maxValue)) - maxValue, 2.1 * (rand() % (2 * maxValue)) - maxValue);
        }
        Point* standartArray = new Point[i];
        std::copy(testArray, testArray + i, standartArray);
        std::cout << "Number of elements: " << i << std::endl;
        int time = int(clock());
        std::sort(standartArray, standartArray + i, ComparatorLess<Point>());
        std::cout << "std::sort TIME: " << (clock() - time) * mis << " microsec" << std::endl;
        time = int(clock());
        timSort(testArray, testArray + i, ComparatorLess<Point>());
        std::cout << "timSort TIME:   " << (clock() - time) * mis << " microsec" << std::endl;
        bool verdict = true;
        for (size_t j = 0; j < i; ++j) {
            if (testArray[j] != standartArray[j]) {
                verdict = false;
                break;
            }
        }
        if (verdict) {
            std::cout << "Verdict OK" << std::endl;
        } else {
            std::cout << "Verdict: WA" << std::endl;
        }
        std::cout << std::endl;
    }
}

void testTimSortString () {
    std::cout << "STRING" << std::endl;
    std::srand(int(std::time(0)));
    for (size_t i = minSize; i <= maxSize; i *= relationSize) {
        std::string* testArray = new std::string[i];
        for (size_t j = 0; j < i; ++j) {
            testArray[j] = std::to_string(rand() % (2 * maxValue) - maxValue);
        }
        std::string* standartArray = new std::string[i];
        std::copy(testArray, testArray + i, standartArray);
        std::cout << "Number of elements: " << i << std::endl;
        int time = int(clock());
        std::sort(standartArray, standartArray + i, ComparatorLess<std::string>());
        std::cout << "std::sort TIME: " << (clock() - time) * mis << " microsec" << std::endl;
        time = int(clock());
        timSort(testArray, testArray + i, ComparatorLess<std::string>());
        std::cout << "timSort TIME:   " << (clock() - time) * mis << " microsec" << std::endl;
        bool verdict = true;
        for (size_t j = 0; j < i; ++j) {
            if (testArray[j] != standartArray[j]) {
                verdict = false;
                break;
            }
        }
        if (verdict) {
            std::cout << "Verdict OK" << std::endl;
        } else {
            std::cout << "Verdict: WA" << std::endl;
        }
        std::cout << std::endl;
    }
}

void testTimSort () {
    testTimSortVectorRandom<int>();
    testTimSortArrayRandom<int>();
    testTimSortArrayOrdered<int>();
    testTimSortPoint();
    testTimSortString();
}



#endif /* test_h */
