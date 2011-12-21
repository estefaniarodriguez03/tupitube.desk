# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/animation
# Target is a library: animation 

HEADERS += ktviewcamera.h \
           ktcamerabar.h \
           ktanimationarea.h \
           # ktcamerawidget.h \
           ktcamerastatus.h \
           postdialog.h
SOURCES += ktviewcamera.cpp \
           ktcamerabar.cpp \
           ktanimationarea.cpp \
           # ktcamerawidget.cpp \
           ktcamerastatus.cpp \
           postdialog.cpp

CONFIG += static warn_on
TEMPLATE = lib
TARGET = animation 

FRAMEWORK_DIR = "../../framework"
include($$FRAMEWORK_DIR/framework.pri)
include(../components_config.pri)
