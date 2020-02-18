cmake_minimum_required(VERSION 3.5)
project(Proj1-NOTE CXX)

# optional strict mode
if(UNIX AND STRICT)
  message("-- Enabling strict compilation mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

add_executable(note note.cpp)
set_property(TARGET note PROPERTY CXX_STANDARD 11)
