#pragma once

#include "TextReader.h"
#include "BitWriter.h"
#include "Bor.h"


class Compressor {
private:
    std::vector<bool> CanonicalVector(size_t code, size_t code_len) {
        std::vector<bool> vec_code;
        for (size_t i = code_len - 1; i < 20; --i) {
            vec_code.push_back(code & (1 << i));
        }
        return vec_code;
    }

    std::vector<std::pair<size_t, std::vector<bool>>> Canonical(const std::vector<std::vector<bool>>& codes) {
        std::vector<std::pair<std::vector<bool>, size_t>> to_sort;
        for (size_t i = 0; i < codes.size(); ++i) {
            to_sort.push_back(std::make_pair(codes[i], i));
        }

        std::sort(to_sort.begin(), to_sort.end(), [&](const std::pair<std::vector<bool>, size_t>& a,
                                                      const std::pair<std::vector<bool>, size_t>& b) {
            if (a.first.size() < b.first.size()) {
                return true;
            } else if (a.first.size() > b.first.size()) {
                return false;
            } else {
                return a.second < b.second;
            }
        });

        std::vector<std::pair<size_t, std::vector<bool>>> new_codes(259);

        size_t current_code = 0;
        size_t code_len = 0;
        symbols_cnt_ = 259;
        for (const auto& [code, i] : to_sort) {
            if (code.empty()) {
                --symbols_cnt_;
                continue;
            }

            if (code_len == 0) {
                new_codes.push_back(std::make_pair(i, CanonicalVector(current_code, code.size())));
                code_len = code.size();
                continue;
            }

            ++current_code;

            if (code_len != code.size()) {
                current_code <<= 1;
                code_len = code.size();
            }

            new_codes.push_back(std::make_pair(i, CanonicalVector(current_code, code.size())));
        }
        max_symbol_code_size_ = code_len;

        return new_codes;
    }

    std::vector<std::vector<bool>> codes_;
    std::vector<std::pair<size_t, std::vector<bool>>> sorted_codes_;
    size_t max_symbol_code_size_;
    size_t symbols_cnt_;
    std::vector<size_t> symbol_size_cnt_;
    std::string file_name_;

    BitWriter& writer_;

public:

    Compressor(std::vector<size_t> symbol_cnt, BitWriter& writer, std::string file_name)
                                        : symbol_size_cnt_(9), file_name_(file_name), writer_(writer) {

        Bor b(symbol_cnt);
        std::vector<std::vector<bool>> codes = b.GetCodes();
        std::vector<std::pair<std::size_t, std::vector<bool>>> new_codes = Canonical(codes);

        for (const auto& [i, code] : new_codes) {
            ++symbol_size_cnt_[code.size()];
            codes[i] = code;
        }

        codes_ = codes;
        sorted_codes_ = new_codes;
    }

    void Write(bool archive_end_flag = false) {
        writer_.Push(symbols_cnt_, 9);
        for (const auto& [i, symb_code] : sorted_codes_) {
            if (symb_code.empty()) {
                continue;
            }
            writer_.Push(i, 9);
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
                writer_.Push(sz, 9);
            }
        }


        for (size_t i = 0; i < file_name_.size(); ++i) {
            writer_.Push(codes_[static_cast<size_t>(file_name_[i])]);
        }

        writer_.Push(codes_[256]);

        std::ifstream fin(file_name_.data());
        char cur_char;
        fin.read(&cur_char, 1);
        while (fin.gcount() > 0) {
            writer_.Push(codes_[static_cast<size_t>(cur_char)]);
            fin.read(&cur_char, 1);
        }

        if (archive_end_flag) {
            writer_.Push(codes_[258]);
        } else {
            writer_.Push(codes_[257]);
        }
    }
};
