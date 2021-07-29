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
link_libraries(
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libavcodec
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libavformat
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libbz2
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libswresample
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libavdevice
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libavutil
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libjpeg
#~/opencv-project/RaspberryPi_FaceRecognition/1.Firmware/linux-draw/libs/linux/libswscale
)

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
#target_link_libraries( Demo libavcodec)
#target_link_libraries( Demo libavformat)
#target_link_libraries( Demo libbz2)
#target_link_libraries( Demo libswresample)
#target_link_libraries( Demo libavdevice)
#target_link_libraries( Demo libavutil)
#target_link_libraries( Demo libjpeg)
#target_link_libraries( Demo libswscale)
