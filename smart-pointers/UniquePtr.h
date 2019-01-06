//
// Created by Shepelev Alexey on 23.04.17.
//

#ifndef G_PROBLEM_UNIQUEPTR_H
#define G_PROBLEM_UNIQUEPTR_H

#include <utility>
#include <memory>
#include <new>

template<typename T>
class UniquePtr {
    T* ptr_;
public:

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    Destructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    ~UniquePtr() {
        reset();
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Default constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    UniquePtr(T* ptr = nullptr) :
            ptr_(ptr) {}

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Copy constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    UniquePtr(const UniquePtr& other) = delete;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Move constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    UniquePtr(UniquePtr&& other) :
            ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    template <typename U>
    UniquePtr(UniquePtr<U>&& other) :
            ptr_(reinterpret_cast<T*>(other.ptr_)) {
        other.ptr_ = nullptr;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Copy assignment operator    %%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    UniquePtr& operator=(const UniquePtr&) = delete;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Move assignment operator    %%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    UniquePtr& operator=(UniquePtr&& other) {
        if (ptr_ != nullptr)
            delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    template <typename U>
    UniquePtr& operator=(UniquePtr<U>&& other) {
        if (ptr_ != nullptr)
            delete ptr_;
        ptr_ = reinterpret_cast<T*>(other.ptr_);
        other.ptr_ = nullptr;
        return *this;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    OPERATOR *    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    T& operator*() const {
        return *ptr_;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    OPERATOR ->    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    T* operator->() const {
        return ptr_;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    GET    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    T* get() const {
        return ptr_;
    }

    T* release() {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    RESET    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    void reset(T* ptr = nullptr) {
        if (ptr == ptr_)
            return;
        if (ptr_ != nullptr) {
            delete ptr_;
        }
        ptr_ = ptr;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    SWAP    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    void swap(UniquePtr& other) {
        std::swap(ptr_, other.ptr_);
    }
};

#endif //G_PROBLEM_UNIQUEPTR_H
