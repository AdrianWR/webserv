add_library(http http.cpp http_request.cpp)
target_include_directories(http PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(http PRIVATE cxx_std_98)