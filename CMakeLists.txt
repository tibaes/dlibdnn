#
# This is a CMake makefile.  You can find the cmake utility and
# information about it at http://www.cmake.org
#

cmake_minimum_required(VERSION 3.4)

PROJECT(weapon)

set(OpenCV_DIR "/usr/local/Cellar/opencv3/3.0.0/share/OpenCV/")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include(../dlib-18.18/dlib/cmake)

# Since there are a lot of examples I'm going to use a macro to simply this
# CMakeLists.txt file.  However, usually you will create only one executable in
# your cmake projects and use the syntax shown above.
MACRO(add_example name)
   ADD_EXECUTABLE(${name} src/${name}.cpp)
   TARGET_LINK_LIBRARIES(${name} dlib ${OpenCV_LIBS})
   set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
ENDMACRO()


#here we apply our macros
add_example(fhog_ex)
add_example(fhog_object_detector_ex)
add_example(video_tracking_ex)
add_example(demo_fhog_track)
add_example(demo_sift_track)
