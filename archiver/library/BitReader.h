#pragma once

#include <iostream>
#include <fstream>

class BitReader {
private:
    std::istream& in_stream_;

    size_t buffer_size_ = 0;
    size_t buffer_ = 0;

    void ReadSymbol();

public:

    BitReader(std::istream& is) : in_stream_(is), buffer_size_(0), buffer_(0) {
    }

    size_t GetChar(size_t bit_number);
};
