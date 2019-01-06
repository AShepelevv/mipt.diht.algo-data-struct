#ifndef comparator_h
#define comparator_h

#include "point.h"

template <typename  T> struct ComparatorLess : std::binary_function <T, T, bool> {
    bool operator() (const T& firstValue, const T& secondValue) const {
        return firstValue < secondValue;
    }
};

template <typename  T> struct ComparatorGreater : std::binary_function <T, T, bool> {
    bool operator() (const T& firstValue, const T& secondValue) const {
        return firstValue > secondValue;
    }
};

template <typename  T, typename RandomAccessIterator> struct ComparatorSortArrays : std::binary_function <T, T, bool> {
    bool operator() (RandomAccessIterator firstBegin, RandomAccessIterator firstEnd, RandomAccessIterator secondBegin, RandomAccessIterator secondEnd) const {
        return firstBegin < secondBegin || (firstBegin == secondBegin && firstEnd <= secondEnd);
    }
};


#endif /* comparator_h */
