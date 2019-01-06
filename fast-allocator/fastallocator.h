//
// Created by Shepelev Alexey on 03.05.17.
//

#ifndef F_PROBLEM_FASTALLOCATOR_H
#define F_PROBLEM_FASTALLOCATOR_H

#include <memory>
#include <utility>
#include <new>



const std::size_t PULL_SIZE = 2048;

template<std::size_t chunkSize>
class FixedAllocator {

private:

    class Stack {

    public:

        void* first;
        std::size_t size;

        void setEmpty() {
            first = nullptr;
            size = 0;
        }

        ~Stack() = default;

        Stack() : first(nullptr), size(0) {}

        Stack& operator=(Stack&& other) {
            size = other.size;
            first = other.first;
            other.first = nullptr;
            other.size = 0;
            return *this;
        }

        Stack(Stack&& other) :
                first(other.first),
                size(other.size) {
            other.first = nullptr;
            other.size = 0;
        }

        void* pop_front() {
            if (!size)
                return nullptr;
            void* deleted = first;
            first = *reinterpret_cast<void**>(deleted);
            --size;
            return deleted;
        }

        void push_front(void* ptr) {
            ++size;
            *reinterpret_cast<void**>(ptr) = first;
            first = ptr;
        }
    };

    Stack freeCells;
    Stack pools;

public:

    ~FixedAllocator() {
        for (std::size_t i = 0; i < pools.size;  ++i) {
            operator delete(pools.pop_front());
        }
    }

    FixedAllocator() {
        createPool();
    }

    FixedAllocator(const FixedAllocator& other) = delete;

    FixedAllocator& operator=(const FixedAllocator& other)  = delete;

    FixedAllocator(FixedAllocator&& other) :
            freeCells(std::move(other.freeCells)),
            pools(std::move(other.pools)) {}

    FixedAllocator& operator=(FixedAllocator&& other) {
        freeCells = std::move(other.freeCells);
        pools = std::move(other.pools);
        return *this;
    }

    void createPool() {
        void* newPool = operator new(chunkSize * PULL_SIZE);
        pools.push_front(newPool);
        char* endPtr = reinterpret_cast<char*>(newPool) + PULL_SIZE * chunkSize;
        for (char* i = reinterpret_cast<char*>(newPool) + chunkSize; i != endPtr; i += chunkSize) {
            freeCells.push_front(reinterpret_cast<void*>(i));
        }
    }

    void* allocate() {
        if (!freeCells.size)
            createPool();
        return freeCells.pop_front();
    }

    void deallocate(void* ptr) {
        freeCells.push_front(ptr);
    }
};

enum FixedSizes {
    FS_First = 4,
    FS_Second = 8,
    FS_Third = 24
};

class CoolAllocator {
protected:
    static FixedAllocator<FS_First> fixedAllocFirst;
    static FixedAllocator<FS_Second> fixedAllocSecond;
    static FixedAllocator<FS_Third> fixedAllocThird;
};


FixedAllocator<FS_First> CoolAllocator::fixedAllocFirst = FixedAllocator<FS_First>();
FixedAllocator<FS_Second> CoolAllocator::fixedAllocSecond = FixedAllocator<FS_Second>();
FixedAllocator<FS_Third> CoolAllocator::fixedAllocThird = FixedAllocator<FS_Third>();


template<typename T>
class FastAllocator : CoolAllocator {

    int typeSize;

public:

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    typedef std::size_t size_type;

    template<typename U>
    struct rebind {
        typedef FastAllocator<U> other;
    };

    int getTypeSize() const {
        return typeSize;
    }

    ~FastAllocator() = default;

    FastAllocator() :
            typeSize(sizeof(T)) {
    }

    template<typename U>
    FastAllocator(const FastAllocator<U>& other) :
            FastAllocator() {
        typeSize = other.getTypeSize();
    }

    FastAllocator(const FastAllocator& other) :
            FastAllocator() {
        typeSize = other.getTypeSize();
    }

    FastAllocator(FastAllocator&& other) :
            FastAllocator() {
        typeSize = other.getTypeSize();
    }

    FastAllocator& operator=(const FastAllocator& other) {
        typeSize = other.typeSize;
        return *this;
    }

    FastAllocator& operator=(FastAllocator&& other) {
        typeSize = other.typeSize;
        return *this;
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

    void deallocate(pointer ptr, std::size_t n = 1) {
        if (typeSize == FS_First && n == 1) {
            return fixedAllocFirst.deallocate(reinterpret_cast<void*>(ptr));
        } else if (typeSize == FS_Second && n == 1) {
            return fixedAllocSecond.deallocate(reinterpret_cast<void*>(ptr));
        } else if (typeSize == FS_Third && n == 1) {
            return fixedAllocThird.deallocate(reinterpret_cast<void*>(ptr));
        } else {
            delete ptr;
        }
    }

    template<class U, class... Args>
    void construct(U* ptr, Args&& ... args) {
        new(reinterpret_cast<void*>(ptr)) U(std::forward<Args>(args)...);
    };

    template<class U>
    void destroy(U* ptr) {
        ptr->~U();
    }

};

template<typename T, typename Allocator = std::allocator<T> >
class List {

