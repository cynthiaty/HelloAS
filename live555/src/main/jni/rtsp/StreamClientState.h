//
// Created by ShangPingping on 2018/6/21.
//

#ifndef LIVE555_STREAM_CLIENT_STATE_H
#define LIVE555_STREAM_CLIENT_STATE_H

#include <MediaSession.hh>
#include <UsageEnvironment.hh>

/**
 * Define a class to hold per-stream state that we maintain throughout each stream's lifetime.
 */
class StreamClientState {
public:
    StreamClientState();
    virtual ~StreamClientState();

public:
    MediaSubsessionIterator* iter;
    MediaSession* session;
    MediaSubsession* subsession;
    TaskToken streamTimerTask;
    double duration;
};

#endif // LIVE555_STREAM_CLIENT_STATE_H
