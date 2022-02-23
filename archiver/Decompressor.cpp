#include "Decompressor.h"

#include "BitReader.h"
#include "Trie.h"

#include <string>
#include <vector>

std::vector<bool> Decompressor::MakeVector(size_t code, size_t code_len) {
    std::vector<bool> vec_code;
    for (size_t i = code_len - 1; i < MAX_ALP_SIZE; --i) {
        vec_code.push_back(code & (1 << i));
    }
    return vec_code;
}

void Decompressor::ReviveCanonicalCodes(std::vector<size_t> size_cnt, const std::vector<size_t>& symbols) {
    size_t size = 1;
    size_t cur_code = 0;
    for (size_t i = 0; i < symbols.size(); ++i) {
        if (i != 0) {
            ++cur_code;
        }
        size_t old_size = size;
        while (size_cnt[size] == 0) {
            ++size;
        }
        --size_cnt[size];

        cur_code <<= (size - old_size);
        codes_[symbols[i]] = MakeVector(cur_code, size);
    }
}

Decompressor::Decompressor(std::string file_name, std::istream& is)
                : file_name_(file_name), codes_(MAX_ALP_SIZE), reader_(is) {
}

void Decompressor::ReviveDecodeInfo() {
    codes_.assign(MAX_ALP_SIZE, std::vector<bool>(0));
    alp_size_ = reader_.GetChar(9);
    std::vector<size_t> symbols(alp_size_);
    for (auto& x : symbols) {
        x = reader_.GetChar(9);
    }

    std::vector<size_t> size_cnt(1, 0);
    for (size_t symbol_cnt = 0; symbol_cnt < alp_size_;) {
        size_cnt.push_back(reader_.GetChar(9));
        symbol_cnt += size_cnt.back();
    }

    ReviveCanonicalCodes(size_cnt, symbols);
}

std::string Decompressor::GetFileName(Trie& my_trie) {
    std::string cur_file_name;
    size_t cur_symb;
    while (true) {
        while (true) {
            auto pair = my_trie.GetCode(reader_.GetChar(1));
            if (pair.first) {
                cur_symb = pair.second;
                break;
            }
        }

        if (cur_symb == FILE_NAME_END_CODE) {
            break;
        } else {
            cur_file_name += static_cast<unsigned char>(cur_symb);
        }
    }

    return cur_file_name;
}

bool Decompressor::PrintFile(Trie& my_trie, std::string& cur_file_name) {
    std::ofstream file_out(cur_file_name.data(), std::ios_base::binary);
    bool arch_end = false;

    size_t cur_symb;
    while (true) {
        while (true) {
            auto pair = my_trie.GetCode(reader_.GetChar(1));
            if (pair.first) {
                cur_symb = pair.second;
                break;
            }
        }

        if (cur_symb >= FILE_NAME_END_CODE) {
            if (cur_symb == ARCHIVE_END_CODE) {
                arch_end = true;
            }
            break;
        } else {
            char ch = static_cast<char>(cur_symb);
            file_out.write(&ch, 1);
        }
    }

    if (arch_end) {
        file_out.close();
    }

    return arch_end;
}

void Decompressor::Decompress() {
    while (true) {
        ReviveDecodeInfo();

        Trie my_trie(codes_);

        std::string cur_file_name = GetFileName(my_trie);

        bool arch_end = PrintFile(my_trie, cur_file_name);

        if (arch_end) {
            break;
        }
    }
}