//
// Created by Shepelev Alexey on 23.04.17.
//

#ifndef G_PROBLEM_SHAREDPTR_H
#define G_PROBLEM_SHAREDPTR_H

#include "WeakPtr.h"
#include "ControlBlock.h"
#include <new>
#include <utility>
#include <memory>


template<typename T>
class WeakPtr;

template<typename T>
class SharedPtr {

    T* ptr_;
    ControlBlock* control_;

public:

    ControlBlock* getControl() const {
        return control_;
    }

    T* getPtr() const {
        return ptr_;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Destructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    ~SharedPtr() {
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Default constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    SharedPtr(T* ptr = nullptr, ControlBlock* control = new ControlBlock(0, 0)) :
            ptr_(ptr),
            control_(control) {
        control->increaseSharedCount();
        //std::cout << 0 << std::endl;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Copy constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    SharedPtr(const SharedPtr<U>& other) :
            ptr_(reinterpret_cast<T*>(other.ptr_)),
            control_(other.control_) {
        control_->increaseSharedCount();
        //std::cout << 1 << std::endl;
    }

    SharedPtr(const SharedPtr& other) :
            ptr_(other.ptr_),
            control_(other.control_) {
        control_->increaseSharedCount();
        //std::cout << 10 << std::endl;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Move constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    SharedPtr(SharedPtr<U>&& other) :
            ptr_(reinterpret_cast<T*>(other.ptr_)),
            control_(other.control_) {
        other.control_ = new ControlBlock(1, 0);
        other.ptr_ = nullptr;
        //std::cout << 2 << std::endl;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Weak constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    SharedPtr(const WeakPtr<U>& other) :
            ptr_(reinterpret_cast<T*>(other.getPtr())),
            control_(other.getControl()) {
        control_->increaseSharedCount();
        control_->setExpiration(false);
        control_->decreaseWeakCount();
        //std::cout << 3 << std::endl;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Copy assignment operator    %%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    SharedPtr& operator=(const SharedPtr<U>& other) {
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
        ptr_ = reinterpret_cast<T*>(other.ptr_);
        control_ = other.control_;
        control_->increaseSharedCount();
        //std::cout << 4 << std::endl;
        return *this;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
        ptr_ = other.ptr_;
        control_ = other.control_;
        control_->increaseSharedCount();
        //std::cout << 40 << std::endl;
        return *this;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Move assignment operator    %%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    SharedPtr& operator=(SharedPtr<U>&& other) {
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
        ptr_ = reinterpret_cast<T*>(other.ptr_);
        control_ = other.control_;
        other.ptr_ = nullptr;
        other.control_ = new ControlBlock(1, 0);
        //std::cout << 5 << std::endl;
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

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    GET    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    T* get() const {
        return ptr_;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    USE_COUNT    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    size_t use_count() const {
        return control_->getSharedCountPrt();
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    RESET    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    void reset() {
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
        control_ = new ControlBlock(1, 0);
        ptr_ = nullptr;

    }

    template<typename U>
    void reset(U* ptr = nullptr) {
        if (reinterpret_cast<T*>(ptr) == ptr_)
            return;
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
        control_ = new ControlBlock(1, 0);
        ptr_ = reinterpret_cast<T*>(ptr);
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    SWAP    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    void swap(SharedPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(control_, other.control_);
    }

};

#endif //G_PROBLEM_SHAREDPTR_H
