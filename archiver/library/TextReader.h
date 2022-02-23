#pragma once

#include <string_view>
#include <iostream>
#include <fstream>
#include <vector>

class TextReader {/// todo count file name!!!
private:
    std::istream& fin_;
    std::vector<size_t> counter_;

public:
    TextReader(std::istream& is) : fin_(is), counter_(259, 0) {
    }

    std::vector<size_t> GetSymbolCnt() {
        char cur_char;
        fin_.read(&cur_char, 1);
        while (fin_.gcount() != 0) {
            ++counter_[static_cast<size_t>(cur_char)];
            fin_.read(&cur_char, 1);
        }
        ++counter_[256];
        ++counter_[257];
        ++counter_[258];

        return counter_;
    }
};


