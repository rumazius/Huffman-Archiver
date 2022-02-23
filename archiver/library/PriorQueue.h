#pragma once

#include <vector>

template<typename T>
class PriorQueue {
private:
    std::vector<T> elements_;

    void ShiftUp(size_t i) {
        if (i == 0 ||  elements_[(i - 1) / 2] < elements_[i]) {
            return;
        }
        std::swap(elements_[i], elements_[(i - 1) / 2]);
        ShiftUp((i - 1) / 2);
    }

    void ShiftDown(size_t i) {
        if (i * 2 + 1 >= elements_.size()) {
            return;
        }
        if (elements_[i] < elements_[i * 2 + 1] &&
            (elements_.size() == i * 2 + 2 || elements_[i] < elements_[i * 2 + 2])) {
            return;
        }

        if (elements_.size() == i * 2 + 2 || elements_[i * 2 + 1] < elements_[i * 2 + 2]) {
            std::swap(elements_[i], elements_[i * 2 + 1]);
            ShiftDown(i * 2 + 1);
        } else {
            std::swap(elements_[i], elements_[i * 2 + 2]);
            ShiftDown(i * 2 + 2);
        }
    }

public:

    void Push(T x) {
        elements_.push_back(x);
        ShiftUp(elements_.size() - 1);
    }

    void Pop() {
        std::swap(elements_[0], elements_.back());
        elements_.pop_back();
        ShiftDown(0);
    }

    bool Empty() {
        return elements_.empty();
    }

    size_t Size() {
        return elements_.size();
    }

    T Top() {
        return elements_[0];
    }
};