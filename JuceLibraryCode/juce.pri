TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += modules/juce_core/zip/zlib/infback.c \
modules/juce_core/zip/zlib/trees.c \
modules/juce_core/zip/zlib/crc32.c \
modules/juce_core/zip/zlib/inftrees.c \
modules/juce_core/zip/zlib/zutil.c \
modules/juce_core/zip/zlib/adler32.c \
modules/juce_core/zip/zlib/uncompr.c \
modules/juce_core/zip/zlib/inflate.c \
modules/juce_core/zip/zlib/inffast.c \
modules/juce_core/zip/zlib/compress.c \
modules/juce_core/zip/zlib/deflate.c \
modules/juce_core/containers/juce_PropertySet.cpp \
modules/juce_core/containers/juce_Variant.cpp \
modules/juce_core/containers/juce_DynamicObject.cpp \
modules/juce_core/containers/juce_NamedValueSet.cpp \
modules/juce_core/containers/juce_AbstractFifo.cpp \
modules/juce_core/native/juce_linux_SystemStats.cpp \
modules/juce_core/native/juce_android_Network.cpp \
modules/juce_core/native/juce_android_Files.cpp \
modules/juce_core/native/juce_win32_Threads.cpp \
modules/juce_core/native/juce_win32_Files.cpp \
modules/juce_core/native/juce_android_Misc.cpp \
modules/juce_core/native/juce_linux_Threads.cpp \
modules/juce_core/native/juce_linux_Files.cpp \
modules/juce_core/native/juce_win32_SystemStats.cpp \
modules/juce_core/native/juce_android_Threads.cpp \
modules/juce_core/native/juce_win32_Network.cpp \
modules/juce_core/native/juce_posix_NamedPipe.cpp \
modules/juce_core/native/juce_linux_Network.cpp \
modules/juce_core/native/juce_win32_Registry.cpp \
modules/juce_core/native/juce_android_SystemStats.cpp \
modules/juce_core/maths/juce_Expression.cpp \
modules/juce_core/maths/juce_Random.cpp \
modules/juce_core/maths/juce_BigInteger.cpp \
modules/juce_core/time/juce_Time.cpp \
modules/juce_core/time/juce_RelativeTime.cpp \
modules/juce_core/time/juce_PerformanceCounter.cpp \
modules/juce_core/juce_core.cpp \
modules/juce_core/text/juce_StringArray.cpp \
modules/juce_core/text/juce_CharacterFunctions.cpp \
modules/juce_core/text/juce_StringPairArray.cpp \
modules/juce_core/text/juce_TextDiff.cpp \
modules/juce_core/text/juce_LocalisedStrings.cpp \
modules/juce_core/text/juce_Identifier.cpp \
modules/juce_core/text/juce_StringPool.cpp \
modules/juce_core/text/juce_String.cpp \
modules/juce_core/json/juce_JSON.cpp \
modules/juce_core/zip/juce_GZIPDecompressorInputStream.cpp \
modules/juce_core/zip/juce_GZIPCompressorOutputStream.cpp \
modules/juce_core/zip/juce_ZipFile.cpp \
modules/juce_core/network/juce_Socket.cpp \
modules/juce_core/network/juce_NamedPipe.cpp \
modules/juce_core/network/juce_URL.cpp \
modules/juce_core/network/juce_IPAddress.cpp \
modules/juce_core/network/juce_MACAddress.cpp \
modules/juce_core/logging/juce_FileLogger.cpp \
modules/juce_core/logging/juce_Logger.cpp \
modules/juce_core/streams/juce_FileInputSource.cpp \
modules/juce_core/streams/juce_SubregionStream.cpp \
modules/juce_core/streams/juce_MemoryInputStream.cpp \
modules/juce_core/streams/juce_InputStream.cpp \
modules/juce_core/streams/juce_BufferedInputStream.cpp \
modules/juce_core/streams/juce_OutputStream.cpp \
modules/juce_core/streams/juce_MemoryOutputStream.cpp \
modules/juce_core/threads/juce_Thread.cpp \
modules/juce_core/threads/juce_ChildProcess.cpp \
modules/juce_core/threads/juce_HighResolutionTimer.cpp \
modules/juce_core/threads/juce_ReadWriteLock.cpp \
modules/juce_core/threads/juce_TimeSliceThread.cpp \
modules/juce_core/threads/juce_ThreadPool.cpp \
modules/juce_core/files/juce_FileInputStream.cpp \
modules/juce_core/files/juce_TemporaryFile.cpp \
modules/juce_core/files/juce_FileOutputStream.cpp \
modules/juce_core/files/juce_DirectoryIterator.cpp \
modules/juce_core/files/juce_FileSearchPath.cpp \
modules/juce_core/files/juce_File.cpp \
modules/juce_core/unit_tests/juce_UnitTest.cpp \
modules/juce_core/system/juce_SystemStats.cpp \
modules/juce_core/misc/juce_Uuid.cpp \
modules/juce_core/misc/juce_Result.cpp \
modules/juce_core/memory/juce_MemoryBlock.cpp \
modules/juce_core/xml/juce_XmlElement.cpp \
modules/juce_core/xml/juce_XmlDocument.cpp \
modules/juce_audio_basics/juce_audio_basics.cpp \
modules/juce_audio_basics/buffers/juce_FloatVectorOperations.cpp \
modules/juce_audio_basics/buffers/juce_AudioDataConverters.cpp \
modules/juce_audio_basics/buffers/juce_AudioSampleBuffer.cpp \
modules/juce_audio_basics/synthesisers/juce_Synthesiser.cpp \
modules/juce_audio_basics/midi/juce_MidiBuffer.cpp \
modules/juce_audio_basics/midi/juce_MidiKeyboardState.cpp \
modules/juce_audio_basics/midi/juce_MidiMessageSequence.cpp \
modules/juce_audio_basics/midi/juce_MidiMessage.cpp \
modules/juce_audio_basics/midi/juce_MidiFile.cpp \
modules/juce_audio_basics/sources/juce_MixerAudioSource.cpp \
modules/juce_audio_basics/sources/juce_ResamplingAudioSource.cpp \
modules/juce_audio_basics/sources/juce_ToneGeneratorAudioSource.cpp \
modules/juce_audio_basics/sources/juce_ReverbAudioSource.cpp \
modules/juce_audio_basics/sources/juce_BufferingAudioSource.cpp \
modules/juce_audio_basics/sources/juce_IIRFilterAudioSource.cpp \
modules/juce_audio_basics/sources/juce_ChannelRemappingAudioSource.cpp \
modules/juce_audio_basics/effects/juce_LagrangeInterpolator.cpp \
modules/juce_audio_basics/effects/juce_IIRFilter.cpp \

