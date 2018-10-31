cmake_minimum_required (VERSION 2.8)
project (tennisball)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( live live_video.cpp )
target_link_libraries( live ${OpenCV_LIBS} )
