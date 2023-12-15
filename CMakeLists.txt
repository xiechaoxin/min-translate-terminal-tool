#最低要求版本
cmake_minimum_required(VERSION 3.5)
#项目名称
project(TranslateApp)
#设置c++标准
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(src/ SRC_FILES)

#构建可执行文件
add_executable(TranslateApp ${SRC_FILES})


add_executable(myu8 tests/utf8/myu8.cpp)

add_executable(u8txt tests/utf8/u8txt.cpp)
target_include_directories(u8txt PRIVATE include)

