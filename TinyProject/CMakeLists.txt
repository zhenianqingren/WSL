cmake_minimum_required(VERSION 3.10)
project(TINYPROJECT)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/operation)
aux_source_directory(${PROJECT_SOURCE_DIR}/operation/. METHOD)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/. OBJECT)

set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_compile_options(-Wall -std=c++11 -O2 -g)
add_library(MLIB SHARED ${METHOD})
target_link_libraries(MLIB)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp ${OBJECT})
