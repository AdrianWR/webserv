add_library(utils utils.cpp)
target_include_directories(utils PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(utils PRIVATE http)
target_compile_features(utils PRIVATE cxx_std_98)
