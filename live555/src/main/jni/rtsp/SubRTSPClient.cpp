//
// Created by ShangPingping on 2018/6/21.
//

#include "SubRTSPClient.h"

/**
 * Create.
 * @param env
 * @param rtspURL
 * @param verbosityLevel
 * @param applicationName
 * @param tunnelOverHTTPPortNum
 * @return
 */
SubRTSPClient* SubRTSPClient::createNew(UsageEnvironment& env,
                                        char const* rtspURL,
                                        int verbosityLevel,
                                        char const* applicationName,
                                        portNumBits tunnelOverHTTPPortNum) {
    return new SubRTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

/**
 * Constructor.
 * @param env
 * @param rtspURL
 * @param verbosityLevel
 * @param applicationName
 * @param tunnelOverHTTPPortNum
 */
SubRTSPClient::SubRTSPClient(UsageEnvironment& env,
                             char const* rtspURL,
                             int verbosityLevel,
                             char const* applicationName,
                             portNumBits tunnelOverHTTPPortNum)
        : RTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) {
}

/**
 * Destructor.
 */
SubRTSPClient::~SubRTSPClient() {
}
