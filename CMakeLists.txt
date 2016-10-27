cmake_minimum_required(VERSION 3.6)
project(DCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES sintactAnalyzer.c util/binarySearchTree.h util/binarySearchTree.c scanner.h scanner.c symbolTable.h symbolTable.c lexAnalyzer.h lexAnalyzer.c util/lexDefinitions.h lexicAnalysisFunctions.c lexicAnalysisFunctions.h errorManager.h errorManager.c)
add_executable(DCompiler ${SOURCE_FILES})