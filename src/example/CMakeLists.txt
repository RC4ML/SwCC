cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(qdma_benchmark LANGUAGES CXX)

add_compile_options(-O3 -mavx512f -march=native)


add_executable(dcqcn
	dcqcn.cpp
)
target_include_directories(dcqcn PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(dcqcn PRIVATE fmt::fmt-header-only)
target_link_libraries(dcqcn PRIVATE qdma pthread)
