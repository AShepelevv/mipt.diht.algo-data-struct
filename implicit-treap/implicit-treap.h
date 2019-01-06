//
// Created by Shepelev Alexey on 18.03.17.
//

#ifndef UNTITLED_IMPLICITTREAP_H
#define UNTITLED_IMPLICITTREAP_H

#include "node.h"

template<typename ValueType>
class ImplicitTreap {

    bool isNeededToDelete_;

    enum Parts {
        P_Left,
        P_Segment,
        P_Right
    } partName;

public:

    Node<ValueType>* root_;

    ~ImplicitTreap() {
        if (isNeededToDelete_)
            deleteNode(root_);
    }

    ImplicitTreap(Node<ValueType>* root = nullptr, bool isNeededToDelete = true) :
            root_(root),
            isNeededToDelete_(isNeededToDelete) {}

    ImplicitTreap& operator=(ImplicitTreap& another) {
        root_ = another.root_;
        return *this;
    }

    void insert(ValueType key, std::size_t position) {
        auto tmp = split(root_, position);
        Node<ValueType>* newNode = new Node<ValueType>;
        *newNode = Node<ValueType>(key, rand());
        tmp.first = merge(tmp.first, newNode);
        root_ = merge(tmp.first, tmp.second);
    }

    void remove(std::size_t position) {
        auto parts = getParts(position, position);
        delete parts.at(P_Segment);
        root_ = merge(parts.at(P_Left), parts.at(P_Right));
    }

    std::vector<Node<ValueType>*> getParts(std::size_t begin, std::size_t end) {
        std::vector<Node<ValueType>*> threeParts;
        auto tmp = split(root_, begin);
        threeParts.push_back(tmp.first);
        tmp = split(tmp.second, end + 1 - begin);
        threeParts.push_back(tmp.first);
        threeParts.push_back(tmp.second);
        return threeParts;
    }

    void mergeParts(std::vector<Node<ValueType>*> parts) {
        for (auto i = 1; i < parts.size(); ++i) {
            parts.front() = merge(parts.front(), parts.at(i));
        }
        root_ = parts.front();
    }

    void reverse(std::size_t begin, std::size_t end) {
        auto parts = getParts(begin, end);
        parts.at(P_Segment)->isRev_ ^= true;
        mergeParts(parts);
    }

    void addValue(std::size_t begin, std::size_t end, ValueType addValue) {
        auto parts = getParts(begin, end);
        parts.at(P_Segment)->updateMarks();
        parts.at(P_Segment)->isAddition_ = true;
        parts.at(P_Segment)->addValue_ = addValue;
        mergeParts(parts);
    }

    void assignValue(std::size_t begin, std::size_t end, ValueType assignedValue) {
        auto parts = getParts(begin, end);
        parts.at(P_Segment)->updateMarks();
        parts.at(P_Segment)->isAssignment_ = true;
        parts.at(P_Segment)->assignedValue_ = assignedValue;
        mergeParts(parts);
    }

    ValueType getSum(std::size_t begin, std::size_t end) {
        auto parts = getParts(begin, end);
        parts.at(P_Segment)->updateMarks();
        ValueType sum = parts.at(P_Segment)->sum();
        mergeParts(parts);
        return sum;
    }

    template<typename GetLastElem, typename GetNode>
    void anotherPermutation(std::size_t begin, std::size_t end, GetLastElem getLast, GetNode getNode) {
        auto parts = getParts(begin, end);
        std::size_t lastEI = getLast(parts.at(P_Segment));
        if (lastEI == 0) {
            parts.at(P_Segment)->isRev_ ^= true;
        } else {
            auto partsOfSegment = split(parts.at(P_Segment), lastEI);
            partsOfSegment.first->updateAll();
            std::size_t breakNodeInd = partsOfSegment.first->size() - 1;
            ValueType breakNodeKey = partsOfSegment.first->endRightValue_;
            IndAndValue<ValueType> nodeToChange = getNode(breakNodeKey, partsOfSegment.second);
            ImplicitTreap leftPart(partsOfSegment.first, false);
            leftPart.assignValue(breakNodeInd, breakNodeInd, nodeToChange.value);
            ImplicitTreap rightPart(partsOfSegment.second, false);
            rightPart.assignValue(nodeToChange.index, nodeToChange.index, breakNodeKey);
            rightPart.root_->isRev_ ^= true;
            parts.at(P_Segment) = merge(leftPart.root_, rightPart.root_);
        }
        mergeParts(parts);
    }

    void nextPermutationOfSubsegment(std::size_t begin, std::size_t end) {
        anotherPermutation(begin, end, getlastDesElInd, getIndAndValueleastNode);
    }

    void prevPermutationOfSubsegment(std::size_t begin, std::size_t end) {
        anotherPermutation(begin, end, getlastAscElInd, getIndAndValuegreatestNode);
    }

    void print() {
        printKeys(root_);
    }
};

#endif //UNTITLED_IMPLICITTREAP_H