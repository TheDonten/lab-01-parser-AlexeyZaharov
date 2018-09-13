cmake_minimum_required (VERSION 2.8)

include(gtest.cmake)
include_directories(SYSTEM
    ./
)

# if(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fsanitize=address -O1 -Wall -Wextra -Wno-unused-command-line-argument -Wshadow -Wnon-virtual-dtor -pedantic")
# endif(CMAKE_COMPILER_IS_GNUCXX)

# enum your files
add_library(json STATIC json.hpp)
add_executable(main test.cpp)
target_link_libraries(main json)
