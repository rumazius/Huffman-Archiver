#include "BitWriter.h"

#include <iostream>
#include <vector>

void BitWriter::Flush() {
    out_stream_.write(&buffer_, 1);
    buffer_size_ = 0;
    buffer_ = 0;
}

void BitWriter::PushToBuffer(bool bit) {
    buffer_ |= (static_cast<char>(bit) << (8 - (buffer_size_ + 1)));
    ++buffer_size_;
    if (buffer_size_ == 8) {
        Flush();
    }
}

void BitWriter::PushToBuffer(size_t to_write, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        PushToBuffer((to_write & (1 << i)) > 0);
    }
}

void BitWriter::PushToBuffer(std::vector<bool> to_write) {
    for (bool bit : to_write) {
        PushToBuffer(bit);
    }
}

void BitWriter::Clear() {
    Flush();
}
