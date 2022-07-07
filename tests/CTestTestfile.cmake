# CMake generated Testfile for 
# Source directory: /home/user42/webserver/tests
# Build directory: /home/user42/webserver/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/home/user42/webserver/tests/test_socket[1]_include.cmake")
include("/home/user42/webserver/tests/test_http[1]_include.cmake")
subdirs("../_deps/googletest-build")
