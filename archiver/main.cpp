#include "TextReader.h"
#include "Bor.h"
#include "BitWriter.h"
#include "Compressor.h"
#include "Decompressor.h"
#include "BitReader.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>



int main(int argc, char* argv[]) {
    if (argc == 0) {
        return 0;
    }
    if (std::strcmp(argv[1], "-h") == 0) {
        std::cout << "archiver -h -- helper\n";
        std::cout << "archiver -c archive_name file1 [file2 ...] -- compressing\n";
        std::cout << "archiver -d archive_name -- decompressing\n";
        return 0;
    } else if (std::strcmp(argv[1], "-d") == 0) {
        if (argc == 2) {
            std::cerr << "Enter file name!";
            throw;
        }
        std::string archive_name = argv[2];
        std::ifstream is(argv[2]);
        if (!is.is_open()) {
            std::cerr << "No such file!";
            throw;
        }
        Decompressor decomp(archive_name, is);
        decomp.Decompress();
    } else {
        if (argc <= 3) {
            std::cerr << "Enter all the file names!";
            throw;
        }

        std::ofstream fout(argv[2]);
        BitWriter writer(fout);
        for (size_t i = 3; i != argc; ++i) {
            std::ifstream fin(argv[i]);

            TextReader t(fin);
            auto symbol_cnt = t.GetSymbolCnt();
            for (size_t j = 0; argv[i][j] != '\0'; ++j) {
                ++symbol_cnt[static_cast<size_t>(argv[i][j])];
            }

            Compressor compressor(symbol_cnt, writer, argv[i]);
            if (i + 1 == argc) {
                compressor.Write(true);
            } else {
                compressor.Write();
            }
        }
        writer.Clear();
    }


//    std::ifstream fin("in");
//    std::ofstream fout("out");
//    TextReader t(fin);
//    auto symb_cnt = t.GetSymbolCnt();
//    symb_cnt['i']++;
//    symb_cnt['n']++;
//    ++symb_cnt[256];
//    ++symb_cnt[257];
//    ++symb_cnt[258];
//    BitWriter writer(fout);
//    Compressor compressor(symb_cnt, writer, "in");
//    compressor.Write();
//    writer.Clear();

//    std::ifstream fin1("out");
//    Decompressor decomp("out", fin1);
    //return 0;
//    decomp.Decompress();

    return 0;
}
