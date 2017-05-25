#ifndef deque_h
#define deque_h

#include <iterator>
#include <algorithm>
#include <iostream>


class Point {
private:
    
    double x, y, z;
    
public:
    
    ~Point () {}
    
    Point () {};
    
    Point (double a, double b, double c) {
        x = a;
        y = b;
        z = c;
    }
};

template<typename Type>
class DequeRAIt: public std::iterator<std::random_access_iterator_tag, Type> {
private:
    
    Type* _pointer;
    Type* _begin;
    Type* _end;
    
public:
    
    DequeRAIt () {}
    
    DequeRAIt (Type* pointer, Type* begin, Type* end) {
        _pointer = pointer;
        _begin = begin;
        _end = end;
    }
    
    DequeRAIt (const DequeRAIt& another) {
        _pointer = another._pointer;
        _begin = another._begin;
        _end = another._end;
    }
    
    DequeRAIt& operator ++ () {
        _pointer = _pointer == _end - 1 ? _begin : ++_pointer;
        return *this;
    }
    
    DequeRAIt operator ++ (int) {
        DequeRAIt ans = *this;
        ++*this;
        return ans;
    }
    
    DequeRAIt& operator += (size_t n) {
        _pointer = _begin + (_pointer - _begin + n) % (_end - _begin);
        return  *this;
    }
    
    DequeRAIt operator + (size_t n) {
        DequeRAIt ans = *this;
        ans += n;
        return ans;
    }
    
    DequeRAIt& operator -- () {
        _pointer = _pointer == _begin ? _end - 1 : --_pointer;
        return *this;
    }
    
    DequeRAIt operator -- (int) {
        DequeRAIt ans = *this;
        --*this;
        return ans;
    }
    
    DequeRAIt& operator -= (size_t n) {
        _pointer = _begin + (_pointer - _begin + _end - _begin - n) % (_end - _begin);
        return *this;
    }
    
    DequeRAIt operator - (size_t n) {
        DequeRAIt ans = *this;
        ans -= n;
        return ans;
    }
    
    Type& operator * () {
        return *_pointer;
    }
    
    bool operator == (const DequeRAIt& another) const {
        return (_pointer == another._pointer) && (_end == another._end) && (_begin == another._begin);
    }
    
    bool operator != (const DequeRAIt& another) const {
        return !(*this == another);
    }
    
    bool operator < (const DequeRAIt& another) const {
        if (_begin < _pointer && _begin < another._pointer) {
            return _pointer < another._pointer;
        } else {
            return _pointer > another._pointer;
        }
    }
    
    size_t operator - (const DequeRAIt& another) const {
        if (_begin < _pointer && _begin < another._pointer) {
            return _pointer - another._pointer;
        } else {
            return _begin < _pointer ? (_end - _begin) - (_pointer - another._pointer) : (_end - _begin) - (another._pointer - _pointer);
        }
    }
};

template <typename Type>
class Deque {
    
    
    Type* array;
    size_t size, elementsNumber;
    DequeRAIt<Type> begin;
    DequeRAIt<Type> last;
    
    
public:
    
    ~Deque () {
        delete[] array;
    }
    
    Deque () {
        size = 1;
        elementsNumber = 0;
        array = new Type[size];
        begin = DequeRAIt<Type>(array, array, array + size);
        last = DequeRAIt<Type>(array + elementsNumber, array, array + size);
    }
    
    Deque (const Deque& another) {
        delete[] array;
        array = new Type[another.size];
        size = another.size;
        elementsNumber = another.elementsNumber;
        begin = DequeRAIt<Type>(array, array, array + size);
        last = DequeRAIt<Type>(array, array + size, array + size);
        std::copy(another.begin, another.last, begin);
    }
    
    void increaseArray () {
        Type* newArray = new Type[2 * size];
        std::copy(begin, begin + size, newArray);
        last = DequeRAIt<Type>(newArray + size, newArray, newArray + 2 * size);
        begin = DequeRAIt<Type>(newArray, newArray, newArray + 2 * size);
        delete [] array;
        array = newArray;
        size *= 2;
    }
    
    void push_back (const Type& element) {
        if (elementsNumber == size) {
            increaseArray();
            *last = element;
            ++last;
            ++elementsNumber;
        } else {
            *last = element;
            ++last;
            ++elementsNumber;
        }
    }
    
    void push_front (const Type& element) {
        if (elementsNumber == size) {
            increaseArray();
            *begin = element;
            --begin;
            ++elementsNumber;
        } else {
            *begin = element;
            --begin;
            ++elementsNumber;
        }
    }
    
    void pop_back () {
        --last;
        --elementsNumber;
    }
    
    void pop_front () {
        ++begin;
        --elementsNumber;
    }
    
    void print () {
        for (int i = 0; i < last - begin; ++i) {
            std::cout << *(begin + i) << " ";
        }
        /*for (int i = 0; i < size; ++i) {
            std::cout << array[i] << " ";
        }*/
        std::cout << std::endl;
    }
    
    void movelast () {
        ++last;
    }
};
#endif /* deque_h */
