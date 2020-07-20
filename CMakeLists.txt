cmake_minimum_required(VERSION 3.6)
project(zetarftools)

# Build ZetaRF
add_subdirectory(ZetaRF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_compile_definitions(WIRINGPI)
include_directories(ZetaRF/src)

file(GLOB_RECURSE CONFIG_SOURCES "src/configs/*.cpp")

add_executable(AverageRSSI src/AverageRSSI.cpp ${CONFIG_SOURCES})
target_link_libraries(AverageRSSI zetarf)

add_executable(BandwidthTest src/BandwidthTest.cpp ${CONFIG_SOURCES})
target_link_libraries(BandwidthTest zetarf)

add_executable(SimpleRx src/SimpleRx.cpp ${CONFIG_SOURCES})
target_link_libraries(SimpleRx zetarf)

add_executable(Beacon src/Beacon.cpp ${CONFIG_SOURCES})
target_link_libraries(Beacon zetarf)
