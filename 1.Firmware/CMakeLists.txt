cmake_minimum_required(VERSION 2.8)

project( Demo )
set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} ./include 
./linux-draw/includes/libavcodec
./linux-draw/includes/libavfilter
./linux-draw/includes/libavutil
./linux-draw/includes/libswresample
./linux-draw/includes/libavdevice
./linux-draw/includes/libavformat
./linux-draw/includes/libjpeg
./linux-draw/includes/libswscale
./linux-draw
)

aux_source_directory(./source SRC_DIR)

link_directories("/home/pi/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux")
add_executable( Demo ${SRC_DIR} )

target_link_libraries( Demo ${OpenCV_LIB}
jpeg
avformat
avcodec
avutil
swscale
avdevice
swresample
m
z
bz2
pthread
)
