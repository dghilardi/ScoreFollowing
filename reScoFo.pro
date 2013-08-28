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
    Source/audioStream/pcmStream/oggdecoder.cpp \
    Source/audioStream/midifile.cpp \
    Source/audioStream/pitchStream/midi_stream.cpp \
#    Source/audioStream/pitchStream/pitchstream.cpp \
    Source/audioStream/pcmStream/pcmstream.cpp \
    Source/audioStream/pitchStream/pitchdetect.cpp \
    Source/dtw.cpp \
    Source/Utils/printutils.cpp \
    Source/odtw.cpp

HEADERS += \
    Source/audioStream/ogg_stream.h \
    Source/debugHeader.h \
    Source/audioStream/mysndfileio.h \
    Source/audioStream/pcmStream/oggdecoder.h \
    Source/audioStream/midifile.h \
    Source/audioStream/pitchStream/midi_stream.h \
    Source/audioStream/pitchStream/pitchstream.h \
    Source/audioStream/pcmStream/pcmstream.h \
    Source/audioStream/pitchStream/pitchdetect.h \
    Source/dtw.h \
    Source/Utils/printutils.h \
    Source/settings.h \
    Source/odtw.h

LIBS += -lvorbis -lvorbisfile -lvorbisenc -laubioext

