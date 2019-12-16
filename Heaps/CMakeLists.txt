cmake_minimum_required(VERSION 3.15)

project(Heaps)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
find_package(GTest REQUIRED)

# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
# include_directories(${gtest_INCLUDE_DIRS})
include_directories(googletest/googletest/include)

LINK_DIRECTORIES(/usr/local/lib)

add_executable(
    Heaps
    Heaps/main.cpp
    Heaps/IHeap.h
    Heaps/BinomialHeap.h
    Heaps/LeftistHeap.h
    Heaps/SkewHeap.h
    Heaps/StlHeap.h
    Heaps/Testing.h
    Heaps/Testing.cpp
)

target_link_libraries(Heaps ${GTEST_LIBRARIES} pthread)