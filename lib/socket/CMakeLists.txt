add_library(socket socket.cpp server_socket.cpp)
target_include_directories(socket PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(socket PRIVATE http)
target_compile_features(socket PRIVATE cxx_std_98)