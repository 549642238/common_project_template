SET(LIB_NAME list)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/${LIB_NAME})

# Generate libs
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
ADD_LIBRARY(${LIB_NAME} ${LIB_NAME}.c)

# Test program
if (GENERATE_TEST_FILE)
	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
	ADD_EXECUTABLE(test_${LIB_NAME} test_${LIB_NAME}.c)
	TARGET_LINK_LIBRARIES(test_${LIB_NAME} ${LIB_NAME})
endif (GENERATE_TEST_FILE)
