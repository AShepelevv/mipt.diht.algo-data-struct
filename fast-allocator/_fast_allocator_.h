//
// Created by Shepelev Alexey on 11.04.17.
//

#ifndef F_PROBLEM_FASTALLOCATOR_H
#define F_PROBLEM_FASTALLOCATOR_H

#include <new>
#include <utility>
#include <iostream>
#include "_fixed_allocator_.h"

template<typename T>
class FastAllocator {

    enum FixedSizes {
        FS_First = 4,
        FS_Second = 8,
        FS_Third = 24
    };

    int typeSize = sizeof(T);
    FixedAllocator<FS_First> fixedAllocFirst;
    FixedAllocator<FS_Second> fixedAllocSecond;
    FixedAllocator<FS_Third> fixedAllocThird;

public:

    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;

    ~FastAllocator() = default;

    FastAllocator() :
            fixedAllocFirst(std::move(FixedAllocator<FS_First>())),
            fixedAllocSecond(std::move(FixedAllocator<FS_Second>())),
            fixedAllocThird(std::move(FixedAllocator<FS_Third>())) {
    }

    template<typename U>
    FastAllocator(const FastAllocator<U>& other) :
            fixedAllocFirst(std::move(FixedAllocator<FS_First>())),
            fixedAllocSecond(std::move(FixedAllocator<FS_Second>())),
            fixedAllocThird(std::move(FixedAllocator<FS_Third>())),
            typeSize(sizeof(U)) {
    }

    FastAllocator(FastAllocator&& other) :
            fixedAllocFirst(std::move(other.fixedAllocFirst)),
            fixedAllocSecond(std::move(other.fixedAllocSecond)),
            fixedAllocThird(std::move(other.fixedAllocThird)) {
        other.fixedAllocFirst = std::move(FixedAllocator<FS_First>());
        other.fixedAllocSecond = std::move(FixedAllocator<FS_Second>());
        other.fixedAllocThird = std::move(FixedAllocator<FS_Third>());

    }

    FastAllocator& operator=(const FastAllocator& other) {
        fixedAllocFirst = std::move(FixedAllocator<FS_First>());
        fixedAllocSecond = std::move(FixedAllocator<FS_Second>());
        fixedAllocThird = std::move(FixedAllocator<FS_Third>());
        return *this;
    }

    FastAllocator& operator=(FastAllocator&& other) {
        fixedAllocFirst = std::move(other.fixedAllocFirst);
        fixedAllocSecond = std::move(other.fixedAllocSecond);
        fixedAllocThird = std::move(other.fixedAllocThird);
        other.fixedAllocFirst = std::move(FixedAllocator<FS_First>());
        other.fixedAllocSecond = std::move(FixedAllocator<FS_Second>());
        other.fixedAllocThird = std::move(FixedAllocator<FS_Third>());
    }

    pointer allocate(size_type n = 1) {
        if (typeSize == FS_First && n == 1) {
            return reinterpret_cast<pointer>(fixedAllocFirst.allocate());
        } else if (typeSize == FS_Second && n == 1) {
            return reinterpret_cast<pointer>(fixedAllocSecond.allocate());
        } else if (typeSize == FS_Third && n == 1) {
            return reinterpret_cast<pointer>(fixedAllocThird.allocate());
        } else {
            return reinterpret_cast<pointer>(operator new(typeSize * n));
        }
    }

    void deallocate(pointer ptr, size_type n) {
        if (typeSize == FS_First && n == 1) {
            return fixedAllocFirst.deallocate(ptr);
        } else if (typeSize == FS_Second && n == 1) {
            return fixedAllocSecond.deallocate(ptr);
        } else if (typeSize == FS_Third && n == 1) {
            return fixedAllocThird.deallocate(ptr);
        } else {
            operator delete(ptr, typeSize * n);
        }
    }

    void construct(pointer p, const_reference val) {
        new(reinterpret_cast<void*>(p)) T(val);
    }

    template<class U, class... Args>
    void construct(U* ptr, Args&& ... args) {
        new(reinterpret_cast<void*>(ptr)) U(std::forward<Args>(args)...);
    };

    void destroy(pointer p) {
        p->~T();
    }

    template<class U>
    void destroy(U* p) {
        p->~U();
    }

};

#endif //F_PROBLEM_FASTALLOCATOR_H