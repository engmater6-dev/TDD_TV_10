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
FetchContent_Declare(
    ApprovalTests
    GIT_REPOSITORY https://github.com/approvals/ApprovalTests.cpp.git
    GIT_TAG v.10.13.0
)
FetchContent_MakeAvailable(ApprovalTests)

include_directories(include)

enable_testing()

# --- TVChannelController 라이브러리 추가 ---
add_library(tvcontroller
    src/TVChannelController.cpp
    include/TVChannelController.h
)
target_include_directories(tvcontroller PUBLIC include)

# --- 테스트 실행 파일들 ---
add_executable(TunerTest test/TunerTest.cpp)
target_link_libraries(TunerTest PRIVATE tvcontroller GTest::gtest_main GTest::gmock)

add_executable(TVControllerTest test/TVControllerTest.cpp)
target_link_libraries(TVControllerTest PRIVATE tvcontroller GTest::gtest_main GTest::gmock)

add_executable(TVControllerTestApprovalTests test/TVControllerTestApprovalTests.cpp)
target_link_libraries(TVControllerTestApprovalTests PRIVATE tvcontroller ApprovalTests GTest::gtest_main)
target_include_directories(TVControllerTestApprovalTests PRIVATE ${approvaltests_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(TunerTest)
gtest_discover_tests(TVControllerTest)
gtest_discover_tests(TVControllerTestApprovalTests)
