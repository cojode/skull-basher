cmake_minimum_required(VERSION 3.5)
project(hl_project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")

include_directories(hl)

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

add_executable(hlgenerator
    ${SRC_DIR}/hlgenerator.c
    ${COMMON_SOURCES}
)

# Set output directory (alternative method for older CMake)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)