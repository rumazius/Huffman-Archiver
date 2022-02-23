#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class BitWriter {
private:
    std::ostream& out_stream_;

    char buffer_;
    size_t buffer_size_;

    void Flush();

public:

    BitWriter(std::ostream& os) : out_stream_(os), buffer_size_(0), buffer_(0) {
    }
    BitWriter(BitWriter& b) : out_stream_(b.out_stream_), buffer_(b.buffer_), buffer_size_(b.buffer_size_) {
    }

    void PushToBuffer(bool bit);

    void PushToBuffer(size_t to_write, size_t size);

    void PushToBuffer(std::vector<bool> to_write);

    void Clear();
};
