cmake_minimum_required(VERSION 2.8)

project( Demo )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} ./includes
./includes/libavcodec
./includes/libavfilter
./includes/libavutil
./includes/libswresample
./includes/libavdevice
./includes/libavformat
./includes/libjpeg
./includes/libswscale
 ./ )

aux_source_directory(./ SRC_DIR)
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

