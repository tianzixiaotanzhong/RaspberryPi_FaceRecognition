cmake_minimum_required(VERSION 2.8)

project( Demo )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} ./include )

aux_source_directory(./source SRC_DIR)

add_executable( Demo ${SRC_DIR} )

target_link_libraries( Demo ${OpenCV_LIBS} )