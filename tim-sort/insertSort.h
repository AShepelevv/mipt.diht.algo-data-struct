#ifndef insertSort_h
#define insertSort_h

template <typename RandomAccessIterator, typename Compare = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>>
void insertSort (RandomAccessIterator first, RandomAccessIterator last, Compare comp = ComparatorLess<typename std::iterator_traits<RandomAccessIterator>::value_type>()) {
    for (RandomAccessIterator i = first + 1; i < last; ++i) {
        if (comp(*i, *(i - 1))) {
            typename std::iterator_traits<RandomAccessIterator>::value_type copy = *i;
            for (RandomAccessIterator j = i - 1; j >= first; --j) {
                if (comp(*j, copy)) {
                    *(j + 1) = copy;
                    break;
                }
                if (j == first) {
                    *(j + 1) = *j;
                    *first = copy;
                    break;
                }
                *(j + 1) = *j;
            }
        }
    }
}

#endif /* insertSort_h */
