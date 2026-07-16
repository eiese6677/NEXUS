cmake_minimum_required(VERSION 3.20)

project(NEXUS
    VERSION 0.1
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(NEXUS

    src/main.cpp

    compiler/lexer/Lexer.cpp
    compiler/lexer/Token.cpp

    compiler/unicode/Unicode.cpp
)


target_include_directories(NEXUS PRIVATE

    compiler/lexer
    compiler/unicode
)