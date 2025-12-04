cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

project(bn-superh-arch CXX)

add_library(${PROJECT_NAME} SHARED
        src/architecture.cpp src/architecture.h src/flags.h src/info.cpp src/instructions.cpp src/instructions.h src/lift.cpp src/opcodes.cpp src/opcodes.h
        src/registers.cpp src/registers.h src/sizes.h src/text.cpp)

target_link_libraries(${PROJECT_NAME}
        binaryninjaapi)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin)

set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

bn_install_plugin(${PROJECT_NAME})

# Install Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        DOWNLOAD_EXTRACT_TIMESTAMP true
        URL https://github.com/google/googletest/archive/4902ea2d7c6faed89b6facee00baa34bb108fc0d.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test Architecture
add_executable(superh_architecture_test src/architecture_info_test.cpp src/architecture_text_test.cpp)
target_link_libraries(superh_architecture_test GTest::gtest_main ${PROJECT_NAME})

# Test Opcode (De)Serialization
add_executable(superh_opcodes_test src/opcodes_test.cpp)
target_link_libraries(superh_opcodes_test GTest::gtest_main ${PROJECT_NAME})

# Discover Tests
include(GoogleTest)
gtest_discover_tests(superh_architecture_test superh_opcodes_test)
