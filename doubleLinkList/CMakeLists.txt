cmake_minimum_required(VERSION 3.16)
project(DoublyLinkedList LANGUAGES CXX)

add_executable(dll_tests main.cpp)
target_compile_features(dll_tests PRIVATE cxx_std_17)
target_compile_options(dll_tests PRIVATE -Wall -Wextra -Wpedantic)
