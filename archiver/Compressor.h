#pragma once

#include "BitWriter.h"

class Compressor {
private:
    struct Code {
        size_t index;
        std::vector<bool> code;

        Code() = default;
        Code(std::pair<size_t, std::vector<bool>> pair);
        bool operator<(const Code& oth) const;
    };

    std::vector<bool> MakeVector(size_t code, size_t code_len);

    std::vector<Code> CanonicalCodes(const std::vector<std::vector<bool>>& codes);

    std::vector<std::vector<bool>> codes_;
    std::vector<Code> sorted_codes_;

    size_t max_symbol_code_size_;
    size_t alp_size_;
    static const size_t FILE_NAME_END_CODE = 256;
    static const size_t MORE_ONE_FILE_CODE = 257;
    static const size_t ARCHIVE_END_CODE = 258;
    static const size_t MAX_ALP_SIZE = 259;

    std::vector<size_t> symbol_size_cnt_;
    std::string file_name_;
    BitWriter& writer_;

    void WriteDecodeInfo();

    void WriteFileName();

    void WriteFileBody(bool archive_end_flag);

public:
    Compressor(std::vector<size_t> symbol_cnt, BitWriter& writer, std::string file_name);

    void WriteEncodedFile(bool archive_end_flag = false);
};
