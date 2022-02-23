#pragma once

#include <string>
#include <vector>
#include "BitReader.h"
#include "Bor.h"

class Decompressor {
private:

    std::string file_name_;
    size_t alp_size_;
    std::vector<std::vector<bool>> codes_;

    BitReader reader_;

    std::vector<bool> CanonicalVector(size_t code, size_t code_len) {
        std::vector<bool> vec_code;
        for (size_t i = code_len - 1; i < 20; --i) {
            vec_code.push_back(code & (1 << i));
        }
        return vec_code;
    }

    void ReviveCans(std::vector<size_t> size_cnt, std::vector<size_t> symbols) {
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

            codes_[symbols[i]] = CanonicalVector(cur_code, size);
        }
    }

public:
    Decompressor(std::string file_name, std::istream& is)
                            : file_name_(file_name), codes_(259), reader_(is) {
    }

    void Decompress() {
        while (true) {
            alp_size_ = reader_.Get(9);
            std::vector<size_t> symbols(alp_size_);
            for (auto& x : symbols) {
                x = reader_.Get(9);
            }

            std::vector<size_t> size_cnt(1, 0);
            for (size_t symbol_cnt = 0; symbol_cnt < alp_size_;) {
                size_cnt.push_back(reader_.Get(9));
                symbol_cnt += size_cnt.back();
            }

            ReviveCans(size_cnt, symbols);

            Bor my_bor(codes_);

            std::string cur_file_name;
            size_t cur_symb;
            while (true) {
                while (true) {
                    auto pair = my_bor.GetCode(reader_.Get(1));
                    if (pair.first) {
                        cur_symb = pair.second;
                        break;
                    }
                }
                if (cur_symb == 256) {
                    break;
                } else {
                    cur_file_name += static_cast<char>(cur_symb);
                }
            }

            std::ofstream file_out(cur_file_name.data());
            bool arch_end = false;

            while (true) {
                while (true) {
                    auto pair = my_bor.GetCode(reader_.Get(1));
                    if (pair.first) {
                        cur_symb = pair.second;
                        break;
                    }
                }
                std::cout << cur_symb << std::endl;
                if (cur_symb > 255) {
                    if (cur_symb == 258) {
                        arch_end = true;
                    }
                    break;
                } else {
                    char ch = static_cast<char>(cur_symb);
                    file_out.write(&ch, 1);
                }
            }

            if (arch_end) {
                break;
            }
        }
    }
};