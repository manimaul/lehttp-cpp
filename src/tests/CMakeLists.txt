add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests HttpServerTest.cpp)

# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest gtest_main)
install(TARGETS runUnitTests DESTINATION bin)
