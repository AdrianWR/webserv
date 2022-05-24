include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_socket test_socket.cpp)
target_include_directories(test_socket PRIVATE ${PROJECT_SOURCE_DIR}/include ${google_googletest_INCLUDE_DIRS})
target_link_libraries(test_socket PRIVATE http gtest_main)

include(GoogleTest)
gtest_discover_tests(test_socket)
