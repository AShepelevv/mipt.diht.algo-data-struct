//
// Created by Shepelev Alexey on 23.04.17.
//

#ifndef G_PROBLEM_WEAKPTR_H
#define G_PROBLEM_WEAKPTR_H

#include "SharedPtr.h"
#include "ControlBlock.h"
#include <memory>
#include <utility>
#include <new>

template<typename T>
class SharedPtr;

template<typename T>
class WeakPtr {

    T* ptr_;
    ControlBlock* control_;

public:

    ControlBlock* getControl() {
        return control_;
    }

    T* getPtr() const {
        return ptr_;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    Destructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    ~WeakPtr() {
        control_->decreaseWeakCount();
        if (control_->isEmpty())
            delete control_;
    };

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Default constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    WeakPtr(T* ptr = nullptr, ControlBlock* control = new ControlBlock(0, 1)) :
            ptr_(ptr),
            control_(control) {}

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    Shared constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    WeakPtr(const SharedPtr<U>& sPrt) :
            ptr_(sPrt.getPtr()),
            control_(sPrt.getControl()) {
        control_->increaseWeakCount();
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Copy constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    WeakPtr(const WeakPtr<U>& ptr) :
            ptr_(ptr.ptr_),
            control_(ptr.control_) {
        control_->increaseWeakCount();
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Move constructor    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    WeakPtr(WeakPtr<U>&& ptr) :
            ptr_(ptr.ptr_),
            control_(ptr.control_) {
        ptr.control_ = new ControlBlock(0, 1);
        ptr.ptr_ = nullptr;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Copy assignment operator    %%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    WeakPtr& operator=(const WeakPtr& other) {
        ptr_ = other.ptr_;
        control_->decreaseWeakCount();
        if (control_->isEmpty())
            delete control_;
        control_ = other.control_;
        control_->increaseWeakCount();
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%    Move assignment operator    %%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    WeakPtr& operator=(WeakPtr&& other) {
        ptr_ = reinterpret_cast<T*>(other.ptr_);
        control_->decreaseWeakCount();
        if (control_->isEmpty())
            delete control_;
        control_ = other.control_;
        other.control_ = new ControlBlock(0, 1);
        other.ptr_ = nullptr;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%*    Shared assignment operator    %%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    WeakPtr& operator=(const SharedPtr<U>& other) {
        ptr_ = reinterpret_cast<T*>(other.getPtr());
        control_->decreaseWeakCount();
        if (control_->isEmpty())
            delete control_;
        control_ = other.getControl();
        control_->increaseWeakCount();
        return *this;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    EXPIRED    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    bool expired() const {
        return control_->isExpired();
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    USE_COUNT    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    size_t use_count() const {
        return control_->getSharedCountPrt();
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    LOCK    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    SharedPtr<T> lock() const {
        return expired() ? SharedPtr<T>(nullptr, control_) : SharedPtr<T>(ptr_, control_);
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    RESET    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    template<typename U>
    void reset(U* ptr = nullptr) {
        if (reinterpret_cast<T*>(ptr) == ptr_)
            return;
        control_->decreaseWeakCount();
        if (control_->isEmpty())
            delete control_;
        ptr_ = reinterpret_cast<T*>(ptr);
        control_ = ControlBlock(0, 1);
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    SWAP    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    void swap(WeakPtr& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(control_, other.control_);
    }
};

#endif //G_PROBLEM_WEAKPTR_H
