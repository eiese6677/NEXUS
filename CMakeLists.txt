cmake_minimum_required(VERSION 3.20)

project(NEXUS
    VERSION 0.1
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# =========================
# NEXUS Compiler Library
# =========================

add_library(nexus_compiler

    compiler/lexer/Lexer.cpp
    compiler/lexer/KeywordTable.cpp

    compiler/parser/Parser.cpp
    compiler/semantic/SemanticAnalyzer.cpp
    compiler/ir/IRBuilder.cpp
    compiler/codegen/CodeGenerator.cpp
    vm/VirtualMachine.cpp

    compiler/unicode/Unicode.cpp
)


target_include_directories(nexus_compiler
    PUBLIC
        ${PROJECT_SOURCE_DIR}/compiler
)



# =========================
# Tests
# =========================

add_executable(test_lexer
    tests/lexer/test.cpp
)
target_link_libraries(test_lexer
    PRIVATE
        nexus_compiler
)

add_executable(test_parser
    tests/parser/test.cpp
)
target_link_libraries(test_parser
    PRIVATE
        nexus_compiler
)

add_executable(NEXUS

    src/main.cpp
)


target_link_libraries(NEXUS
    PRIVATE
        nexus_compiler
)