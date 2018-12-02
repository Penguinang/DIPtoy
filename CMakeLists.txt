cmake_minimum_required(VERSION 3.4)

project(Toy)
set(TOY_VERSION_MAJOE 1)
set(TOY_VERSION_MINOR 0)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

link_libraries(m)
include_directories(include/stb)

# 空间滤波
set(space_exectuables 
    convolution
    average-filter
    median
    laplace)

foreach(exectuable ${space_exectuables})
    add_executable(${exectuable} src/space-filter/${exectuable}.c)
endforeach(exectuable)

# 灰度变换
set(grayscale_exectuables
    bit_layer
    invert
    log)

add_subdirectory(src/grayscale-transform)
foreach(exectuable ${grayscale_exectuables})
    add_executable(${exectuable} src/grayscale-transform/${exectuable}.c)    
    target_link_libraries(${exectuable} Grayscale)
endforeach(exectuable)

# 其他
set(other_exectuables
    gray
    noise
    add
    test)
foreach(exectuable ${other_exectuables})
    add_executable(${exectuable} src/others/${exectuable}.c)
endforeach(exectuable)



# 检查第三方库头文件
include(CheckIncludeFiles)
CHECK_INCLUDE_FILES("stb_image_write.h;stb_image.h" Header_found)
find_file(STBW stb_image_write.h PATHS include/stb)
find_file(STB stb_image.h PATHS include/stb)

if(Header_found OR (STBW AND STB))
    message("You have downloaded stb header from https://github.com/nothings/stb")
else(Header_found OR (STBW AND STB))
    message("You need to download stb header from https://github.com/nothings/stb")
    exec_program("wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -P include/stb -x")
    exec_program("wget https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h -P include/stb -x")
endif(Header_found OR (STBW AND STB))