project(sampquery-c)
cmake_minimum_required(VERSION 3.8)

set(LIBRARY_DIR "${CMAKE_SOURCE_DIR}/lib")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_subdirectory(src)

option(ENABLE_TEST "Enables test module" OFF)

if(ENABLE_TEST)
    add_subdirectory(test)
    install(TARGETS test DESTINATION "${CMAKE_SOURCE_DIR}/bin")
endif()
