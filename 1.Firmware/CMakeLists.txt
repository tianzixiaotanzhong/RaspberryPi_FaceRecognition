cmake_minimum_required(VERSION 2.8)

project( Demo )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} ./include 
	#./linux-draw/includes/libavcodec
	#./linux-draw/includes/libavfilter
	#./linux-draw/includes/libavutil
	#./linux-draw/includes/libswresample
	#./linux-draw/includes/libavdevice
	#./linux-draw/includes/libavformat
	#./linux-draw/includes/libjpeg
	#./linux-draw/includes/libswscale
./linux-draw/includes
./linux-draw
)

aux_source_directory(./source SRC_DIR)
aux_source_directory(./linux-draw LINUX_DRAW)

link_directories("/home/pi/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux")
add_executable( Demo ${SRC_DIR} ${LINUX_DRAW})
MESSAGE(STATUS "this var key = ${LINUX_DRAW}")
target_link_libraries( Demo ${OpenCV_LIBS}
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
