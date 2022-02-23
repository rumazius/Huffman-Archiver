project(archiver)

cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(library)
add_subdirectory(tests)

add_executable(archiver
    main.cpp
        Compressor.cpp Decompressor.cpp)

target_link_libraries(archiver archiver_library)

