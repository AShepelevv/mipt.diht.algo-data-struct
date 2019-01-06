//
// Created by Shepelev Alexey on 03.05.17.
//

#ifndef G_PROBLEM_SMARTPOINTERS_H
#define G_PROBLEM_SMARTPOINTERS_H

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
        if (ptr_ != nullptr)
            delete ptr_;
        ptr_ = ptr;
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*    SWAP    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    void swap(UniquePtr& other) {
        std::swap(ptr_, other.ptr_);
    }
};

class ControlBlock {

    size_t weakCount_;
    size_t sharedCount_;
    bool isExpired_;

public:

    ~ControlBlock() = default;

    ControlBlock(size_t sharedCount, size_t weakCount) :
            weakCount_(weakCount),
            sharedCount_(sharedCount),
            isExpired_(false) {}

    size_t getWeakCountPrt() const {
        return weakCount_;
    }

    size_t getSharedCountPrt() const {
        return sharedCount_;
    }

    void increaseWeakCount() {
        ++weakCount_;
    }

    void decreaseWeakCount() {
        --weakCount_;
    }

    void increaseSharedCount() {
        ++sharedCount_;
    }

    void decreaseSharedCount() {
        --sharedCount_;
    }

    template<typename T>
    void decreaseSharedCountAndSetExpiration(T* ptr) {
        decreaseSharedCount();
        if (!sharedCount_) {
            if (ptr != nullptr)
                delete ptr;
            setExpiration();
        }
    }

    bool isEmpty() const {
        return sharedCount_ == 0 && weakCount_ == 0;
    }

    bool isExpired() const {
        return sharedCount_ == 0;
    }

    void setExpiration(bool value = true) {
        isExpired_ = value;
    }

};

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
        if (*this == other)
            return;
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
        ptr_ = reinterpret_cast<T*>(other.ptr_);
        control_ = other.control_;
        control_->increaseSharedCount();
        return *this;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        control_->decreaseSharedCountAndSetExpiration<T>(ptr_);
        if (control_->isEmpty())
            delete control_;
        ptr_ = other.ptr_;
        control_ = other.control_;
        control_->increaseSharedCount();
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

#endif //G_PROBLEM_SMARTPOINTERS_H
