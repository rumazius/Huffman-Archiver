#pragma once

#include <vector>

template<typename T>
class PriorQueue {
private:
    std::vector<T> elements_;

    void ShiftUp(size_t i);

    void ShiftDown(size_t i);

public:

    void Push(T x);

    void Pop();

    bool Empty();

    size_t Size();

    T Top();
};