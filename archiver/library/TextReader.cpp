#include "TextReader.h"

#include <cstdint>

std::vector<size_t> TextReader::GetSymbolCnt() {
    char cur_char;
    fin_.read(&cur_char, 1);
    while (fin_.gcount() != 0) {
        ++symbol_cnt_[static_cast<uint8_t>(cur_char)];
        fin_.read(&cur_char, 1);
    }
    ++symbol_cnt_[256];
    ++symbol_cnt_[257];
    ++symbol_cnt_[258];

    return symbol_cnt_;
}