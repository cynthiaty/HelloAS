LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SRC = \
    liveMedia/Media.cpp \
    liveMedia/MediaSource.cpp \
    liveMedia/FramedSource.cpp \
    liveMedia/FramedFileSource.cpp \
    liveMedia/FramedFilter.cpp \
    liveMedia/RTPSource.cpp \
    liveMedia/MultiFramedRTPSource.cpp \
    liveMedia/SimpleRTPSource.cpp \
    liveMedia/H261VideoRTPSource.cpp \
    liveMedia/H264VideoRTPSource.cpp \
    liveMedia/H265VideoRTPSource.cpp \
    liveMedia/QCELPAudioRTPSource.cpp \
    liveMedia/AMRAudioRTPSource.cpp \
    liveMedia/JPEGVideoRTPSource.cpp \
    liveMedia/VorbisAudioRTPSource.cpp \
    liveMedia/TheoraVideoRTPSource.cpp \
    liveMedia/VP8VideoRTPSource.cpp \
    liveMedia/VP9VideoRTPSource.cpp \
    liveMedia/ByteStreamFileSource.cpp \
    liveMedia/ByteStreamMultiFileSource.cpp \
    liveMedia/ByteStreamMemoryBufferSource.cpp \
    liveMedia/BasicUDPSource.cpp \
    liveMedia/DeviceSource.cpp \
    liveMedia/AudioInputDevice.cpp \
    liveMedia/WAVAudioFileSource.cpp \
    liveMedia/MPEG1or2Demux.cpp \
    liveMedia/MPEG1or2DemuxedElementaryStream.cpp \
    liveMedia/MPEGVideoStreamFramer.cpp \
    liveMedia/MPEG1or2VideoStreamFramer.cpp \
    liveMedia/MPEG1or2VideoStreamDiscreteFramer.cpp \
    liveMedia/MPEG4VideoStreamFramer.cpp \
    liveMedia/MPEG4VideoStreamDiscreteFramer.cpp \
    liveMedia/H264or5VideoStreamFramer.cpp \
    liveMedia/H264or5VideoStreamDiscreteFramer.cpp \
    liveMedia/H264VideoStreamFramer.cpp \
    liveMedia/H264VideoStreamDiscreteFramer.cpp \
    liveMedia/H265VideoStreamFramer.cpp \
    liveMedia/H265VideoStreamDiscreteFramer.cpp \
    liveMedia/MPEGVideoStreamParser.cpp \
    liveMedia/MPEG1or2AudioStreamFramer.cpp \
    liveMedia/MPEG1or2AudioRTPSource.cpp \
    liveMedia/MPEG4LATMAudioRTPSource.cpp \
    liveMedia/MPEG4ESVideoRTPSource.cpp \
    liveMedia/MPEG4GenericRTPSource.cpp \
    liveMedia/MP3FileSource.cpp \
    liveMedia/MP3Transcoder.cpp \
    liveMedia/MP3ADU.cpp \
    liveMedia/MP3ADUdescriptor.cpp \
    liveMedia/MP3ADUinterleaving.cpp \
    liveMedia/MP3ADUTranscoder.cpp \
    liveMedia/MP3StreamState.cpp \
    liveMedia/MP3Internals.cpp \
    liveMedia/MP3InternalsHuffman.cpp \
    liveMedia/MP3InternalsHuffmanTable.cpp \
    liveMedia/MP3ADURTPSource.cpp \
    liveMedia/MPEG1or2VideoRTPSource.cpp \
    liveMedia/MPEG2TransportStreamMultiplexor.cpp \
    liveMedia/MPEG2TransportStreamFromPESSource.cpp \
    liveMedia/MPEG2TransportStreamFromESSource.cpp \
    liveMedia/MPEG2TransportStreamFramer.cpp \
    liveMedia/MPEG2TransportStreamAccumulator.cpp \
    liveMedia/ADTSAudioFileSource.cpp \
    liveMedia/H263plusVideoRTPSource.cpp \
    liveMedia/H263plusVideoStreamFramer.cpp \
    liveMedia/H263plusVideoStreamParser.cpp \
    liveMedia/DVVideoStreamFramer.cpp \
    liveMedia/AC3AudioStreamFramer.cpp \
    liveMedia/AC3AudioRTPSource.cpp \
    liveMedia/DVVideoRTPSource.cpp \
    liveMedia/JPEGVideoSource.cpp \
    liveMedia/AMRAudioSource.cpp \
    liveMedia/AMRAudioFileSource.cpp \
    liveMedia/InputFile.cpp \
    liveMedia/StreamReplicator.cpp \
    liveMedia/MediaSink.cpp \
    liveMedia/FileSink.cpp \
    liveMedia/BasicUDPSink.cpp \
    liveMedia/AMRAudioFileSink.cpp \
    liveMedia/H264or5VideoFileSink.cpp \
    liveMedia/H264VideoFileSink.cpp \
    liveMedia/H265VideoFileSink.cpp \
    liveMedia/MPEG4ESVideoRTPSink.cpp \
    liveMedia/OggFileSink.cpp \
    liveMedia/RTPSink.cpp \
    liveMedia/MultiFramedRTPSink.cpp \
    liveMedia/AudioRTPSink.cpp \
    liveMedia/VideoRTPSink.cpp \
    liveMedia/TextRTPSink.cpp \
    liveMedia/RTPInterface.cpp \
    liveMedia/MPEG1or2AudioRTPSink.cpp \
    liveMedia/MP3ADURTPSink.cpp \
    liveMedia/MPEG1or2VideoRTPSink.cpp \
    liveMedia/MPEG4LATMAudioRTPSink.cpp \
    liveMedia/MPEG4GenericRTPSink.cpp \
    liveMedia/H263plusVideoRTPSink.cpp \
    liveMedia/H264or5VideoRTPSink.cpp \
    liveMedia/H264VideoRTPSink.cpp \
    liveMedia/H265VideoRTPSink.cpp \
    liveMedia/DVVideoRTPSink.cpp \
    liveMedia/AC3AudioRTPSink.cpp \
    liveMedia/VorbisAudioRTPSink.cpp \
    liveMedia/TheoraVideoRTPSink.cpp \
    liveMedia/VP8VideoRTPSink.cpp \
    liveMedia/VP9VideoRTPSink.cpp \
    liveMedia/GSMAudioRTPSink.cpp \
    liveMedia/JPEGVideoRTPSink.cpp \
    liveMedia/SimpleRTPSink.cpp \
    liveMedia/AMRAudioRTPSink.cpp \
    liveMedia/T140TextRTPSink.cpp \
    liveMedia/TCPStreamSink.cpp \
    liveMedia/OutputFile.cpp \
    liveMedia/uLawAudioFilter.cpp \
    liveMedia/MPEG2IndexFromTransportStream.cpp \
    liveMedia/MPEG2TransportStreamIndexFile.cpp \
    liveMedia/MPEG2TransportStreamTrickModeFilter.cpp \
    liveMedia/RTCP.cpp \
    liveMedia/rtcp_from_spec.c \
    liveMedia/GenericMediaServer.cpp \
    liveMedia/RTSPServer.cpp \
    liveMedia/RTSPServerRegister.cpp \
    liveMedia/RTSPClient.cpp \
    liveMedia/RTSPCommon.cpp \
    liveMedia/RTSPServerSupportingHTTPStreaming.cpp \
    liveMedia/RTSPRegisterSender.cpp \
    liveMedia/SIPClient.cpp \
    liveMedia/MediaSession.cpp \
    liveMedia/ServerMediaSession.cpp \
    liveMedia/PassiveServerMediaSubsession.cpp \
    liveMedia/OnDemandServerMediaSubsession.cpp \
    liveMedia/FileServerMediaSubsession.cpp \
    liveMedia/MPEG4VideoFileServerMediaSubsession.cpp \
    liveMedia/H264VideoFileServerMediaSubsession.cpp \
    liveMedia/H265VideoFileServerMediaSubsession.cpp \
    liveMedia/H263plusVideoFileServerMediaSubsession.cpp \
    liveMedia/WAVAudioFileServerMediaSubsession.cpp \
    liveMedia/AMRAudioFileServerMediaSubsession.cpp \
    liveMedia/MP3AudioFileServerMediaSubsession.cpp \
    liveMedia/MPEG1or2VideoFileServerMediaSubsession.cpp \
    liveMedia/MPEG1or2FileServerDemux.cpp \
    liveMedia/MPEG1or2DemuxedServerMediaSubsession.cpp \
    liveMedia/MPEG2TransportFileServerMediaSubsession.cpp \
    liveMedia/ADTSAudioFileServerMediaSubsession.cpp \
    liveMedia/DVVideoFileServerMediaSubsession.cpp \
    liveMedia/AC3AudioFileServerMediaSubsession.cpp \
    liveMedia/MPEG2TransportUDPServerMediaSubsession.cpp \
    liveMedia/ProxyServerMediaSession.cpp \
    liveMedia/QuickTimeFileSink.cpp \
    liveMedia/QuickTimeGenericRTPSource.cpp \
    liveMedia/AVIFileSink.cpp \
    liveMedia/EBMLNumber.cpp \
    liveMedia/MatroskaFile.cpp \
    liveMedia/MatroskaFileParser.cpp \
    liveMedia/MatroskaDemuxedTrack.cpp \
    liveMedia/MatroskaFileServerMediaSubsession.cpp \
    liveMedia/MP3AudioMatroskaFileServerMediaSubsession.cpp \
    liveMedia/MatroskaFileServerDemux.cpp \
    liveMedia/OggFile.cpp \
    liveMedia/OggFileParser.cpp \
    liveMedia/OggDemuxedTrack.cpp \
    liveMedia/OggFileServerMediaSubsession.cpp \
    liveMedia/OggFileServerDemux.cpp \
    liveMedia/BitVector.cpp \
    liveMedia/StreamParser.cpp \
    liveMedia/DigestAuthentication.cpp \
    liveMedia/ourMD5.cpp \
    liveMedia/Base64.cpp \
    liveMedia/Locale.cpp \
    groupsock/GroupsockHelper.cpp \
    groupsock/GroupEId.cpp \
    groupsock/inet.c \
    groupsock/Groupsock.cpp \
    groupsock/NetInterface.cpp \
    groupsock/NetAddress.cpp \
    groupsock/IOHandlers.cpp \
    BasicUsageEnvironment/BasicHashTable.cpp \
    UsageEnvironment/UsageEnvironment.cpp \
    UsageEnvironment/HashTable.cpp \
    UsageEnvironment/strDup.cpp \
    BasicUsageEnvironment/BasicUsageEnvironment0.cpp \
    BasicUsageEnvironment/BasicUsageEnvironment.cpp \
    BasicUsageEnvironment/BasicTaskScheduler0.cpp \
    BasicUsageEnvironment/BasicTaskScheduler.cpp \
    BasicUsageEnvironment/DelayQueue.cpp \

