#pragma once

#include "BitReader.h"
#include "Trie.h"

#include <string>
#include <vector>

class Decompressor {
private:

    std::string file_name_;
    size_t alp_size_;
    std::vector<std::vector<bool>> codes_;
    static const size_t FILE_NAME_END_CODE = 256;
    static const size_t MORE_ONE_FILE_CODE = 257;
    static const size_t ARCHIVE_END_CODE = 258;
    static const size_t MAX_ALP_SIZE = 259;


    BitReader reader_;

    std::vector<bool> MakeVector(size_t code, size_t code_len);

    void ReviveCanonicalCodes(std::vector<size_t> size_cnt, const std::vector<size_t>& symbols);

    void ReviveDecodeInfo();

    std::string GetFileName(Trie& my_trie);

    bool PrintFile(Trie& my_trie, std::string& cur_file_name);

public:
    Decompressor(std::string file_name, std::istream& is);

    void Decompress();
};