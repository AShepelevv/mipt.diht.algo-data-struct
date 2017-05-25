#include <iostream>
#include <iterator>

template <typename RAIter, typename Compare>
void mergeSort(RAIter first, RAIter last, Compare comp) {
    typedef typename std::iterator_traits<RAIter>::value_type value_type;
    value_type * buffer = new value_type[last - first];
    if (first + 1 >= last) {
        return;
    }
    value_type* middle = first + (last - first) / 2;
    mergeSort(first, middle, comp);
    mergeSort(middle, last, comp);
    value_type* left = first;
    value_type* right = middle;
    for (int i = 0; i < last - first; ++i) {
        if (left < middle && right < last) {
            buffer[i] = comp(*left, *right) ? *(left++) : *(right++);
        } else {
            buffer[i] = left >= middle ? *(right++) : *(left++);
        }
    }
    for (int i = 0; i < last - first; ++i) {
        first[i] = buffer[i];
    }
    delete [] buffer;
}

template <typename value_type>
class ListElement {
public:
    
    value_type thisValue;
    ListElement<value_type>* next;
    
    ListElement<value_type> () {
        next = nullptr;
        thisValue = value_type();
    }
    
    ListElement (value_type value, ListElement<value_type>* pointer) {
        thisValue = value;
        next = pointer;
    }
};


template<typename value_type>
class ConstListIter: public std::iterator<std::forward_iterator_tag, value_type> {
protected:
    
    ListElement<value_type>* pointer;
    
public:
    
    ~ConstListIter () {
    }
    
    ConstListIter (ListElement<value_type> *elementPointer) {
        pointer = elementPointer;
    }
    
    ConstListIter (const ConstListIter& another) {
        pointer = another.pointer;
    }
    
    const value_type& operator * () {
        return pointer->thisValue;
    }
    
    const value_type* operator -> () {
        return &pointer->thisValue;
    }
    
    ConstListIter operator ++ () {
        pointer = pointer->next;
        return *this;
    }
    
    ConstListIter operator ++ (int) {
        ConstListIter copy = *this;
        ++(*this);
        return copy;
    }
    
    ConstListIter operator + (int n) {
        ConstListIter copy = *this;
        for (size_t i = 0; i < n; ++i) {
            ++copy;
        }
        return copy;
    }
    
    bool operator == (const ConstListIter another) const {
        return pointer == another.pointer;
    }
    
    bool operator != (const ConstListIter another) const {
        return !(*this == another);
    }
    
    ListElement<value_type>& getElement () {
        return *pointer;
    }
};

template<typename value_type>
class ListIter: public ConstListIter<value_type> {
    
public:
    
    ListIter<value_type> (ListElement<value_type> * elementPointer) : ConstListIter<value_type>(elementPointer) {}
    
    value_type& operator * () {
        return ConstListIter<value_type>::pointer->thisValue;
    }
    
    value_type* operator -> () {
        return &ConstListIter<value_type>::pointer->thisValue;
    }

};

template <typename value_type, value_type default_value = 0>
class ForwardList {
    
    ListElement<value_type> *first;
    int listSize;
    
public:
    
    ~ForwardList () {
        for (int i = 0; i < listSize; ++i) {
            pop_front();
        }
    }
    
    ForwardList (int size = 0, const value_type& value = value_type()) : listSize(size) {
        if (size == 0) {
            first = nullptr;
            return;
        }
        first = new ListElement<value_type>;
        first->next = nullptr;
        first->thisValue = value;
        for (int i = 1; i < size; ++i) {
            ListElement<value_type> * newElement = new ListElement<value_type>;
            newElement->thisValue = value;
            newElement->next = first;
            first = newElement;
        }
    }
    
    ForwardList (const ForwardList& another) {
        listSize = 0;
        for (auto i = another.begin(); i != another.end(); ++i) {
            push_front(*i);
        }
    }
    
    void pop_front () {
        if (!listSize) {
            return;
        }
        ListElement<value_type> *deleted = first;
        first = deleted->next;
        delete deleted;
        --listSize;
    }
    
