# Output dir
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Target executable file
SET(exec_name my_project)
ADD_EXECUTABLE(${exec_name} main.c)

# Include files
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
file(GLOB children ${PROJECT_SOURCE_DIR}/lib/*)
FOREACH (child ${children})
	file(RELATIVE_PATH child_name ${PROJECT_SOURCE_DIR}/lib ${child})

	IF (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${child_name})
		INCLUDE_DIRECTORIES(${child})
		# Depend self-making libraries
		TARGET_LINK_LIBRARIES(${exec_name} ${child_name})
	ENDIF ()
ENDFOREACH ()

# Depend system libraries
LINK_DIRECTORIES(/usr/lib /usr/local/lib ${PROJECT_SOURCE_DIR}/lib)
