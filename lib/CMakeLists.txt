add_library(socket socket.cpp server_socket.cpp)
target_include_directories(socket PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(socket PRIVATE cxx_std_98)

add_library(http http.cpp)
target_include_directories(http PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(http PRIVATE cxx_std_98)