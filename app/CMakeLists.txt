# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.6.0)

project("licenseLibrary")

add_library(
        license_tool
        SHARED
        src/main/cpp/main.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
#CmakeLists.txt翻译:搜索指定构建库并将变量作为存储路径。
#因为Cmake构建工具默认包含了系统类库，你仅仅需要指定你想要添加的公共NDK类库的name.
#CMake构建工具会在完成构建之前校验指定的类库name是否存在

# 将NDK log类库的位置存储到变量 log-lib中
#可以在构建脚本的其他地方使用这个变量 ${log-lib}
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

target_link_libraries(
        license_tool

        # Links the target library to the log library
        # included in the NDK.
        #需要在目标库文件中使用的库，表示可以在native-lib中使用log-lib库的内容
        ${log-lib})