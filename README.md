# common_project_template
通用项目开发模板 【Linux C】

## 项目目录树
```
├── bin
│   └── 可执行文件 + 测试库文件
├── build （自行创建用于存放编译临时文件）
├── CMakeLists.txt
├── include
│   └── common.h
├── lib （库）
│   └── bitmap
└── src （源文件）
    ├── CMakeLists.txt
    └── main.c
```

## 编译
+ 静态编译
```
$ mkdir build
$ cd build
$ cmake ..
$ make -j32
```
+ 动态编译
```
$ mkdir build
$ cd build
$ cmake -D BUILD_SHARED_LIBS=ON ..
$ make -j32
```
+ 编译测试库可执行文件
```
$ mkdir build
$ cd build
$ cmake -D GENERATE_TEST_FILE=ON ..
$ make -j32
```
**建议每次cmake前rm build/\* -rf**
