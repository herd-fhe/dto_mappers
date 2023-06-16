cmake_minimum_required(VERSION 3.21)
project(
        herd_dto_mappers
        VERSION 0.1
        DESCRIPTION "Herd's dto mappers"
        LANGUAGES CXX
)

## Common
add_library(herd_dto_mappers)
target_compile_features(herd_dto_mappers INTERFACE cxx_std_20)

# Warnings config
if(MSVC)
    target_compile_options(
            herd_dto_mappers
            INTERFACE
            /W4
            /WX "/permissive-"
    )
else()
    target_compile_options(
            herd_dto_mappers
            INTERFACE
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wdouble-promotion
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Werror
    )
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(
                herd_dto_mappers
                INTERFACE
                -Waddress-of-packed-member
                -Wmisleading-indentation
                -Wduplicated-cond
                -Wduplicated-branches
                -Wlogical-op
                -Wuseless-cast
        )
    endif()
endif()


target_include_directories(herd_dto_mappers PUBLIC include)
target_include_directories(herd_dto_mappers PRIVATE include)

target_sources(
        herd_dto_mappers
        PRIVATE
        src/crypto.cpp
        src/executor.cpp
        src/storage.cpp
        src/worker.cpp

        include/herd/mapper/exception.hpp
        include/herd/mapper/crypto.hpp
        include/herd/mapper/storage.hpp
        include/herd/mapper/executor.hpp
        include/herd/mapper/worker.hpp
)

target_link_libraries(herd_dto_mappers PUBLIC herd_common herd_proto)