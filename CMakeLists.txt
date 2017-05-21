cmake_minimum_required (VERSION 2.6)
project (getoffset)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ~/bin)

include_directories( "${PROJECT_SOURCE_DIR}/../include" )
include_directories( "${PROJECT_SOURCE_DIR}" )
enable_language (Fortran)
ADD_DEFINITIONS(-DENBALE_FORTRAN)
ADD_DEFINITIONS(-DENBALE_OPENMP)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

AUX_SOURCE_DIRECTORY(. SRC_LIST)
SET(EXTRA_SRC_LIST ../sources/CFileSystem.cpp ../sources/RSImage.cpp ../sources/ImageInfo.cpp ../sources/Matrix.cpp )


SET(FORTRAN_SRC ../imcorr/cross.f ../imcorr/esterr.f ../imcorr/eval.f ../imcorr/fft2d.f ../imcorr/fitreg.f ../imcorr/gcorr.f ../imcorr/gnorm.f ../imcorr/kvert.f ../imcorr/sums.f)

FIND_PACKAGE(GDAL REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

message( ${GDAL_LIBRARY} )
message(${SRC_LIST} ${EXTRA_SRC_LIST} ${FORTRAN_SRC} )

ADD_DEFINITIONS(-D__LINUX__)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x ")

add_executable(getoffset ${SRC_LIST} ${EXTRA_SRC_LIST} ${FORTRAN_SRC})
target_link_libraries(getoffset  ${GDAL_LIBRARY} ${OpenCV_LIBS})
