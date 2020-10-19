CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MY_PROJ)

SET(CMAKE_C_COMPILER "gcc")
ADD_DEFINITIONS("-Wall")
SET(CMAKE_BUILD_TYPE DEBUG)

OPTION(GENERATE_TEST_FILE "Generate libries test files" OFF)
if (NOT BUILD_SHARED_LIBS)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static")
endif (NOT BUILD_SHARED_LIBS)

# Libraries building related
file(GLOB children ${PROJECT_SOURCE_DIR}/lib/*)
FOREACH (child ${children})
	IF (IS_DIRECTORY ${child})
		ADD_SUBDIRECTORY(${child})
	ENDIF ()
ENDFOREACH ()

# Main program building related
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src)
