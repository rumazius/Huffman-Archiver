cmake_minimum_required(VERSION 3.20)

set(SRC_FILES PriorQueue.cpp PriorQueue.h TextReader.h TextReader.cpp Bor.h BitWriter.h BitReader.h)

add_library(archiver_library ${SRC_FILES})
target_include_directories(archiver_library PUBLIC ./)