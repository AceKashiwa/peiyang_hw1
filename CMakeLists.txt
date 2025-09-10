# 指定CMake的最低版本要求
cmake_minimum_required (VERSION 3.10)

# 定义项目的名称和使用的编程语言
project (main) 

# 添加头文件搜索路径
include_directories (inc)

# 设置变量的值
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(src SRC_LIST)

# 指定要生成的可执行文件和其源文件
add_executable (main ${SRC_LIST})
