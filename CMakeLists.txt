cmake_minimum_required(VERSION 3.10)
project(test-task)

set(EXE_NAME curves)
set(LIB_NAME libcurves)

add_library(${LIB_NAME} INTERFACE
    include/curves/curves.h
)
target_include_directories(${LIB_NAME} INTERFACE include)

find_package(TBB REQUIRED tbb)

add_executable(${EXE_NAME} main.cpp)
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME} tbb)
target_compile_features(${EXE_NAME} PRIVATE cxx_std_20)
