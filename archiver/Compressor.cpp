#include "Compressor.h"

#include "BitWriter.h"
#include "Trie.h"

#include <cstdint>

std::vector<bool> Compressor::MakeVector(size_t code, size_t code_len) {
    std::vector<bool> vec_code;
    for (size_t i = code_len - 1; i < MAX_ALP_SIZE; --i) {
        vec_code.push_back(code & (1 << i));
    }
    return vec_code;
}

std::vector<Compressor::Code> Compressor::CanonicalCodes(const std::vector<std::vector<bool>>& codes) {
    std::vector<Code> to_sort;
    for (size_t i = 0; i < codes.size(); ++i) {
        to_sort.push_back(std::make_pair(i, codes[i]));
    }

    std::sort(to_sort.begin(), to_sort.end(), [&](const Code& a, const Code& b) {
        if (a.code.size() < b.code.size()) {
            return true;
        } else if (a.code.size() > b.code.size()) {
            return false;
        } else {
            return a.index < b.index;
        }
    });

    std::vector<Code> new_codes(MAX_ALP_SIZE);

    size_t current_code = 0;
    size_t code_len = 0;
    alp_size_ = MAX_ALP_SIZE;
    for (const auto& [i, code] : to_sort) {
        if (code.empty()) {
            --alp_size_;
            continue;
        }

        if (code_len == 0) {
            new_codes.push_back(std::make_pair(i, MakeVector(current_code, code.size())));
            code_len = code.size();
            continue;
        }

        ++current_code;

        if (code_len != code.size()) {
            current_code <<= (code.size() - code_len);
            code_len = code.size();
        }

        new_codes.push_back(std::make_pair(i, MakeVector(current_code, code.size())));
    }
    max_symbol_code_size_ = code_len;

    return new_codes;
}

Compressor::Compressor(std::vector<size_t> symbol_cnt, BitWriter& writer, std::string file_name)
                                                    : symbol_size_cnt_(MAX_ALP_SIZE), file_name_(file_name), writer_(writer) {


    Trie b(symbol_cnt);
    std::vector<std::vector<bool>> codes = b.GetCodes();
    std::vector<Code> new_codes = CanonicalCodes(codes);

    for (const auto& [i, code] : new_codes) {
        ++symbol_size_cnt_[code.size()];
        codes[i] = code;
    }

    codes_ = codes;
    sorted_codes_ = new_codes;
}

void Compressor::WriteDecodeInfo() {
    writer_.PushToBuffer(alp_size_, 9);
    for (const auto& [i, symbol_code] : sorted_codes_) {
        if (symbol_code.empty()) {
            continue;
        }
        writer_.PushToBuffer(i, 9);
    }

    {
        std::vector<size_t> non_zero_cnt;
        for (size_t i = symbol_size_cnt_.size() - 1; i > 0; --i) {
            if (symbol_size_cnt_[i] == 0 && non_zero_cnt.empty()) {
                continue;
            }
            non_zero_cnt.push_back(symbol_size_cnt_[i]);
        }
        std::reverse(non_zero_cnt.begin(), non_zero_cnt.end());

        for (const auto sz : non_zero_cnt) {
            writer_.PushToBuffer(sz, 9);
        }
    }
}

void Compressor::WriteFileName() {
    for (size_t i = 0; i < file_name_.size(); ++i) {
        writer_.PushToBuffer(codes_[static_cast<size_t>(file_name_[i])]);
    }

    writer_.PushToBuffer(codes_[FILE_NAME_END_CODE]);
}

void Compressor::WriteFileBody(bool archive_end_flag) {
    std::ifstream fin(file_name_.data(), std::ios_base::binary);
    char cur_char;
    fin.read(&cur_char, 1);

    while (fin.gcount() > 0) {
        writer_.PushToBuffer(codes_[static_cast<uint8_t>(cur_char)]);
        fin.read(&cur_char, 1);
    }

    if (archive_end_flag) {
        writer_.PushToBuffer(codes_[ARCHIVE_END_CODE]);
    } else {
        writer_.PushToBuffer(codes_[MORE_ONE_FILE_CODE]);
    }
}

void Compressor::WriteEncodedFile(bool archive_end_flag) {
    WriteDecodeInfo();

    WriteFileName();

    WriteFileBody(archive_end_flag);
}

Compressor::Code::Code(std::pair<size_t, std::vector<bool>> pair) : index(pair.first), code(pair.second) {
}

bool Compressor::Code::operator<(const Code& oth) const {
    return index < oth.index;
}