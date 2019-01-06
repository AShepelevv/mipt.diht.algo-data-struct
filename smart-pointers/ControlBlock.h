//
// Created by Shepelev Alexey on 26.04.17.
//

#ifndef G_PROBLEM_CONTROLBLOCK_H
#define G_PROBLEM_CONTROLBLOCK_H

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

#endif //G_PROBLEM_CONTROLBLOCK_H
