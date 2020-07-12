cmake_minimum_required(VERSION 3.6)
project(zetarftools)

# Build ZetaRF
add_subdirectory(ZetaRF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_compile_definitions(WIRINGPI)
include_directories(ZetaRF/src)

add_executable(AverageRSSI AverageRSSI.cpp)
target_link_libraries(AverageRSSI zetarf)