######################################################################
# Automatically generated by qmake (1.07a) Fri Jan 20 09:41:23 2006
######################################################################

TEMPLATE = app
CONFIG -= moc

macx {
    CONFIG -= app_bundle
    CONFIG += warn_on static console
}

INCLUDEPATH += .
DEFINES += __STDC_CONSTANT_MACROS

LIBS += -lavformat -lavcodec -lavutil
# Input
SOURCES += main.cpp
