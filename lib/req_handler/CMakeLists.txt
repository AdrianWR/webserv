add_library(req_handler req_handler.cpp error.cpp)
target_include_directories(req_handler PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(req_handler PRIVATE cxx_std_98)
