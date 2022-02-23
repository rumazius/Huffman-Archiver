cmake_minimum_required(VERSION 3.20)

set(SRC_FILES PriorQueue.h TextReader.cpp BitWriter.cpp BitReader.cpp Trie.cpp)

add_library(archiver_library ${SRC_FILES})
target_include_directories(archiver_library PUBLIC ./)