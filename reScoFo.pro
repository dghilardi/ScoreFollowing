unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += aubio \
    vorbisenc \
    openal \
    freealut \
    opencv
}
include(juce.pri)
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += Source/main.cpp \
    Source/audioStream/ogg_stream.cpp \
    Source/audioStream/mysndfileio.cpp \
    Source/audioStream/oggdecoder.cpp \
    Source/audioStream/midifile.cpp \
    Source/audioStream/midi_stream.cpp

HEADERS += \
    Source/audioStream/ogg_stream.h \
    Source/debugHeader.h \
    Source/audioStream/mysndfileio.h \
    Source/audioStream/oggdecoder.h \
    Source/audioStream/midifile.h \
    Source/audioStream/midi_stream.h

LIBS += -lvorbis -lvorbisfile -lvorbisenc -laubioext

