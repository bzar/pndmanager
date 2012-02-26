cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(CMAKE_CXX_FLAGS "-g -Wall")
#set(CMAKE_CXX_FLAGS "-Wall")

file (GLOB_RECURSE PNDMANAGER_HEADERS src/*.h)
QT4_WRAP_CPP(PNDMANAGER_MOC ${PNDMANAGER_HEADERS})

include_directories(lib/qtpndman/include)
include_directories(lib/qtpndman/lib/libpndman/src/include)

add_subdirectory(lib)

file (GLOB_RECURSE PNDMANAGER_SRCS src/mainwindow.cpp src/*.cpp)
add_executable(pndmanager ${PNDMANAGER_SRCS} ${PNDMANAGER_MOC})
target_link_libraries(pndmanager ${QT_LIBRARIES} qtpndman)
