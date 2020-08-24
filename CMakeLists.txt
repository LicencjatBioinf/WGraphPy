cmake_minimum_required(VERSION 2.8.12)
project(WGraphPy)

add_subdirectory(pybind11)
pybind11_add_module(WGraphCpp src/main.cpp)