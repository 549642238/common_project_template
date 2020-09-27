# lib文件夹用途
**定义并使用自定义库**  
## 添加自定义库的方法
+ Step 1. 以库名新建文件夹，例如"bitmap"
+ Step 2. 在库文件夹内新建库的声明/定义文件和实现文件，以库名作为文件名，例如bitmap.h/bitmap.c
+ Step 3. 在库文件夹内新建库测试文件，以"test_库名"作为文件名，例如test_bitmap.c
+ Step 4. 编写CMakeList
```
SET(LIB_NAME bitmap)                                         # 库名
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
```

## 如何使用自定义库
+ Step 1. src/main.c或src下其它项目文件包含对应库的头文件
+ Step 2. 参考对应库的test文件使用库提供的接口函数
