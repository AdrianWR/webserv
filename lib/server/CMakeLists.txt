add_library(server
    socket.cpp
    server_socket.cpp
    server.cpp
)
target_include_directories(server PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(server PRIVATE http)
target_compile_features(server PRIVATE cxx_std_98)