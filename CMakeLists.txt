#指定CMake最低版本
cmake_minimum_required(VERSION 3.17)

#项目名称
project(QtFrame)

#指定C++版本(Variable Num)
set(CMAKE_CXX_STANDARD 17)

#将当前目录加入环境变量(Variable ON-OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#调用moc、uic、rcc处理Qt扩展(Variable ON-OFF)
set(CMAKE_AUTOMOC ON)   # 元对象编译器
set(CMAKE_AUTOUIC ON)   # 资源文件
set(CMAKE_AUTORCC ON)   # UI文件

#设置MinGW路径(CMAKE_PREFIX_PATH MingwPath)
set(CMAKE_PREFIX_PATH C:/Qt/Qt5.9.0/5.9/mingw53_32)

#添加包路径
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5SerialPort REQUIRED)


#查找./下的所有源文件(FindPath SourceFileList)
aux_source_directory(./ SRC_LIST)
#查找./Cxx_Main下的所有源文件 -->添加到SRC_LIST
aux_source_directory(./Main SRC_LIST)
#查找./Child_CameraConfig下的所有源文件 -->添加到SRC_LIST
aux_source_directory(./Child_CameraConfig SRC_LIST)
aux_source_directory(./Child_SystemConfig SRC_LIST)
aux_source_directory(./CV SRC_LIST)


#生成可执行文件(ExecutableName SourceFileList)
add_executable(QtFrame ${SRC_LIST})

#为项目添加共享库链接(ExecutableName QWidgets)
target_link_libraries(QtFrame Qt5::Widgets ${CMAKE_SOURCE_DIR}/File_Static/MvCameraControlx86.lib ${Qt5SerialPort_LIBRARIES})

#重定义可执行文件的输出路径(EXECUTABLE_OUTPUT_PATH SavePath)
set(EXECUTABLE_OUTPUT_PATH ../)

