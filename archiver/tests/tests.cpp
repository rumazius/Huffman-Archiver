#include "BitReader.h"
#include "BitWriter.h"
#include "PriorQueue.h"
#include "Trie.h"
#include "TextReader.h"
#include "../Compressor.cpp"
#include "../Decompressor.cpp"

#include <iostream>
#include <sstream>
#include <fstream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

bool VectorEq(const std::vector<bool>& a, const std::vector<bool>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

TEST_CASE("TestPriorQue") {
    PriorQueue<int> q;
    q.Push(1);
    q.Push(1);
    q.Push(1);

    REQUIRE(q.Top() == 1);
    REQUIRE(q.Size() == 3);

    q.Pop();
    REQUIRE(q.Size() == 2);

    q.Push(0);

    REQUIRE(q.Top() == 0);

    while (!q.Empty()) {
        q.Pop();
    }

    REQUIRE(q.Empty());
    REQUIRE(q.Size() == 0);

    q.Push(4);
    q.Push(6);
    q.Push(0);
    q.Push(1);
    q.Push(1);
    q.Push(3);

    q.Pop();
    q.Pop();
    q.Pop();
    REQUIRE(q.Top() == 3);
}

TEST_CASE("TestBitReaderBitWriter") {
    std::ofstream os("TestRead.txt", std::ios_base::binary);
    BitWriter writer(os);

    writer.PushToBuffer(true);
    writer.Clear();


    writer.PushToBuffer(std::vector<bool>({true, true, true, false, false, false, true, true, true}));

    os.close();

    std::ifstream is("TestRead.txt", std::ios_base::binary);
    BitReader reader(is);

    int chr = reader.GetChar(8);
    REQUIRE(chr == 1);

    chr = reader.GetChar(8);
    REQUIRE(chr == 199);
}

TEST_CASE("TestBor") {
    std::vector<size_t> v(259, 0);
    v[0] = v[1] = 1;
    ++v[0];
    v[2] = 3;

    Trie trie(v);
    auto codes = trie.GetCodes();

    std::vector<bool> v0 = {false, true};
    std::vector<bool> v1 = {false, false};
    std::vector<bool> v2 = {true};

    REQUIRE(VectorEq(codes[0], v0));
    REQUIRE(VectorEq(codes[1], v1));
    REQUIRE(VectorEq(codes[2], v2));
}

TEST_CASE("TestCompDecomp") {
    std::ofstream ffile("test_ans0.txt", std::ios_base::binary);
    std::string tyan = "tyanochku_by";
    for (size_t i = 0; i < tyan.size(); ++i) {
        ffile.write(&tyan[i], 1);
    }
    ffile.close();

    std::ofstream fout("ans0", std::ios_base::binary);
    BitWriter writer(fout);
    std::string file_name = "test_ans0.txt";
    std::ifstream fin(file_name.data(), std::ios_base::binary);

    TextReader t(fin);
    auto symbol_cnt = t.GetSymbolCnt();

    for (size_t j = 0; j < file_name.size(); ++j) {
        ++symbol_cnt[static_cast<size_t>(file_name[j])];
    }

    Compressor compressor(symbol_cnt, writer, file_name.data());

    compressor.WriteEncodedFile(true);

    writer.Clear();

    fin.close();
    fout.close();

    std::string archive_name = "ans0";
    std::ifstream is(archive_name.data(), std::ios_base::binary);

    Decompressor decomp(archive_name, is);
    decomp.Decompress();

    std::string cur_res;
    std::ifstream ofs("test_ans0.txt");
    char ch;
    ofs.read(&ch, 1);
    while (ofs.gcount() > 0) {
        cur_res += ch;
        ofs.read(&ch, 1);
    }

    REQUIRE(cur_res == tyan);
}

