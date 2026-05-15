cmake_minimum_required(VERSION 3.14)
project(bestreviewer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    TLS_VERIFY OFF
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- CMake - ApprovalTests 설치 ---
# FetchContent로 헤더 전용 라이브러리 추가
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0)
FetchContent_MakeAvailable(googletest)

include_directories(include)

enable_testing()

add_executable(tv_test
    test/TVChannelControllerTest.cpp
    src/TVChannelController.cpp)
target_include_directories(tv_test PRIVATE include)
target_link_libraries(tv_test
    GTest::gtest_main GMock::gmock_main)
gtest_discover_tests(tv_test)
