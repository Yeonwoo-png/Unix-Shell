cmake_minimum_required(VERSION 3.16)

project(Reun LANGUAGES C)

add_executable(reun
    src/main.c
    src/lexer.c
)

set_target_properties(reun PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_include_directories(reun PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(reun PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
