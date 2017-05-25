#ifndef partition_h
#define partition_h

#include "iTimSortParams.h"

template <typename RandomAccessIterator, typename Compare = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>>
std::vector<std::pair<RandomAccessIterator, RandomAccessIterator>> splitToRuns (RandomAccessIterator first, RandomAccessIterator last, Compare comp = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>(), const ITimSortParams& params = defaultParams()) {
    const size_t minRunLength = params.minRun(last - first);
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type ValueType;
    std::vector<std::pair<RandomAccessIterator, RandomAccessIterator>> stack;
    for (RandomAccessIterator i = first; i < last;) {
        std::pair<RandomAccessIterator, RandomAccessIterator> currentPair = std::make_pair(i, i + 1);
        bool increaseValues = comp(*(currentPair.first), *(currentPair.second));
        while ((currentPair.second - currentPair.first <= minRunLength || (comp(*(currentPair.second - 1), *currentPair.second) == increaseValues)) && last - currentPair.second > 0) {
            if (comp(*(currentPair.second - 1), *currentPair.second) != increaseValues) break;
            ++currentPair.second;
        }
        if (currentPair.second - currentPair.first < minRunLength && last >= currentPair.first + minRunLength) {
            currentPair.second = currentPair.first + minRunLength;
        }
        if (!increaseValues && currentPair.second - currentPair.first > minRunLength) {
            std::reverse(currentPair.first, currentPair.second);
        }
        i += currentPair.second - currentPair.first;
        stack.push_back(currentPair);
    }
    return stack;
}
#endif /* partition_h */
