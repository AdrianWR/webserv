cmake_minimum_required(VERSION 3.16)

project(
	webserv
	VERSION 0.1.0
	DESCRIPTION "HTTP Web Server"
	LANGUAGES CXX
	)

# Options to be triggered only if this is a main project
# (i.e. not a sub-project).
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_STANDARD 98)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -Wall -Wextra -pedantic -Werror")
	#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	include(CTest)
	 # Docs only available if this is the main app
  	find_package(Doxygen)
  	if(Doxygen_FOUND)
    	add_subdirectory(docs)
  	else()
    	message(STATUS "Doxygen not found, not building docs")
  	endif()
endif()


# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14; use our shim
if(${CMAKE_VERSION} VERSION_LESS 3.14)
  include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

# Compiled library code
add_subdirectory(lib)

#Compile the main executable
add_subdirectory(src)


# Testing only available if this is the main app
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

