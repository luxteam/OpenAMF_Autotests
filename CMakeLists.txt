cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    api.cpp
    autotests.cpp
    geometry.cpp
    implementation.cpp
    main.cpp
    multithread.cpp
    references.cpp
)

set(HEADERS
    autotests.h
)

add_executable(run_tests ${SOURCES} ${HEADERS})

include_directories(../../../include)

target_link_libraries(run_tests PUBLIC
    gtest_main
    openAmf
)

