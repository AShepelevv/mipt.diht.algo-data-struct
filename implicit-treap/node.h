//
// Created by Shepelev Alexey on 18.03.17.
//

#ifndef UNTITLED_NODE_H
#define UNTITLED_NODE_H

#include <utility>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>

template<typename ValueType>
class IndAndValue {
public:
    std::size_t index;
    ValueType value;

    IndAndValue(std::size_t new_index, ValueType new_value) :
            index(new_index), value(new_value) {};
};

template<typename ValueType>
class Node;

auto isDescending = [](auto* ptr_) { return ptr_->isDescending_; };

auto isAscending = [](auto* ptr_) { return ptr_->isAscending_; };

auto getlastDesElInd = [](auto* ptr_) { return ptr_->lastDescendingElemIndex(); };

auto getlastAscElInd = [](auto* ptr_) { return ptr_->lastAscendingElemIndex(); };

auto getIndAndValueleastNode = [](auto key, auto* ptr_) { return ptr_->leastNodeGreaterSet(key); };

auto getIndAndValuegreatestNode = [](auto key, auto* ptr_) { return ptr_->greatestNodeLessSet(key); };

template<typename ValueType>
class Node {

    template<typename Compare>
    void ifOneNull(Node<ValueType>* ptr, Compare comp, ValueType endValue) {
        ptr->updateAll();
        if (!ptr->isSorted())
            return;
        if (ptr->isAscending_ && !comp(key_, endValue))
            isAscending_ = true;
        if (ptr->isDescending_ && !comp(endValue, key_))
            isDescending_ = true;
    }

public:

    ValueType key_;
    long long prior_;
    Node<ValueType>* left_ = nullptr;
    Node<ValueType>* right_ = nullptr;
    std::size_t sizeOfSubTreap_ = 1;
    std::size_t subTreapSum_ = ValueType();
    bool isRev_ = false;
    ValueType addValue_ = ValueType();
    ValueType assignedValue_ = ValueType();
    bool isAddition_ = false;
    bool isAssignment_ = false;
    bool isDescending_ = true;
    bool isAscending_ = true;
    ValueType endLeftValue_ = ValueType();
    ValueType endRightValue_ = ValueType();

    Node(ValueType key = ValueType(),
         long long prior = rand()) :
            key_(key),
            prior_(prior) {
        updateSize();
        updateSum();
        setPoints();
        setSorted();
    }

    std::size_t leftSize() const {
        return left_ == nullptr ? 0 : left_->sizeOfSubTreap_;
    }

    std::size_t rightSize() const {
        return right_ == nullptr ? 0 : right_->sizeOfSubTreap_;
    }

    ValueType leftSum() const {
        return left_ == nullptr ? 0 : left_->subTreapSum_;
    }

    ValueType rightSum() const {
        return right_ == nullptr ? 0 : right_->subTreapSum_;
    }

    ValueType sum() const {
        return subTreapSum_;
    }

    void updateSum() {
        subTreapSum_ = leftSum() + rightSum() + key_;
        subTreapSum_ += isAddition_ ? addValue_ * size() : 0;
        subTreapSum_ = isAssignment_ ? assignedValue_ * size() : subTreapSum_;
    }

    std::size_t size() const {
        return sizeOfSubTreap_;
    }

    void updateSize() {
        sizeOfSubTreap_ = 1 + rightSize() + leftSize();
    }

    void setPoints() {
        if (left_ != nullptr)
            left_->updateMarks();
        if (right_ != nullptr)
            right_->updateMarks();
        endLeftValue_ = left_ == nullptr ? key_ : left_->endLeftValue_;
        endRightValue_ = right_ == nullptr ? key_ : right_->endRightValue_;
    }

    bool isSorted() const {
        return isAscending_ || isDescending_;
    }

    void setSorted() {
        isAscending_ = false;
        isDescending_ = false;
        if (left_ == nullptr && right_ == nullptr) {
            isAscending_ = true;
            isDescending_ = true;
            return;
        }
        if (left_ == nullptr) {
            ifOneNull(right_, std::greater<ValueType>(), right_->endLeftValue_);
            return;
        }
        if (right_ == nullptr) {
            ifOneNull(left_, std::less<ValueType>(), left_->endRightValue_);
            return;
        }
        right_->updateMarks();
        left_->updateMarks();

        if (left_->isAscending_ != right_->isAscending_ &&
            left_->isDescending_ != right_->isDescending_)
            return;

        if (left_->isAscending_ && right_->isAscending_ &&
            left_->endRightValue_ <= key_ && right_->endLeftValue_ >= key_)
            isAscending_ = true;

        if (left_->isDescending_ && right_->isDescending_ &&
            left_->endRightValue_ >= key_ && right_->endLeftValue_ <= key_)
            isDescending_ = true;
    }

    void pushReversion() {
        if (isRev_) {
            std::swap(isAscending_, isDescending_);
            std::swap(endLeftValue_, endRightValue_);
            std::swap(left_, right_);
            isRev_ = false;
            if (left_ != nullptr)
                left_->isRev_ ^= true;
            if (right_ != nullptr)
                right_->isRev_ ^= true;
        }
    }

    void pushAddition(Node<ValueType>* child) {
        if (child != nullptr) {
            if (child->isAddition_)
                child->addValue_ += addValue_;
            else if (child->isAssignment_)
                child->assignedValue_ += addValue_;
            else {
                child->isAddition_ = true;
                child->addValue_ = addValue_;
            }
            child->updateSum();
        }
    }

    void pushAssigment(Node<ValueType>* child) {
        if (child != nullptr) {
            if (child->isAddition_) {
                child->isAddition_ = false;
                child->isAssignment_ = true;
            } else {
                child->isAssignment_ = true;
            }
            child->assignedValue_ = assignedValue_;
            child->updateSum();
        }
    }

