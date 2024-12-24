cmake_minimum_required(VERSION 3.12)
project(music-gen)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall)

include_directories(AFTER include)

SET(SRC_LIST src/main.c src/param.c src/parse.c src/sample.c src/wav.c)

add_executable(mgen ${SRC_LIST})
target_link_libraries(mgen m)
