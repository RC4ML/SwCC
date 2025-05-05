cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(qdma_benchmark LANGUAGES CXX)

add_compile_options(-O3 -mavx512f -march=native)


add_executable(qdma_throughput
        qdma_throughput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dma/dma.cpp
        #	${CMAKE_CURRENT_SOURCE_DIR}/mmio/mmio.cpp
)
target_include_directories(qdma_throughput PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/dma
        #	${CMAKE_CURRENT_SOURCE_DIR}/mmio
)
target_link_libraries(qdma_throughput PRIVATE fmt::fmt-header-only)
target_link_libraries(qdma_throughput PRIVATE qdma pthread)

add_executable(qdma_random
        qdma_random.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dma/dma.cpp
)
target_include_directories(qdma_random PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/dma
)
target_link_libraries(qdma_random PRIVATE fmt::fmt-header-only)
target_link_libraries(qdma_random PRIVATE qdma pthread)

add_executable(qdma_latency
	qdma_latency.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dma/dma.cpp
)
target_include_directories(qdma_latency PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/dma
)
target_link_libraries(qdma_latency PRIVATE fmt::fmt-header-only)
target_link_libraries(qdma_latency PRIVATE qdma pthread)
#
#add_executable(axil_latency
#	axil_latency.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/load/load.cpp
#)
#target_include_directories(axil_latency PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}/load
#)
#target_link_libraries(axil_latency
#	qdma
#	pthread
#)

add_executable(qdma_gpu
		qdma_gpu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/dma/dma.cpp
)
target_include_directories(qdma_gpu PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/..
		${CMAKE_CURRENT_SOURCE_DIR}/dma
)
target_link_libraries(qdma_gpu PRIVATE fmt::fmt-header-only)
target_link_libraries(qdma_gpu PRIVATE qdma pthread)

add_executable(send
	send.cpp
)
target_include_directories(send PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(send PRIVATE fmt::fmt-header-only)
target_link_libraries(send PRIVATE qdma pthread)

add_executable(timely
	timely.cpp
)
target_include_directories(timely PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(timely PRIVATE fmt::fmt-header-only)
target_link_libraries(timely PRIVATE qdma pthread)

add_executable(timely10
	timely10.cpp
)
target_include_directories(timely10 PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(timely10 PRIVATE fmt::fmt-header-only)
target_link_libraries(timely10 PRIVATE qdma pthread)

add_executable(dcqcn
	dcqcn.cpp
)
target_include_directories(dcqcn PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(dcqcn PRIVATE fmt::fmt-header-only)
target_link_libraries(dcqcn PRIVATE qdma pthread)

add_executable(dcqcn10
	dcqcn10.cpp
)
target_include_directories(dcqcn10 PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(dcqcn10 PRIVATE fmt::fmt-header-only)
target_link_libraries(dcqcn10 PRIVATE qdma pthread)