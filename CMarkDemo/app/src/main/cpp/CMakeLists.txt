# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html


# CMark最低支持版本
cmake_minimum_required(VERSION 3.4.1)



# 定义变量
set(var 123)
message("var = ${var}")


# 定义数组
set(list_var 1 2 3)
set(list_1 "1;2;3;4")
message("list_var=${list_1}")

# if
set(if_tap OFF)
set(elseif_tap ON)
if(${if_tap})
    message("if")
elseif(${elseif_tap})
    message("else if")
endif()

# while
set(a "")
while(NOT a STREQUAL "xxx")
    set(a "${a}x")
    message("a = ${a}")
endwhile()



# 遍历一
foreach(item 1 2 3)
    message("item = ${item}")
endforeach()

# 遍历二 从0开始到3
foreach(item RANGE 3)
    message("item2 = ${item}")
endforeach()

# 遍历三 start stop step
foreach(item RANGE 0 3 2)
    message("item3 ${item}")
endforeach()

# 遍历四
foreach(item IN LISTS list_var)
    message("item 4 ${item}")
endforeach(item)

# 函数定义 ARGV表示所有参数 ARGC表示参数个数
function(func x y z)
    message("call func")
    message("x = ${x}")
    message("y = ${y}")
    message("z = ${z}")
    message("ARGC = ${ARGC}")
    message("ARGV = ${ARGV}")
    message("agr1 = ${ARGV0} agr2 = ${ARGV1} agr3 = ${ARGV2}")
endfunction(func)

# 调用函数
func(1 2 3)

# 宏定义
macro(ma x y z)
    message("call ma")
    message("x = ${x}")
    message("y = ${y}")
    message("z = ${z}")
endmacro(ma)

# 调用宏
ma(3 4 5)


# 设置头文件目录
include_directories(${CMAKE_SOURCE_DIR/inc})



# 添加库，根据native-lib.cpp源文件编译生成native-lib的动态库
add_library(
             native-lib
             SHARED
             native-lib.cpp )
# 设置第三方so库路径 CMAKE_CXX_FLAGS 后面要空一格，不然会编译出错的
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")



# 查找系统库中的log库并赋值给lig-lib
# 系统库路径为/Android/Sdk/ndk/android-ndk-r17c/platforms/android-23/arch-arm/usr/lib
#[[find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )]]

# 设置依赖库 第一个参数必须为目标模块 添加了fmod和fmodL的so库
target_link_libraries( # Specifies the target library.
                       native-lib
                       fmod
                       fmodL
                       log)