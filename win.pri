CONFIG += release
DESTDIR = release

DEFINES += TUP_DEBUG HAVE_FFMPEG
# DEFINES += HAVE_FFMPEG

DEFINES += TUPITUBE_VERSION=\\\"0.2\\\" REVISION=\\\"15\\\" CODE_NAME=\\\"Pytuna\\\" CONFIG_VERSION=\\\"3\\\"

contains(DEFINES, TUP_DEBUG) {
    CONFIG += console
}