    class Node {

    public:

        T value_;
        Node* next_;
        Node* previous_;

        ~Node() = default;

        Node(T value = T(), Node* next = nullptr, Node* previous = nullptr) :
                value_(value),
                next_(next),
                previous_(previous) {}
    };

    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using size_type = std::size_t;

    Node* front_;
    Node* back_;
    size_t size_;
    NodeAlloc alloc_;

    void setDefault() {
        size_ = 0;
        front_ = nullptr;
        back_ = nullptr;
        alloc_ = std::move(NodeAlloc());
    }

public:

    ~List() {
        while (size_) {
            pop_back();
        }
    }

    explicit List(const Allocator& alloc = NodeAlloc()) :
            front_(nullptr),
            back_(nullptr),
            size_(0),
            alloc_(alloc) {
    }

    List(size_type count, const T& value = T(), const Allocator& alloc = NodeAlloc()) :
            List(alloc) {
        for (auto i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    List(const List& other) : List() {
        *this = other;
    }

    List(List&& other) :
            front_(other.front_),
            back_(other.back_),
            size_(other.size_),
            alloc_(std::move(other.alloc_)) {
        other.setDefault();
    }

    List& operator=(List&& other) {
        if (this == &other)
            return *this;
        while (size_) {
            pop_back();
        }
        size_ = other.size_;
        front_ = other.front_;
        back_ = other.back_;
        alloc_ = std::move(other.alloc_);

        other.setDefault();
        return *this;
    }

    List& operator=(const List& other) {
        if (this == &other)
            return *this;
        while (size_) {
            pop_back();
        }
        for (auto i = other.front_; i != nullptr; i = i->next_) {
            push_back(i->value_);
        }
        return *this;
    }

    size_t size() const {
        return size_;
    }

    void push_back(const T& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, value, nullptr, back_);
        if (back_ != nullptr)
            back_->next_ = newNode;
        back_ = newNode;
        if (front_ == nullptr)
            front_ = newNode;
        ++size_;
    }

    void push_back(T&& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, std::move(value), nullptr, back_);
        if (back_ != nullptr)
            back_->next_ = newNode;
        back_ = newNode;
        if (front_ == nullptr)
            front_ = newNode;
        ++size_;
    }

    void push_front(T& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, value, front_, nullptr);
        if (front_ != nullptr)
            front_->previous_ = newNode;
        front_ = newNode;
        if (back_ == nullptr)
            back_ = newNode;
        ++size_;
    }

    void push_front(T&& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, std::move(value), front_, nullptr);
        if (front_ != nullptr)
            front_->previous_ = newNode;
        front_ = newNode;
        if (back_ == nullptr)
            back_ = newNode;
        ++size_;
    }

    void pop_back() {
        if (!size_)
            return;
        Node* deleted = back_;
        back_ = back_->previous_;
        alloc_.destroy(deleted);
        alloc_.deallocate(deleted, 1);
        --size_;
    }

    void pop_front() {
        if (!size_)
            return;
        Node* deleted = front_;
        front_ = front_->next_;
        alloc_.destroy(deleted);
        alloc_.deallocate(deleted, 1);
        --size_;
    }

    void erase(Node* deleted) {
        if (!size_)
            return;
        --size_;
        if (deleted == front_) {
            pop_front();
            return;
        }
        if (deleted == back_) {
            pop_back();
            return;
        }
        deleted->next_->previous_ = deleted->previous_;
        deleted->previous_->next_ = deleted->next_;
        alloc_.destroy(deleted);
        alloc_.deallocate(deleted, 1);
    }

    void insert_after(Node* precedingNode, const T& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, value, precedingNode->next_, precedingNode);
        precedingNode->next_ = newNode;
        if (newNode->next_ != nullptr)
            newNode->next_->previous_ = newNode;
        ++size_;
    }

    void insert_after(Node* precedingNode, T&& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, std::move(value), precedingNode->next_, precedingNode);
        precedingNode->next_ = newNode;
        if (newNode->next_ != nullptr)
            newNode->next_->previous_ = newNode;
        ++size_;
    }

    void insert_before(Node* succeedingNode, const T& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, value, succeedingNode, succeedingNode->previous_);
        succeedingNode->previous_ = newNode;
        if (newNode->previous_ != nullptr)
            newNode->previous_->next_ = newNode;
        ++size_;
    }

    void insert_before(Node* succeedingNode, T&& value) {
        Node* newNode = alloc_.allocate(1);
        alloc_.construct(newNode, std::move(value), succeedingNode, succeedingNode->previous_);
        succeedingNode->previous_ = newNode;
        if (newNode->previous_ != nullptr)
            newNode->previous_->next_ = newNode;
        ++size_;
    }
};

#endif //F_PROBLEM_FASTALLOCATOR_H