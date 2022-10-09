cmake_minimum_required(VERSION 3.10)

project(gcaplus)
#set(THREADS_PREFER_PTHREAD_FLAG ON)

add_library(gcaplus STATIC GCAdapter.cpp)

# grab header files from this dir
target_include_directories(gcaplus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

find_package(Threads REQUIRED)
target_link_libraries(gcaplus PRIVATE Threads::Threads usb-1.0)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(libusblib REQUIRED libusb-1.0)

#target_link_libraries(gcapluslib PRIVATE libusb-1.0)

#find_package(usb-1.0 REQUIRED)
#target_link_libraries(gcapluslib )

