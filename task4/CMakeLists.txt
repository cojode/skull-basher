cmake_minimum_required(VERSION 3.10)
project(hl_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(SRC_DIR hl)
set(COMMON_SOURCES 
    ${SRC_DIR}/common.c
    ${SRC_DIR}/hlparser.c
    ${SRC_DIR}/strlcat.c
)

add_executable(hldetector
    ${SRC_DIR}/hldetector.c
    ${COMMON_SOURCES}
)
target_include_directories(hldetector PRIVATE ${SRC_DIR})

add_executable(hlgenerator
    ${SRC_DIR}/hlgenerator.c
    ${COMMON_SOURCES}
)
target_include_directories(hlgenerator PRIVATE ${SRC_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)