# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = F:\k210\cmake\bin\cmake.exe

# The command to remove a file.
RM = F:\k210\cmake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\WangZX\Desktop\test\FaceRecognition

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\WangZX\Desktop\test\FaceRecognition\build

# Include any dependencies generated for this target.
include CMakeFiles/Demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Demo.dir/flags.make

CMakeFiles/Demo.dir/source/main.cpp.obj: CMakeFiles/Demo.dir/flags.make
CMakeFiles/Demo.dir/source/main.cpp.obj: CMakeFiles/Demo.dir/includes_CXX.rsp
CMakeFiles/Demo.dir/source/main.cpp.obj: ../source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\WangZX\Desktop\test\FaceRecognition\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Demo.dir/source/main.cpp.obj"
	F:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Demo.dir\source\main.cpp.obj -c C:\Users\WangZX\Desktop\test\FaceRecognition\source\main.cpp

CMakeFiles/Demo.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Demo.dir/source/main.cpp.i"
	F:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\WangZX\Desktop\test\FaceRecognition\source\main.cpp > CMakeFiles\Demo.dir\source\main.cpp.i

CMakeFiles/Demo.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Demo.dir/source/main.cpp.s"
	F:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\WangZX\Desktop\test\FaceRecognition\source\main.cpp -o CMakeFiles\Demo.dir\source\main.cpp.s

CMakeFiles/Demo.dir/source/my_csv.cpp.obj: CMakeFiles/Demo.dir/flags.make
CMakeFiles/Demo.dir/source/my_csv.cpp.obj: CMakeFiles/Demo.dir/includes_CXX.rsp
CMakeFiles/Demo.dir/source/my_csv.cpp.obj: ../source/my_csv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\WangZX\Desktop\test\FaceRecognition\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Demo.dir/source/my_csv.cpp.obj"
	F:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Demo.dir\source\my_csv.cpp.obj -c C:\Users\WangZX\Desktop\test\FaceRecognition\source\my_csv.cpp

CMakeFiles/Demo.dir/source/my_csv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Demo.dir/source/my_csv.cpp.i"
	F:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\WangZX\Desktop\test\FaceRecognition\source\my_csv.cpp > CMakeFiles\Demo.dir\source\my_csv.cpp.i

CMakeFiles/Demo.dir/source/my_csv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Demo.dir/source/my_csv.cpp.s"
	F:\mingw64\bin\x86_64-w64-mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\WangZX\Desktop\test\FaceRecognition\source\my_csv.cpp -o CMakeFiles\Demo.dir\source\my_csv.cpp.s

# Object files for target Demo
Demo_OBJECTS = \
"CMakeFiles/Demo.dir/source/main.cpp.obj" \
"CMakeFiles/Demo.dir/source/my_csv.cpp.obj"

# External object files for target Demo
Demo_EXTERNAL_OBJECTS =

Demo.exe: CMakeFiles/Demo.dir/source/main.cpp.obj
Demo.exe: CMakeFiles/Demo.dir/source/my_csv.cpp.obj
Demo.exe: CMakeFiles/Demo.dir/build.make
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_gapi453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_stitching453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_aruco453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_barcode453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_bgsegm453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_bioinspired453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_ccalib453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_dnn_objdetect453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_dnn_superres453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_dpm453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_face453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_fuzzy453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_hfs453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_img_hash453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_intensity_transform453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_line_descriptor453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_mcc453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_quality453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_rapid453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_reg453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_rgbd453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_saliency453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_stereo453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_structured_light453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_superres453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_surface_matching453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_tracking453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_videostab453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_wechat_qrcode453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_xfeatures2d453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_xobjdetect453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_xphoto453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_shape453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_highgui453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_datasets453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_plot453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_text453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_ml453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_phase_unwrapping453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_optflow453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_ximgproc453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_video453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_videoio453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_dnn453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_imgcodecs453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_objdetect453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_calib3d453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_features2d453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_flann453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_photo453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_imgproc453.dll.a
Demo.exe: F:/mingw64/install/x64/mingw/lib/libopencv_core453.dll.a
Demo.exe: CMakeFiles/Demo.dir/linklibs.rsp
Demo.exe: CMakeFiles/Demo.dir/objects1.rsp
Demo.exe: CMakeFiles/Demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\WangZX\Desktop\test\FaceRecognition\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Demo.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Demo.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Demo.dir/build: Demo.exe

.PHONY : CMakeFiles/Demo.dir/build

CMakeFiles/Demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Demo.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Demo.dir/clean

CMakeFiles/Demo.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\WangZX\Desktop\test\FaceRecognition C:\Users\WangZX\Desktop\test\FaceRecognition C:\Users\WangZX\Desktop\test\FaceRecognition\build C:\Users\WangZX\Desktop\test\FaceRecognition\build C:\Users\WangZX\Desktop\test\FaceRecognition\build\CMakeFiles\Demo.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Demo.dir/depend