INCLUDES = \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/BasicUsageEnvironment \
    $(LOCAL_PATH)/BasicUsageEnvironment/include \
    $(LOCAL_PATH)/UsageEnvironment \
    $(LOCAL_PATH)/UsageEnvironment/include \
    $(LOCAL_PATH)/groupsock \
    $(LOCAL_PATH)/groupsock/include \
    $(LOCAL_PATH)/liveMedia \
    $(LOCAL_PATH)/liveMedia/include \

EXPORT_INCLUDES = $(INCLUDES)

STATIC_LIBRARIES =

SHARED_LIBRARIES =

LOCAL_MODULE := live555
LOCAL_ARM_MODE := arm
LOCAL_PRELINK_MODULE := false
LOCAL_SRC_FILES := $(SRC)
LOCAL_C_INCLUDES := $(INCLUDES)
LOCAL_EXPORT_C_INCLUDES := $(EXPORT_INCLUDES)
LOCAL_STATIC_LIBRARIES := $(STATIC_LIBRARIES)
LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)

LOCAL_CPPFLAGS := \
    -DNULL=0 -DSOCKLEN_T=socklen_t -DNO_SSTREAM -DBSD=1 -DNO_SSTREAM -fexceptions -DANDROID -DXLOCALE_NOT_USED

LOCAL_MODULE_TAGS := optional

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
