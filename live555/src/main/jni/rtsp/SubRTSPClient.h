//
// Created by ShangPingping on 2018/6/21.
//

#ifndef LIVE555_SUB_RTSP_CLIENT_H
#define LIVE555_SUB_RTSP_CLIENT_H


#include <RTSPClient.hh>
#include <UsageEnvironment.hh>
#include <NetAddress.hh>
#include "StreamClientState.h"

/**
 * If you're streaming just a single stream, then you can define and use just a single.
 * "StreamClientState" structure, as a global variable in your application.
 * Instead, we have to have a separate "StreamClientState" structure for each "SubRTSPClient".
 * To do this, we subclass "SubRTSPClient", and add a "StreamClientState" field to the subclass.
 */
class SubRTSPClient: public RTSPClient {
public:
    static SubRTSPClient* createNew(UsageEnvironment& env,
                                    char const* rtspURL,
                                    int verbosityLevel = 0,
                                    char const* applicationName = NULL,
                                    portNumBits tunnelOverHTTPPortNum = 0);

protected:
    SubRTSPClient(UsageEnvironment& env,
                  char const* rtspURL,
                  int verbosityLevel,
                  char const* applicationName,
                  portNumBits tunnelOverHTTPPortNum);
    virtual ~SubRTSPClient();

public:
    StreamClientState scs;
};


#endif // LIVE555_SUB_RTSP_CLIENT_H
