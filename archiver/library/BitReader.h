#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>

class BitReader {
private:
    std::istream& in_stream_;

    size_t buffer_size_ = 0;
    size_t buffer_ = 0;

    void ReadSymbol() {
        char cur_ch;
        in_stream_.read(&cur_ch, 1);
        if (in_stream_.gcount() == 0) {
            std::cerr << "Bad file!";
            throw;
        }
        buffer_ <<= 8;
        buffer_ |= cur_ch;
        buffer_size_ += 8;
    }


public:

    BitReader(std::istream& is) : in_stream_(is), buffer_size_(0), buffer_(0) {
    }

    size_t Get(size_t bit_number) {
        size_t res = 0;
        for (size_t i = 0; i < bit_number; ++i) {
            if (buffer_size_ == 0) {
                ReadSymbol();
            }

            bool cur_bit = (buffer_ & (1 << (buffer_size_ - 1))) > 0;
            --buffer_size_;
            res |= (static_cast<char>(cur_bit) << i);
        }
        return res;
    }
};
