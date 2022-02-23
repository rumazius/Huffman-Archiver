#pragma once

#include <iostream>
#include <fstream>

class BitWriter {
private:
    std::ostream& out_stream_;

    char buffer_;
    size_t buffer_size_;

    void Flush() {
        out_stream_.write(&buffer_, 1);
        buffer_size_ = 0;
        buffer_ = 0;
    }

public:

    BitWriter(std::ostream& os) : out_stream_(os), buffer_size_(0), buffer_(0) {
    }

    void Push(bool bit) {
        buffer_ |= (static_cast<int>(bit) << (8 - (buffer_size_ + 1)));
        ++buffer_size_;
        if (buffer_size_ == 8) {
            Flush();
        }
    }

    void Push(size_t to_write, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            Push((to_write & (1 << i)) > 0);
        }
    }

    void Push(std::vector<bool> to_write) {
        for (const auto& bit : to_write) {
            Push(bit);
        }
    }

    void Clear() {
        Flush();
    }
};
