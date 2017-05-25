#ifndef inplaceMerge_h
#define inplaceMerge_h

#include <cmath>
#include <iostream>
#include "comparator.h"
#include "insertSort.h"

const size_t minMerge = 64;

template <typename RandomAccessIterator, typename Compare = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void swapMerge (RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, RandomAccessIterator buffer, Compare comp = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>()) {
    RandomAccessIterator bufferPointer = buffer, middlePointer = middle, firstPointer = first;
    for (RandomAccessIterator i = first; i < middle; ++i) {
        std::swap(*i, *(bufferPointer++));
    }
    bufferPointer = buffer;
    for (size_t i = 0; i < last - first; ++i) {
        if (bufferPointer < buffer + (last - middle) && middlePointer < last) {
            !comp(*bufferPointer, *middlePointer) ? std::swap(*(middlePointer++), *(firstPointer++)) : std::swap(*(bufferPointer++), *(firstPointer++));
        } else {
            middlePointer < last ? std::swap(*(middlePointer++), *(firstPointer++)) :  std::swap(*(bufferPointer++), *(firstPointer++));
        }
    }
}

template <typename RandomAccessIterator, typename Compare = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void inplaceMerge (RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>()) {
    
    if (last - middle == 0 || middle - first == 0) {
        return;
    }
    
    if (last - first < minMerge) {
        insertSort(first, last, comp);
        return;
    }
    
    size_t runsLength = std::ceil(sqrt(last - first));
    
    if (last - middle <= 2 * runsLength) {
        size_t secondSize = last - middle;
        for (RandomAccessIterator i = middle - secondSize; i >= first + secondSize; i -= secondSize) {
            swapMerge(i, i + secondSize, i + 2 * secondSize, first);
        }
        insertSort(first, first + 3 * secondSize);
        return;
    }
    
    std::pair<RandomAccessIterator, RandomAccessIterator> midPiece = std::make_pair(first + (middle - first) / runsLength * runsLength, first + (middle - first) / runsLength * runsLength + runsLength);
    std::pair<RandomAccessIterator, RandomAccessIterator> endPiece = std::make_pair(first + (last - first) / runsLength * runsLength, last);
    
    for (size_t i = 0; i < runsLength; ++i) {
        std::swap(*(midPiece.first + i), *(endPiece.first - runsLength + i));
    }
    
    midPiece = std::make_pair(endPiece.first - runsLength, endPiece.first);
    
    for (RandomAccessIterator i = first; i <= midPiece.first - runsLength; i += runsLength) {
        RandomAccessIterator indexMin = i;
        for (RandomAccessIterator j = i; j <= midPiece.first - runsLength; j += runsLength) {
            if (comp(*j, *indexMin)) {
                indexMin = j;
            }
        }
        for (size_t y = 0; y < runsLength; ++y) {
            std::swap(indexMin[y], i[y]);
        }
    }
    
    for (RandomAccessIterator i = first + 2 * runsLength; i <= midPiece.first; i += runsLength) {
        swapMerge(i - 2 * runsLength, i - runsLength, i, midPiece.first);
    }
    insertSort(midPiece.first, last, comp); //insertSort(midPiece.first - 2 * runsLength, last, comp);
    
    insertSort(midPiece.first - (last - midPiece.first), last, comp);
    
    
    for (RandomAccessIterator i = midPiece.first - 2 * (last - midPiece.first); i >= first; i -= (last - midPiece.first)) {

        swapMerge(i, i + (last - midPiece.first), i + 2 * (last - midPiece.first), midPiece.first, comp);
    }
    insertSort(midPiece.first - 2 * runsLength, last, comp);
    insertSort(first, first + 2 * (last - midPiece.first), comp);
}
#endif /* inplaceMerge_h */