    void updateMarks() {
        pushReversion();
        if (isAddition_) {
            key_ += addValue_;
            endLeftValue_ += addValue_;
            endRightValue_ += addValue_;
            isAddition_ = false;
            pushAddition(left_);
            pushAddition(right_);
        } else if (isAssignment_) {
            key_ = assignedValue_;
            endLeftValue_ = assignedValue_;
            endRightValue_ = assignedValue_;
            isAssignment_ = false;
            isDescending_ = true;
            isAscending_ = true;
            pushAssigment(left_);
            pushAssigment(right_);
        }
        updateSum();
    }

    void updateParams() {
        updateSize();
        setPoints();
        setSorted();
    }

    void updateAll() {
        updateMarks();
        updateParams();
    }

    template<typename Compare, typename Direction>
    std::size_t lastElemIndex(Direction isDirection, Compare comp) {
        updateAll();
        if (isDirection(this) || size() == 1)
            return 0;
        if (right_ == nullptr) {
            left_->updateAll();
            if (comp(key_, left_->endRightValue_))
                return leftSize();
            return left_->lastElemIndex(isDirection, comp);
        }
        if (isDirection(right_)) {
            right_->updateAll();
            if (comp(right_->endLeftValue_, key_))
                return leftSize() + 1;
            left_->updateAll();
            if (comp(key_, left_->endRightValue_))
                return leftSize();
            return left_->lastElemIndex(isDirection, comp);
        }
        right_->updateAll();
        return right_->lastElemIndex(isDirection, comp) + leftSize() + 1;
    }

    std::size_t lastDescendingElemIndex() {
        return lastElemIndex(isDescending, std::greater<ValueType>());
    }

    std::size_t lastAscendingElemIndex() {
        return lastElemIndex(isAscending, std::less<ValueType>());
    }

    template<typename Compare>
    IndAndValue<ValueType> someNode(ValueType setValue, Compare comp) {
        updateAll();
        if (!comp(key_, setValue)) {
            if (left_ == nullptr)
                return IndAndValue<ValueType>(leftSize(), key_);
            left_->updateAll();
            return left_->someNode(setValue, comp);
        }
        if (right_ == nullptr)
            return IndAndValue<ValueType>(leftSize(), key_);
        right_->updateAll();
        if (!comp(right_->endLeftValue_, setValue))
            return IndAndValue<ValueType>(leftSize(), key_);
        auto tmp = right_->someNode(setValue, comp);
        tmp.index += leftSize() + 1;
        return tmp;
    };

    IndAndValue<ValueType> leastNodeGreaterSet(ValueType setValue) {
        return someNode(setValue, std::greater<ValueType>());
    }

    IndAndValue<ValueType> greatestNodeLessSet(ValueType setValue) {
        return someNode(setValue, std::less<ValueType>());
    }

};

template<typename ValueType>
void printKeys(Node<ValueType>* root) {
    if (root == nullptr)
        return;
    root->updateAll();
    printKeys(root->left_);
    std::cout << root->key_ << " ";
    printKeys(root->right_);

}

template<typename ValueType>
std::pair<Node<ValueType>*, Node<ValueType>*> split(Node<ValueType>* root, std::size_t index) {
    if (root == nullptr)
        return std::pair<Node<ValueType>*, Node<ValueType>*>(nullptr, nullptr);
    root->updateMarks();
    if (index == root->leftSize()) {
        Node<ValueType>* tmp = root->left_;
        root->left_ = nullptr;
        root->updateAll();
        return std::pair<Node<ValueType>*, Node<ValueType>*>(tmp, root);
    }
    if (root->leftSize() < index) {
        auto tmp = split(root->right_, index - root->leftSize() - 1);
        root->right_ = tmp.first;
        root->updateAll();
        return std::pair<Node<ValueType>*, Node<ValueType>*>(root, tmp.second);
    } else {
        auto tmp = split(root->left_, index);
        root->left_ = tmp.second;
        root->updateAll();
        return std::pair<Node<ValueType>*, Node<ValueType>*>(tmp.first, root);
    }
}

template<typename ValueType>
void additionInMerge(Node<ValueType>* firstTreap, Node<ValueType>* secondTreap) {
    firstTreap->subTreapSum_ += secondTreap->sum();
    firstTreap->sizeOfSubTreap_ += secondTreap->size();
}

template<typename ValueType>
Node<ValueType>* merge(Node<ValueType>* leftTreap, Node<ValueType>* rightTreap) {
    if (leftTreap == nullptr) {
        return rightTreap;
    }
    if (rightTreap == nullptr) {
        return leftTreap;
    }
    leftTreap->updateMarks();
    rightTreap->updateMarks();
    Node<ValueType>* mergeResult;
    if (leftTreap->prior_ > rightTreap->prior_) {
        additionInMerge(leftTreap, rightTreap);
        leftTreap->endRightValue_ = rightTreap->endRightValue_;
        leftTreap->right_ = merge(leftTreap->right_, rightTreap);
        mergeResult = leftTreap;
    } else {
        additionInMerge(rightTreap, leftTreap);
        rightTreap->endLeftValue_ = leftTreap->endLeftValue_;
        rightTreap->left_ = merge(leftTreap, rightTreap->left_);
        mergeResult = rightTreap;
    }
    mergeResult->setSorted();
    return mergeResult;
}


template<typename ValueType>
void deleteNode(Node<ValueType>* root) {
    if (root == nullptr)
        return;
    deleteNode(root->left_);
    deleteNode(root->right_);
    delete root;
}

#endif //UNTITLED_NODE_H