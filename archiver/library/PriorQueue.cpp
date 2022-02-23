#include "PriorQueue.h"
#include <vector>

template<class T>
void PriorQueue<T>::Push(T x) {
    elements_.push_back(x);
    ShiftUp(elements_.size() - 1);
}

template<class T>
void PriorQueue<T>::Pop() {
    std::swap(elements_[0], elements_.back());
    elements_.pop_back();
    ShiftDown(0);
}

template<class T>
bool PriorQueue<T>::Empty() {
    return elements_.empty();
}

template<class T>
size_t PriorQueue<T>::Size() {
    return elements_.size();
}


template <typename T>
T PriorQueue<T>::Top() {
    return elements_[0];
}

template<class T>
void PriorQueue<T>::ShiftUp(size_t i) {
    if (i == 0 ||  elements_[(i - 1) / 2] < elements_[i]) {
        return;
    }
    std::swap(elements_[i], elements_[(i - 1) / 2]);
    ShiftUp((i - 1) / 2);
}

template<class T>
void PriorQueue<T>::ShiftDown(size_t i) {
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
