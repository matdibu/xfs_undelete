cmake_minimum_required(VERSION 3.20)

project(
    "xfs_undelete"
    VERSION 0.0.1
    LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

set(sources
    src/main.cpp
    src/linux_file.cpp
    src/linux_file2.cpp
    src/xfs_exceptions.cpp
    src/xfs_extent.cpp
    src/xfs_inode.cpp
    src/xfs_inode_entry.cpp
    src/xfs_parser.cpp
    src/xfs_undelete.cpp
)

add_compile_options(-Wall -Wextra -std=c++20 -Wno-c++98-compat -Wno-c++98-compat-pedantic)

add_executable(${PROJECT_NAME} ${sources})

#target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

