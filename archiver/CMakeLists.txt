project(archiver)

cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(library)

add_executable(archiver
    main.cpp
        Compressor.h Compressor.cpp Decompressor.h)

target_link_libraries(archiver archiver_library)

