cmake_minimum_required (VERSION 2.6)
project (test_gdal_opencv)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ~/codes/testGdal_Opencv)
set(GDAL_INCLUDE_DIR ~/anaconda2/include )

include_directories( "${PROJECT_SOURCE_DIR}" )

#include_directories( "/home/lchuang/anaconda2/include" )
#link_directories( "/home/lchuang/anaconda2/lib")

AUX_SOURCE_DIRECTORY(. SRC_LIST)
#SET(EXTRA_SRC_LIST ../sources/CFileSystem.cpp ../sources/RSImage.cpp ../sources/ImageInfo.cpp )

FIND_PACKAGE(GDAL REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

message( ${GDAL_LIBRARY} )
message(${SRC_LIST} ${EXTRA_SRC_LIST} ${FORTRAN_SRC} )

ADD_DEFINITIONS(-D__LINUX__)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x ")

add_executable(test_gdal_opencv ${SRC_LIST}  )
#${EXTRA_SRC_LIST}
target_link_libraries(test_gdal_opencv  ${GDAL_LIBRARY} ${OpenCV_LIBS})