    void push_front (const value_type& value) {
        ++listSize;
        ListElement<value_type> *newElement = new ListElement<value_type>;
        newElement->thisValue = value;
        newElement->next = first;
        first = newElement;
    }
    
    void insertAfter (ConstListIter<value_type> iter, const value_type& value) {
        ListElement<value_type> *newElement = new ListElement<value_type>;
        newElement->thisValue = value;
        newElement->next = iter.getElement().next;
        iter.getElement().next = newElement;
        ++listSize;
    }
    
    template <typename InIter>
    void insertAfter (ConstListIter<value_type> iter, const InIter first, const InIter last) {
        for (InIter i = first; i != last; ++i) {
            insertAfter(iter, *i);
        }
    }
    
    void eraseAfter (ConstListIter<value_type> iter) {
        ListElement<value_type> * deleted = iter.getElement().next;
        if (deleted == nullptr) {
            return;
        }
        iter.getElement().next = iter.getElement().next->next;
        delete deleted;
        --listSize;
    }
    
    void eraseAfter (ConstListIter<value_type> iter, int number) {
        for (auto i = 0; i < number; ++i) {
            eraseAfter(iter);
        }
    }
    
    void eraseAfter (ConstListIter<value_type> iter, ConstListIter<value_type> last) {
        for (auto i = iter; i != last; ++i) {
            eraseAfter(iter);
        }
    }
    
    ListIter<value_type> begin () const {
        return ListIter<value_type>(first);
    }
    
    ListIter<value_type> end () const {
        return ListIter<value_type>(nullptr);
    }
    
    ConstListIter<value_type> cbegin () const {
        return ConstListIter<value_type>(first);
    }
    
    ConstListIter<value_type> cend () const {
        return ConstListIter<value_type>(nullptr);
    }
    
    void swap (const ForwardList<value_type>& another) {
        std::swap(listSize, another.listSize);
        std::swap(first, another.first);
    }
    
    void resize (int size, const value_type& value = value_type()) {
        int oldListSize = listSize;
        if (size < oldListSize) {
            for (auto i = 0; i < oldListSize - size; ++i) {
                pop_front();
            }
        } else {
            for (auto i = 0; i < size - oldListSize; ++i) {
                push_front(value);
            }
        }
    }
    
    int size () const {
        return listSize;
    }
    
    const value_type& front () const {
        return first->value;
    }
    
    bool empty () const {
        return !listSize;
    }
    
    void clear () {
        eraseAfter(cbegin(), cend());
        pop_front();
    }
    
    template <typename Comp = std::less<value_type>>
    void sort (Comp comp = std::less<value_type>()) {
        auto size = listSize;
        value_type * copyArray = new value_type [size];
        int pointer = 0;
        for (auto i = begin(); i != end(); ++i) {
            copyArray[pointer++] = *i;
        }
        clear();
        mergeSort(copyArray, copyArray + size, comp);
        pointer = size;
        for (auto i = 0; i < size; ++i) {
            push_front(copyArray[--pointer]);
        }
        delete [] copyArray;
    }
    
    void reverse () {
        ListElement<value_type> * firstP = &cbegin().getElement();
        ListElement<value_type> * secondP = cbegin().getElement().next;
        firstP->next = nullptr;
        ListElement<value_type> * thirdP = secondP->next;
        for (int i = 2; i < listSize; ++i) {
            secondP->next = firstP;
            firstP = secondP;
            secondP = thirdP;
            thirdP = thirdP->next;
            
        }
        secondP->next = firstP;
        first = secondP;
    }
    
};

int main () {
    
    ForwardList<int> l (0);
    for (int i = 0; i < 10; ++i) {
        l.push_front(i);
    }
    for (auto i = l.cbegin(); i != l.cend(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    l.reverse();
    l.push_front(7);
    l.sort();
    for (auto i = l.cbegin(); i != l.cend(); ++i) {
        std::cout << *i << " ";
    }
    
    std::cout << "Complete" << std::endl;
}
