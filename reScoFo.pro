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

CONFIG(debug,debug|release) {
  message( debug )
} else {
    DEFINES+=NDEBUG
}


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
    Source/odtw/odtw.cpp \
    Source/audioStream/featureStream/features.cpp \
    Source/audioStream/featureStream/featurestream.cpp \
    Source/odtw/pitchodtw.cpp \
    Source/odtw/featureodtw.cpp \
    Source/audioStream/featureStream/featuredetectors.cpp \
    Source/audioStream/Input/micinput.cpp \
    Source/audioStream/Input/AudioBuffer.cpp \
    Source/audioStream/featureStream/pcmfeaturestream.cpp \
    Source/audioStream/featureStream/midifeaturestream.cpp \
    Source/audioStream/featureStream/midifeatures.cpp \
    Source/Testing/accuracytest.cpp

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
    Source/odtw/odtw.h \
    Source/audioStream/featureStream/features.h \
    Source/audioStream/featureStream/featurestream.h \
    Source/odtw/pitchodtw.h \
    Source/odtw/featureodtw.h \
    Source/audioStream/featureStream/featuredetectors.h \
    Source/audioStream/Input/micinput.h \
    Source/audioStream/Input/AudioBuffer.h \
    Source/audioStream/featureStream/pcmfeaturestream.h \
    Source/audioStream/featureStream/midifeaturestream.h \
    Source/audioStream/featureStream/midifeatures.h \
    Source/Testing/accuracytest.h

LIBS += -lvorbis -lvorbisfile -lvorbisenc -laubioext -lportaudiocpp -lpthread -lportaudio -ljsoncpp