HEADERS += JuceHeader.h \
AppConfig.h \
modules/juce_core/containers/juce_ReferenceCountedArray.h \
modules/juce_core/containers/juce_Array.h \
modules/juce_core/containers/juce_ArrayAllocationBase.h \
modules/juce_core/containers/juce_SortedSet.h \
modules/juce_core/containers/juce_LinkedListPointer.h \
modules/juce_core/containers/juce_ElementComparator.h \
modules/juce_core/containers/juce_Variant.h \
modules/juce_core/containers/juce_OwnedArray.h \
modules/juce_core/containers/juce_SparseSet.h \
modules/juce_core/containers/juce_DynamicObject.h \
modules/juce_core/containers/juce_NamedValueSet.h \
modules/juce_core/containers/juce_AbstractFifo.h \
modules/juce_core/containers/juce_PropertySet.h \
modules/juce_core/containers/juce_ScopedValueSetter.h \
modules/juce_core/containers/juce_HashMap.h \
modules/juce_core/native/juce_posix_SharedCode.h \
modules/juce_core/native/juce_BasicNativeHeaders.h \
modules/juce_core/native/juce_win32_ComSmartPtr.h \
modules/juce_core/native/juce_android_JNIHelpers.h \
modules/juce_core/native/juce_osx_ObjCHelpers.h \
modules/juce_core/maths/juce_MathsFunctions.h \
modules/juce_core/maths/juce_BigInteger.h \
modules/juce_core/maths/juce_Random.h \
modules/juce_core/maths/juce_Range.h \
modules/juce_core/maths/juce_Expression.h \
modules/juce_core/time/juce_Time.h \
modules/juce_core/time/juce_RelativeTime.h \
modules/juce_core/time/juce_PerformanceCounter.h \
modules/juce_core/text/juce_String.h \
modules/juce_core/text/juce_CharacterFunctions.h \
modules/juce_core/text/juce_StringArray.h \
modules/juce_core/text/juce_NewLine.h \
modules/juce_core/text/juce_LocalisedStrings.h \
modules/juce_core/text/juce_CharPointer_UTF16.h \
modules/juce_core/text/juce_StringPool.h \
modules/juce_core/text/juce_StringPairArray.h \
modules/juce_core/text/juce_TextDiff.h \
modules/juce_core/text/juce_CharPointer_UTF32.h \
modules/juce_core/text/juce_CharPointer_UTF8.h \
modules/juce_core/text/juce_CharPointer_ASCII.h \
modules/juce_core/text/juce_Identifier.h \
modules/juce_core/json/juce_JSON.h \
modules/juce_core/zip/juce_ZipFile.h \
modules/juce_core/zip/zlib/inftrees.h \
modules/juce_core/zip/zlib/crc32.h \
modules/juce_core/zip/zlib/inffast.h \
modules/juce_core/zip/zlib/inflate.h \
modules/juce_core/zip/zlib/zconf.h \
modules/juce_core/zip/zlib/zlib.h \
modules/juce_core/zip/zlib/inffixed.h \
modules/juce_core/zip/zlib/deflate.h \
modules/juce_core/zip/zlib/trees.h \
modules/juce_core/zip/zlib/zutil.h \
modules/juce_core/zip/zlib/zconf.in.h \
modules/juce_core/zip/juce_GZIPCompressorOutputStream.h \
modules/juce_core/zip/juce_GZIPDecompressorInputStream.h \
modules/juce_core/network/juce_NamedPipe.h \
modules/juce_core/network/juce_MACAddress.h \
modules/juce_core/network/juce_URL.h \
modules/juce_core/network/juce_IPAddress.h \
modules/juce_core/network/juce_Socket.h \
modules/juce_core/logging/juce_FileLogger.h \
modules/juce_core/logging/juce_Logger.h \
modules/juce_core/streams/juce_MemoryInputStream.h \
modules/juce_core/streams/juce_OutputStream.h \
modules/juce_core/streams/juce_InputSource.h \
modules/juce_core/streams/juce_SubregionStream.h \
modules/juce_core/streams/juce_InputStream.h \
modules/juce_core/streams/juce_MemoryOutputStream.h \
modules/juce_core/streams/juce_FileInputSource.h \
modules/juce_core/streams/juce_BufferedInputStream.h \
modules/juce_core/threads/juce_ScopedReadLock.h \
modules/juce_core/threads/juce_CriticalSection.h \
modules/juce_core/threads/juce_WaitableEvent.h \
modules/juce_core/threads/juce_ScopedWriteLock.h \
modules/juce_core/threads/juce_ThreadLocalValue.h \
modules/juce_core/threads/juce_ScopedLock.h \
modules/juce_core/threads/juce_HighResolutionTimer.h \
modules/juce_core/threads/juce_Process.h \
modules/juce_core/threads/juce_InterProcessLock.h \
modules/juce_core/threads/juce_ReadWriteLock.h \
modules/juce_core/threads/juce_TimeSliceThread.h \
modules/juce_core/threads/juce_SpinLock.h \
modules/juce_core/threads/juce_DynamicLibrary.h \
modules/juce_core/threads/juce_ThreadPool.h \
modules/juce_core/threads/juce_ChildProcess.h \
modules/juce_core/threads/juce_Thread.h \
modules/juce_core/files/juce_File.h \
modules/juce_core/files/juce_FileSearchPath.h \
modules/juce_core/files/juce_TemporaryFile.h \
modules/juce_core/files/juce_FileInputStream.h \
modules/juce_core/files/juce_FileOutputStream.h \
modules/juce_core/files/juce_DirectoryIterator.h \
modules/juce_core/files/juce_MemoryMappedFile.h \
modules/juce_core/unit_tests/juce_UnitTest.h \
modules/juce_core/system/juce_TargetPlatform.h \
modules/juce_core/system/juce_SystemStats.h \
modules/juce_core/system/juce_StandardHeader.h \
modules/juce_core/system/juce_PlatformDefs.h \
modules/juce_core/misc/juce_WindowsRegistry.h \
modules/juce_core/misc/juce_Result.h \
modules/juce_core/misc/juce_Uuid.h \
modules/juce_core/memory/juce_OptionalScopedPointer.h \
modules/juce_core/memory/juce_ScopedPointer.h \
modules/juce_core/memory/juce_Singleton.h \
modules/juce_core/memory/juce_MemoryBlock.h \
modules/juce_core/memory/juce_LeakedObjectDetector.h \
modules/juce_core/memory/juce_ByteOrder.h \
modules/juce_core/memory/juce_Atomic.h \
modules/juce_core/memory/juce_ReferenceCountedObject.h \
modules/juce_core/memory/juce_Memory.h \
modules/juce_core/memory/juce_WeakReference.h \
modules/juce_core/memory/juce_HeapBlock.h \
modules/juce_core/juce_core.h \
modules/juce_core/xml/juce_XmlDocument.h \
modules/juce_core/xml/juce_XmlElement.h \
modules/juce_audio_basics/buffers/juce_FloatVectorOperations.h \
modules/juce_audio_basics/buffers/juce_AudioDataConverters.h \
modules/juce_audio_basics/buffers/juce_AudioSampleBuffer.h \
modules/juce_audio_basics/synthesisers/juce_Synthesiser.h \
modules/juce_audio_basics/midi/juce_MidiMessage.h \
modules/juce_audio_basics/midi/juce_MidiMessageSequence.h \
modules/juce_audio_basics/midi/juce_MidiKeyboardState.h \
modules/juce_audio_basics/midi/juce_MidiFile.h \
modules/juce_audio_basics/midi/juce_MidiBuffer.h \
modules/juce_audio_basics/sources/juce_AudioSource.h \
modules/juce_audio_basics/sources/juce_IIRFilterAudioSource.h \
modules/juce_audio_basics/sources/juce_ChannelRemappingAudioSource.h \
modules/juce_audio_basics/sources/juce_ToneGeneratorAudioSource.h \
modules/juce_audio_basics/sources/juce_MixerAudioSource.h \
modules/juce_audio_basics/sources/juce_PositionableAudioSource.h \
modules/juce_audio_basics/sources/juce_ResamplingAudioSource.h \
modules/juce_audio_basics/sources/juce_ReverbAudioSource.h \
modules/juce_audio_basics/sources/juce_BufferingAudioSource.h \
modules/juce_audio_basics/effects/juce_LagrangeInterpolator.h \
modules/juce_audio_basics/effects/juce_Decibels.h \
modules/juce_audio_basics/effects/juce_IIRFilter.h \
modules/juce_audio_basics/effects/juce_Reverb.h \
modules/juce_audio_basics/juce_audio_basics.h \
