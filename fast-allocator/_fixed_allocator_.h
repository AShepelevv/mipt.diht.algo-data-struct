//
// Created by Shepelev Alexey on 10.04.17.
//

#ifndef F_PROBLEM_FIXEDALLOCATOR_H
#define F_PROBLEM_FIXEDALLOCATOR_H

#include <new>
#include <utility>


const size_t PULL_SIZE = 2048;

template<typename T>
class StackElement {
public:

    T thisValue;
    StackElement<T>* next;

    ~StackElement() = default;

    StackElement() = default;

    StackElement<T>(T value, StackElement<T>* pointer) {
        thisValue = value;
        next = pointer;
    }
};

template<typename T>
class Stack {

public:

    StackElement<T>* first;
    size_t size;

    ~Stack() {
        while (size) {
            --size;
            auto del = first->next;
            delete first;
            first = del;
        }
    }

    Stack<T>() : first(nullptr), size(0) {}

    Stack& operator=(Stack&& other) {
        size = other.size;
        first = other.first;
        other.first = nullptr;
        other.size = 0;
        return *this;
    }

    Stack(Stack&& other) : size(other.size), first(other.first) {
        other.first = nullptr;
        other.size = 0;
    }

    StackElement<T>* pop_front() {
        if (!size)
            return nullptr;
        StackElement<T>* deleted = first;
        first = deleted->next;
        --size;
        return deleted;
    }

    void pop_front(bool t) {
        if (!size)
            return;
        StackElement<T>* deleted = first;
        first = deleted->next;
        delete deleted;
        --size;
    }


    void push_front(const T& value, StackElement<T>* newElement) {
        ++size;
        newElement->next = first;
        newElement->thisValue = value;
        first = newElement;
    }

    void push_front(const T& value) {
        ++size;
        StackElement<T>* newElement = new StackElement<T>(value, first);
        first = newElement;
    }
};

template<size_t chunkSize>
class FixedAllocator {

private:

    Stack<void*> freeCells;
    Stack<void*> bookedCells;
    Stack<void*> pulls;

public:

    ~FixedAllocator() {
        for (auto ptr = pulls.first; ptr != nullptr; ptr = ptr->next) {
            operator delete(ptr->thisValue, chunkSize * PULL_SIZE);
        }
    }

    FixedAllocator() {
        createPull();
    }

    FixedAllocator(FixedAllocator&& other) :
            freeCells(std::move(other.freeCells)),
            bookedCells(std::move(other.bookedCells)),
            pulls(std::move(other.pulls)) {
        other.pulls = std::move(Stack<void*>());
        other.bookedCells = std::move(Stack<void*>());
        other.freeCells = std::move(Stack<void*>());
    }

    FixedAllocator& operator=(FixedAllocator&& other) {
        freeCells = std::move(other.freeCells);
        bookedCells = std::move(other.bookedCells);
        pulls = std::move(other.pulls);
        other.pulls = std::move(Stack<void*>());
        other.bookedCells = std::move(Stack<void*>());
        other.freeCells = std::move(Stack<void*>());
        return *this;
    }

    void createPull() {
        void* newPull = operator new(chunkSize * PULL_SIZE);
        pulls.push_front(newPull);
        char* endPtr = reinterpret_cast<char*>(newPull) + PULL_SIZE * chunkSize;
        for (char* i = reinterpret_cast<char*>(newPull); i != endPtr; i += chunkSize) {
            freeCells.push_front(reinterpret_cast<void*>(i));
        }
    }

    void* allocate() {
        if (!freeCells.size)
            createPull();
        bookedCells.push_front(freeCells.first->thisValue, freeCells.pop_front());
        return bookedCells.first->thisValue;
    }

    void deallocate(void* ptr) {
        freeCells.push_front(bookedCells.first->thisValue, bookedCells.pop_front());
    }
};

#endif //F_PROBLEM_FIXEDALLOCATOR_H
