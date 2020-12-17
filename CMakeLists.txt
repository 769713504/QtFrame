#指定CMake最低版本
cmake_minimum_required(VERSION 3.17)

#项目名称
project(QtFrame)

#指定C++版本
set(CMAKE_CXX_STANDARD 17)

#将当前目录加入环境变量
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#调用moc、uic、rcc处理Qt扩展
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#设置MinGW路径
set(CMAKE_PREFIX_PATH C:/Qt/Qt5.9.0/5.9/mingw53_32)

#添加包路径
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

#查找所有源文件,保存变量
aux_source_directory(. SRC_LIST)

#生成可执行文件
add_executable(QtFrame ${SRC_LIST})

#为项目添加共享库链接
target_link_libraries(QtFrame Qt5::Widgets)

#重定义可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ../)

