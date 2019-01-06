//
// Created by Shepelev Alexey on 20.04.17.
//

#ifndef F_PROBLEM_LIST_H
#define F_PROBLEM_LIST_H

#include <iostream>
#include <memory>
#include <utility>

template<typename T, typename Allocator = std::allocator<T>>
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

    Node* front_;
    Node* back_;
    size_t size_;
    NodeAlloc alloc_;


    //using node_alloc_ = typename Allocator::template rebind<Node>::other;

    void setDefault() {
        size_ = 0;
        front_ = nullptr;
        back_ = nullptr;
        alloc_ = std::move(NodeAlloc());
    }

public:

    //***********************************    Destructor    ***********************************//

    ~List() {
        while (size_) {
            pop_back();
        }
    }

    //******************************    Default constructor    *******************************//

    explicit List(const Allocator& alloc = Allocator()) :
            front_(nullptr),
            back_(nullptr),
            size_(0) {}

    //****************************    Count copies constructor    ****************************//

    List(size_t count, const T& value = T(), const Allocator& alloc = Allocator()) : List() {
        for (auto i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    //********************************    Copy constructor    ********************************//

    List(const List& other) : List() {
        *this = other;
    }

    //********************************    Move constructor    ********************************//

    List(List&& other) :
            front_(other.front_),
            back_(other.back_),
            size_(other.size_),
            alloc_(std::move(other.alloc_)) {
        other.setDefault();
    }

    //****************************    Move assignment operator    ****************************//

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

    //****************************    Copy assignment operator    ****************************//

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

    //**************************************    SIZE    **************************************//

    size_t size() const {
        return size_;
    }

    //************************************    PUSH_BACK    ***********************************//

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

    //***********************************    PUSH_FRONT    ***********************************//

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

    //***************************************    POP    **************************************//

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

    //**************************************    ERASE    *************************************//

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

    //***********************************    INSERT_AFTER    *********************************//

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

    //**********************************    INSERT_BEFORE    *********************************//

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


#endif //F_PROBLEM_LIST_H
