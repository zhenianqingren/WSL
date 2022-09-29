基本语法格式：指令(参数 1 参数 2…)

参数使用括弧括起

参数之间使用空格或分号分开

指令是大小写无关的，参数和变量是大小写相关的

1set(HELLO hello.cpp)
2add_executable(hello main.cpp hello.cpp)
3ADD_EXECUTABLE(hello main.cpp ${HELLO})
变量使用${}方式取值，但是在 IF 控制语句中是直接使用变量名

6.3 重要指令和CMake常用变量
6.3.1 重要指令
cmake_minimum_required - 指定CMake的最小版本要求

1# CMake最小版本要求为2.8.3
2cmake_minimum_required(VERSION 2.8.3)
语法：cmake_minimum_required(VERSION versionNumber [FATAL_ERROR])

project - 定义工程名称，并可指定工程支持的语言  

1# 指定工程名为HELLOWORLD
2project(HELLOWORLD)
语法：project(projectname [CXX] [C] [Java])

set - 显式的定义变量  

1# 定义SRC变量，其值为main.cpp hello.cpp
2set(SRC sayhello.cpp hello.cpp)
语法：set(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])

include_directories - 向工程添加多个特定的头文件搜索路径  --->相当于指定g++编译器的-I参数

1# 将/usr/include/myincludefolder 和 ./include 添加到头文件搜索路径
2include_directories(/usr/include/myincludefolder ./include)
语法：include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 …)

link_directories - 向工程添加多个特定的库文件搜索路径  --->相当于指定g++编译器的-L参数

1# 将/usr/lib/mylibfolder 和 ./lib 添加到库文件搜索路径
2link_directories(/usr/lib/mylibfolder ./lib)
语法：link_directories(dir1 dir2 …)  

add_library - 生成库文件

1# 通过变量 SRC 生成 libhello.so 共享库
2add_library(hello SHARED ${SRC})
语法：add_library(libname [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 … sourceN)

add_compile_options - 添加编译参数

1# 添加编译参数 -Wall -std=c++11
2add_compile_options(-Wall -std=c++11 -O2)
语法：add_compile_options(

add_executable - 生成可执行文件

1# 编译main.cpp生成可执行文件main
2add_executable(main main.cpp)
语法：add_library(exename source1 source2 … sourceN)

target_link_libraries - 为 target 添加需要链接的共享库  --->相同于指定g++编译器-l参数

1# 将hello动态库文件链接到可执行文件main
2target_link_libraries(main hello)
语法：target_link_libraries(target library1library2…)

add_subdirectory - 向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制存放的位置

1# 添加src子目录，src中需有一个CMakeLists.txt
2add_subdirectory(src)
语法：add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])

aux_source_directory - 发现一个目录下所有的源代码文件并将列表存储在一个变量中，这个指令临时被用来自动构建源文件列表

1# 定义SRC变量，其值为当前目录下所有的源代码文件
2aux_source_directory(. SRC)
3# 编译SRC变量所代表的源代码文件，生成main可执行文件
4add_executable(main ${SRC})
语法：aux_source_directory(dir VARIABLE)

6.3.2 CMake常用变量
CMAKE_C_FLAGS   gcc编译选项

CMAKE_CXX_FLAGS  g++编译选项

1# 在CMAKE_CXX_FLAGS编译选项后追加-std=c++11
2set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
CMAKE_BUILD_TYPE  编译类型(Debug, Release)

1# 设定编译类型为debug，调试时需要选择debug
2set(CMAKE_BUILD_TYPE Debug) 
3# 设定编译类型为release，发布时需要选择release
4set(CMAKE_BUILD_TYPE Release) 
CMAKE_BINARY_DIR

PROJECT_BINARY_DIR

<projectname>__BINARY_DIR

这三个变量指代的内容是一致的。

如果是 in source build，指的就是工程顶层目录。

