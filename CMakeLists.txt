CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MY_PROJ)

SET(CMAKE_C_COMPILER "gcc")

OPTION(COMPILE_STATIC "Generate static libries and link static" OFF)

# Libraries building related
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/lib/bitmap)

# Main program building related
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src)
