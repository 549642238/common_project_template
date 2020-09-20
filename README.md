# common_project_template
通用项目开发模板 【Linux C】

## 编译
+ 静态编译
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
+ 动态编译
```
$ mkdir build
$ cd build
$ cmake -D BUILD_SHARED_LIBS=ON ..
$ make
```
**建议每次cmake前rm build/* -rf**
