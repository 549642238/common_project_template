# Include files
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/bitmap)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/min_heap)

# Output dir
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Target executable file
ADD_EXECUTABLE(my_project main.c)

# Depend libraries
LINK_DIRECTORIES(/usr/lib /usr/local/lib ${PROJECT_SOURCE_DIR}/lib)
TARGET_LINK_LIBRARIES(my_project bitmap min_heap)
