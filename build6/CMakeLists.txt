cmake_minimum_required(VERSION 2.8)

project(lab6)

find_package( libusb REQUIRED )

include_directories( ${libusb_INCLUDE_DIRS} )

add_executable( lab6 lab6.cpp )

target_link_libraries( lab6 ${libusb_LIBS} )
