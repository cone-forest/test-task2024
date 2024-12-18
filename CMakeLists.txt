cmake_minimum_required(VERSION 3.10)
project(test-task)

set(EXE_NAME curves)
set(LIB_NAME libcurves)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(STD_SEQ    "Enable sequenced calculation of radii sum"                     ON)
option(STD_PAR    "Enable parallel calculation of radii sum using std::execution" OFF)
option(TBB_PAR    "Enable parallel calculation of radii sum using TBB"            OFF)
option(OPENMP_PAR "Enable parallel calculation of radii sum using OpenMP"         OFF)

add_library(${LIB_NAME} INTERFACE
    include/curves/curves.h
)
target_include_directories(${LIB_NAME} INTERFACE include)

set(CURVES_DEPENDENCIES_LIBS "")
set(CURVES_COMPILE_DEFINITIONS  "")

if (STD_SEQ)
  list(APPEND CURVES_COMPILE_DEFINITIONS "STD_SEQ")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" AND STD_PAR)
  find_package(TBB REQUIRED tbb)
  list(APPEND CURVES_DEPENDENCIES_LIBS tbb)
  list(APPEND CURVES_COMPILE_DEFINITIONS "STD_PAR")
endif()

if (TBB_PAR)
  find_package(TBB REQUIRED tbb)
  list(APPEND CURVES_DEPENDENCIES_LIBS tbb)
  list(APPEND CURVES_COMPILE_DEFINITIONS "TBB_PAR")
endif()

if (OPENMP_PAR)
  find_package(OpenMP REQUIRED)
  list(APPEND CURVES_DEPENDENCIES_LIBS OpenMP::OpenMP_CXX)
  list(APPEND CURVES_COMPILE_DEFINITIONS "OPENMP_PAR")
endif()

add_executable(${EXE_NAME} main.cpp)
target_compile_features(${EXE_NAME} PRIVATE cxx_std_20)
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME} ${CURVES_DEPENDENCIES_LIBS})
target_compile_definitions(${EXE_NAME} PRIVATE ${CURVES_COMPILE_DEFINITIONS})
