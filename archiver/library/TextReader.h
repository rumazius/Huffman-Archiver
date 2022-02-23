#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class TextReader {
private:
    std::istream& fin_;
    std::vector<size_t> symbol_cnt_;

public:
    TextReader(std::istream& is) : fin_(is), symbol_cnt_(259, 0) {
    }

    std::vector<size_t> GetSymbolCnt();
};