cmake_minimum_required(VERSION 2.8.3)
project(o_learning)

find_package(catkin REQUIRED COMPONENTS
  qt_build 
  roscpp roslib std_msgs sensor_msgs image_transport cv_bridge
)

find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
set(QT_USE_QTGUI TRUE)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)
set(CMAKE_AUTOMOC ON)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(qt_srcs src/mainwindow.cpp src/rosthread.cpp src/Classifier.cpp src/trainnet.cpp)
set(qt_hdrs src/mainwindow.h src/rosthread.h src/Classifier.h src/trainnet.h)



find_package(Caffe)

#set(CAFFE_INCLUDEDIR ~/caffe/include ~/caffe/distribute/include)
#set(CAFFE_LINK_LIBRARAY ~/caffe/build/lib)

include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})


find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenCV REQUIRED)

QT4_WRAP_CPP(qt_moc_srcs ${qt_hdrs})
QT4_WRAP_UI(uis_h src/mainwindow.ui)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES o_learning
#  CATKIN_DEPENDS qt_build roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)
#include_directories(${CAFFE_INCLUDEDIR})
#link_directories(${CAFFE_LINK_LIBRARAY})

add_executable(test_node ${qt_srcs} src/main.cpp ${uis_h} ${qt_moc_src})
target_link_libraries(test_node ${QT_LIBRARIES} ${ROS_LIBRARIES} ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Caffe_LIBRARIES} glog)

