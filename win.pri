CONFIG += release
DESTDIR = release

# DEFINES += TUP_DEBUG HAVE_FFMPEG
DEFINES += HAVE_FFMPEG

DEFINES += TUPITUBE_VERSION=\\\"0.2\\\" REVISION=\\\"19\\\" CODE_NAME=\\\"Aiyra\\\" CONFIG_VERSION=\\\"3\\\"

contains(DEFINES, TUP_DEBUG) {
    CONFIG += console
}
