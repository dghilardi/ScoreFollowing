unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += aubio \
    vorbisenc \
    openal \
    freealut \
    opencv
}
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    audioStream/ogg_stream.cpp \
    audioStream/mysndfileio.cpp \
    audioStream/oggdecoder.cpp

HEADERS += \
    audioStream/ogg_stream.h \
    audioStream/debugHeader.h \
    audioStream/mysndfileio.h \
    audioStream/oggdecoder.h

LIBS += -lvorbis -lvorbisfile -lvorbisenc -laubioext

