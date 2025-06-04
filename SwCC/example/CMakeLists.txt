cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(qdma_benchmark LANGUAGES CXX)

add_compile_options(-O3 -mavx512f -march=native)


add_executable(latency_sender
	latency_sender.cpp
)
target_include_directories(latency_sender PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(latency_sender PRIVATE fmt::fmt-header-only)
target_link_libraries(latency_sender PRIVATE qdma pthread)

add_executable(latency_receiver
	latency_receiver.cpp
)
target_include_directories(latency_receiver PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(latency_receiver PRIVATE fmt::fmt-header-only)
target_link_libraries(latency_receiver PRIVATE qdma pthread)

add_executable(throughput_sender
	throughput_sender.cpp
)
target_include_directories(throughput_sender PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(throughput_sender PRIVATE fmt::fmt-header-only)
target_link_libraries(throughput_sender PRIVATE qdma pthread)

add_executable(throughput_receiver
	throughput_receiver.cpp
)
target_include_directories(throughput_receiver PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(throughput_receiver PRIVATE fmt::fmt-header-only)
target_link_libraries(throughput_receiver PRIVATE qdma pthread)

