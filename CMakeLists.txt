cmake_minimum_required(VERSION 3.9)
project(lehttp)

set(CMAKE_CXX_STANDARD 17)

# Find LibEvent
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(LibEvent REQUIRED)

# Include Google Tests
if (DEFINED ENV{GTEST})
    message("including tests")
    add_subdirectory(tests)
endif()

# Setup Library
add_library(lehttp SHARED lehttp/Server.cpp)

include_directories(lehttp/include)


target_link_libraries(lehttp ${LIBEVENT_LIBRARIES})

install(TARGETS lehttp
        LIBRARY DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/lehttp/include/
        DESTINATION include/lehttp
        FILES_MATCHING PATTERN "*.h*")

if (DEFINED ENV{LE_EXAMPLE})
    add_executable(lehttp_example example/main.cpp)
    target_link_libraries(lehttp_example lehttp)
    target_include_directories(lehttp_example PRIVATE lehttp/include)
    install(TARGETS lehttp_example DESTINATION bin)
endif()
