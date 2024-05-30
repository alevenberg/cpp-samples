cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

project(repro CXX)

find_package(unofficial-avro-cpp CONFIG REQUIRED)

add_executable(avro_read avro_read.cc)

target_compile_options(avro_read PUBLIC -fsanitize=undefined -fsanitize=address)
target_link_options(avro_read PUBLIC -fsanitize=undefined -fsanitize=address)

target_link_libraries(avro_read PRIVATE unofficial::avro-cpp::avrocpp)