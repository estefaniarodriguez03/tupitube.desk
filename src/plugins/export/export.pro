CONFIG += ordered warn_on
TEMPLATE = subdirs

unix {
    !include(../../../tupiglobal.pri) {
        error("Please configure first")
    }
}

win32 {
    include(../../../win.pri)
}

SUBDIRS += imageplugin 

# SMIL plugin will be disabled temporary 
# smileplugin 

!contains(DEFINES, TUP_32BIT) {
    contains(DEFINES, HAVE_FFMPEG) {
            SUBDIRS += ffmpegplugin
    }
}

# linux-g++ {
#    contains(DEFINES, HAVE_THEORA) {
#        SUBDIRS += theoraplugin
#    }
# }

# Experimental code
contains(DEFINES, HAVE_APNG) {
         SUBDIRS += apngplugin
}
