cmake_minimum_required(VERSION 3.9)
project(lehttp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(LibEvent REQUIRED)
add_subdirectory(src/tests)

add_executable(lehttp
        src/main.cpp
        src/Server.cpp
        src/Server.h
        src/Config.h
        src/LEhttp.h
        src/HttpMethod.h
        src/HttpStatus.h
        src/HttpResponse.h
        src/HttpRequest.h)

target_link_libraries(lehttp ${LIBEVENT_LIBRARIES})
