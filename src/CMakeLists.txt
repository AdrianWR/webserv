add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${google_googletest_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE server)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE utils)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_98)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
# target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -H)