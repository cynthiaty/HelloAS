//
// Created by ShangPingping on 2018/6/21.
//

#ifndef LIVE555_DUMMY_SINK_H
#define LIVE555_DUMMY_SINK_H


#include <MediaSink.hh>
#include <UsageEnvironment.hh>
#include <MediaSession.hh>
#include <RTSPClient.hh>
#include <linux/time.h>

#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 3000000

/**
 * Define a data sink (a subclass of "MediaSink") to receive the data for each subsession.
 * In practice, this might be a class that decodes and then renders the incoming audio or video.
 * Or it might be a "FileSink", for outputting the received data into a file.
 */
class DummySink: public MediaSink {
public:
    static DummySink* createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId = NULL);
    static void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

private:
    DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);
    virtual ~DummySink();

    static void afterGettingFrame(void* clientData,
                                  unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned durationInMicroseconds);
    void afterGettingFrame(unsigned frameSize,
                           unsigned numTruncatedBytes,
                           struct timeval presentationTime,
                           unsigned durationInMicroseconds);

private:
    virtual Boolean continuePlaying();

private:
    u_int8_t* fReceiveBuffer;
    MediaSubsession& fSubsession;
    char* fStreamId;
};


#endif // LIVE555_DUMMY_SINK_H
