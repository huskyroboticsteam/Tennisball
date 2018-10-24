cmake_minimum_required (VERSION 2.8)
project (tennisball)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( tennisball tennisball.cpp )
target_link_libraries( tennisball ${OpenCV_LIBS} )
