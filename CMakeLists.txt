#set Cmake minimum version
cmake_minimum_required (VERSION 2.8)
#defines tennisball project
project (tennisball)

#ensure that the bin directory is created (this is where compiled executables go)
file(MAKE_DIRECTORY ./bin)

#make sure that compiled executables are placed in the bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

#locate the OpenCV library
find_package( OpenCV REQUIRED )
#make sure the OpenCV library directory is included when compiling
include_directories( ${OpenCV_INCLUDE_DIRS} )
#add the `live` executable (compiled from `live_video.cpp`)
add_executable( live live_video.cpp )
add_executable( still StillImageTesting.cpp )
#link the OpenCV library
target_link_libraries( live ${OpenCV_LIBS} )
target_link_libraries( still ${OpenCV_LIBS} )