如果是 out-of-source 编译,指的是工程编译发生的目录。

PROJECT_BINARY_DIR 跟其他指令稍有区别，不过现在，你可以理解为他们是一致的。

CMAKE_SOURCE_DIR

PROJECT_SOURCE_DIR
<projectname>__SOURCE_DIR

这三个变量指代的内容是一致的,不论采用何种编译方式,都是工程顶层目录。

也就是在 in source build时,他跟 CMAKE_BINARY_DIR 等变量一致。

PROJECT_SOURCE_DIR 跟其他指令稍有区别,现在,你可以理解为他们是一致的。

CMAKE_C_COMPILER：指定C编译器

CMAKE_CXX_COMPILER：指定C++编译器

EXECUTABLE_OUTPUT_PATH：可执行文件输出的存放路径

LIBRARY_OUTPUT_PATH：库文件输出的存放路径

6.4 CMake编译工程
CMake目录结构：项目主目录存在一个CMakeLists.txt文件

两种方式设置编译规则：

包含源文件的子文件夹包含CMakeLists.txt文件，主目录的CMakeLists.txt通过add_subdirectory添加子目录即可；

包含源文件的子文件夹未包含CMakeLists.txt文件，子目录编译规则体现在主目录的CMakeLists.txt中；

6.4.1 编译流程
在 linux 平台下使用 CMake 构建C/C++工程的流程如下:

手动编写 CmakeLists.txt。

执行命令 cmake PATH生成 Makefile ( PATH 是顶层CMakeLists.txt 所在的目录 )。

执行命令make 进行编译。

1# important tips
2.          # 表示当前目录
3./         # 表示当前目录
4
5..      # 表示上级目录
6../     # 表示上级目录
6.4.2 两种构建方式
内部构建(in-source build)：不推荐使用

内部构建会在同级目录下产生一大堆中间文件，这些中间文件并不是我们最终所需要的，和工程源文件放在一起会显得杂乱无章。

1## 内部构建
2
3# 在当前目录下，编译本目录的CMakeLists.txt，生成Makefile和其他文件
4cmake .
5# 执行make命令，生成target
6make
外部构建(out-of-source build)：==推荐使用==

将编译输出文件与源文件放到不同目录中

 1## 外部构建
 2
 3# 1. 在当前目录下，创建build文件夹
 4mkdir build 
 5# 2. 进入到build文件夹
 6cd build
 7# 3. 编译上级目录的CMakeLists.txt，生成Makefile和其他文件
 8cmake ..
 9# 4. 执行make命令，生成target
10make

cmake_minimum_required(VERSION 3.11)

define_source_files（）

Project(CmakeTest)
aux_source_directory(. src)
#add_executable(project1 ${src}) #编译为可执行程序
#add_library(project1 ${src}) #编译为静态库
#add_library(project1 SHARED ${src}) #编译为动态链接库

#add_executable(project1 MACOSX_BUNDLE ${src}) #编译为可执行程序 *.app

#add_library(project1 MODULE ${src}) #编译为程序资源包 *.bundle
#set_target_properties(project1 PROPERTIES BUNDLE TRUE)

#add_library(project1 SHARED ${src}) #编译为程序资源包 *.framework
#set_target_properties(project1 PROPERTIES FRAMEWORK TRUE)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb") # Debug模式下的编译指令
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") # Release模式下的编译指令

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin) #设置可执行文件的输出目录

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib) #设置库文件的输出目录


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)

#上面两条语句分别设置了Debug版本和Release版本可执行文件的输出目录,

#一旦设置上面的属性,在任何环境下生成的可执行文件都将直接放在你所设置的目录.

#四. set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../lib)

#上面两条语句分别设置了Debug版本和Release版本库文件的输出目录,

#一旦设置上面的属性,在任何环境下生成的库文件都将直接放在你所设置的目录
#否则 库文件将默认输出到你运行cmake命令的目录内.
