//
// Created by ShangPingping on 2018/5/3.
//

#include <jni.h>
#include <android/log.h>
#include <string>
#include <sstream>
#include <RTSPClient.hh>
#include <UsageEnvironment.hh>
#include <BasicUsageEnvironment.hh>
#include <NetAddress.hh>
#include <SubRTSPClient.h>
#include <DummySink.h>
#include "MyRTSPClient.h"
#include "RTSPCommon.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

extern JNIEnv *_env;
extern jclass _clazz;
extern jobject _obj;
extern jmethodID _methodID1;
extern jmethodID _methodID2;

extern RTSPClient* _rtspClient;
extern unsigned char runningFlag;
extern char eventLoopWatchVariable;
extern Boolean isRunning;
extern string printInfo;

/**
 * Program entry, create rtsp link.
 * @param env
 * @param clazz
 * @param path
 * @return
 */
jint Java_com_example_cynthiaty_live555_rtsp_RtspClient_start(JNIEnv * env, jclass clazz, jstring path) {
    if(isRunning) return -1;

    const char* sdp_title = env->GetStringUTFChars(path, NULL);

    _env = env;
    _clazz = clazz;
    runningFlag = 1;
    isRunning = true;

    _methodID1 = env->GetMethodID(clazz, "onNativeInfo", "(Ljava/lang/String;)V");
    if (_methodID1 == NULL) {
        LOGE("can't find method: public void onNativeInfo(java.lang.String)");
    }

    MyRTSPClient::start(sdp_title);

    env->ReleaseStringUTFChars(path, sdp_title);
    env->DeleteLocalRef(path);

    if (_clazz != NULL && _obj != NULL) {
        _env->DeleteLocalRef(_clazz);
        _env->DeleteLocalRef(_obj);
    }

    _methodID1 = NULL;
    _methodID2 = NULL;

    _clazz = NULL;
    _obj = NULL;

    _env = NULL;
    _rtspClient = NULL;

    return 0;
}

/**
 * Close rtsp link.
 * @param env
 * @param clazz
 */
void Java_com_example_cynthiaty_live555_rtsp_RtspClient_stop(JNIEnv * env, jclass clazz) {
    if(!isRunning) return;

    isRunning = false;
}

/**
 * Start.
 * @param path
 * @return
 */
int MyRTSPClient::start(const char* path) {
    // Begin by setting up our usage environment.
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

    // We need at least one "rtsp://" URL argument.
    if (path == NULL) {
        printInfo = "URL is not allow NULL!";
        RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
        return 1;
    }

    eventLoopWatchVariable = 0;

    // There are argc-1 URLs: argv[1] through argv[argc-1].  Open and start streaming each one:
    MyRTSPClient::openURL(*env, NULL, path);

    // All subsequent activity takes place within the event loop:
    env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
    // This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.

    env->reclaim();
    env = NULL;
    delete scheduler;
    scheduler = NULL;

    return 0;
}

/**
 * Open url from java.
 * @param env
 * @param progName
 * @param rtspURL
 */
void MyRTSPClient::openURL(UsageEnvironment& env, char const* progName, char const* rtspURL) {
    RTSPClient* rtspClient = SubRTSPClient::createNew(env, rtspURL, RTSP_CLIENT_VERBOSITY_LEVEL, progName);
    if (rtspClient == NULL) {
        printInfo = "Failed to create a RTSP client for URL \""
              + string(rtspURL) + "\":"
              + string(env.getResultMsg());
        RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
        return;
    }

    _rtspClient = rtspClient;

    rtspClient->sendDescribeCommand(continueAfterDESCRIBE);
}

/**
 * Continue after DESCRIBE.
 * @param rtspClient
 * @param resultCode
 * @param resultString
 */
void MyRTSPClient::continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString) {
    do {
        UsageEnvironment& env = rtspClient->envir();
        StreamClientState& scs = ((SubRTSPClient*) rtspClient)->scs;

        if (resultCode != 0) {
            printInfo = "Failed to get a SDP description: " + string(resultString);
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
            delete[] resultString;
            break;
        }

        char* const sdpDescription = resultString;
        printInfo = "Got a SDP description:\n" + string(resultString);
        RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));

        // Create a media session object from this SDP description:
        scs.session = MediaSession::createNew(env, sdpDescription);
        delete[] sdpDescription;

        if (scs.session == NULL) {
            printInfo = "Failed to create a MediaSession object from the SDP description: "
                  + string(env.getResultMsg());
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
            break;
        } else if (!scs.session->hasSubsessions()) {
            printInfo = "This session has no media subsessions (i.e., no \"m=\" lines)";
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
            break;
        }

        // Then, create and set up our data source objects for the session.
        scs.iter = new MediaSubsessionIterator(*scs.session);
        setupNextSubsession(rtspClient);
        return;
    } while (0);

    DummySink::shutdownStream(rtspClient);
}

/**
 * Continue after SETUP.
 * @param rtspClient
 * @param resultCode
 * @param resultString
 */
void MyRTSPClient::continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString) {
    do {
        UsageEnvironment& env = rtspClient->envir();
        StreamClientState& scs = ((SubRTSPClient*) rtspClient)->scs;

        if (resultCode != 0) {
            printInfo = "Failed to set up the subsession: " + string(resultString);
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
            break;
        }

        printInfo = "Set up the subsession (";
        ostringstream oss;
        if (scs.subsession->rtcpIsMuxed()) {
            oss << printInfo << "client port " << scs.subsession->clientPortNum() << ")";
            printInfo = oss.str();
        } else {
            oss << printInfo << "client port " << scs.subsession->clientPortNum()
                << "-" << scs.subsession->clientPortNum() + 1 << ")";
            printInfo = oss.str();
        }

        RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));

        // Having successfully setup the subsession, create a data sink for it.
        scs.subsession->sink = DummySink::createNew(env, *scs.subsession, rtspClient->url());

        // perhaps use your own custom "MediaSink" subclass instead.
        if (scs.subsession->sink == NULL) {
            printInfo = "Failed to create a data sink for the subsession: " + string(env.getResultMsg());
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
            break;
        }

        printInfo = "Created a data sink for the subsession";
        RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));

        scs.subsession->miscPtr = rtspClient;
        scs.subsession->sink->startPlaying(*(scs.subsession->readSource()),
                                           subsessionAfterPlaying,
                                           scs.subsession);

        // Also set a handler to be called if a RTCP "BYE" arrives for this subSession.
        if (scs.subsession->rtcpInstance() != NULL) {
            scs.subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, scs.subsession);
        }
    } while (0);

    delete[] resultString;

    setupNextSubsession(rtspClient);
}

/**
 * Continue after PLAY.
 * @param rtspClient
 * @param resultCode
 * @param resultString
 */
void MyRTSPClient::continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString) {
    Boolean success = false;
    ostringstream oss;

    do {
        UsageEnvironment& env = rtspClient->envir();
        StreamClientState& scs = ((SubRTSPClient*) rtspClient)->scs;

        if (resultCode != 0) {
            printInfo = "Failed to start playing session: " + string(resultString);
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
            break;
        }

        // Set a timer to be handled at the end of the stream's expected duration.
        if (scs.duration > 0) {
            unsigned const delaySlop = 2;
            scs.duration += delaySlop;
            unsigned uSecsToDelay = (unsigned) (scs.duration * 1000000);
            scs.streamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay,
                                                            (TaskFunc*) streamTimerHandler,
                                                            rtspClient);
        }

        printInfo = "Started playing session";
        RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));

        if (scs.duration > 0) {
            oss << "(for up to " << scs.duration << " seconds)...";
            printInfo = oss.str();
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
        }

        success = true;
    } while (0);

    delete[] resultString;

    if (!success) {
        DummySink::shutdownStream(rtspClient);
    }
}

/**
 * Used to iterate through each stream's subsessions, setting up each one.
 * @param rtspClient
 */
void MyRTSPClient::setupNextSubsession(RTSPClient* rtspClient) {
    UsageEnvironment& env = rtspClient->envir();
    StreamClientState& scs = ((SubRTSPClient*) rtspClient)->scs;

    scs.subsession = scs.iter->next();
    if (scs.subsession != NULL) {
        if (!scs.subsession->initiate()) {
            printInfo = "Failed to initiate the subsession: " + string(env.getResultMsg());
            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
            setupNextSubsession(rtspClient);
        } else {
            printInfo = "Initiated the subsession (";
            ostringstream oss;
            if (scs.subsession->rtcpIsMuxed()) {
                oss << printInfo << "client port " << scs.subsession->clientPortNum() << ")";
                printInfo = oss.str();
            } else {
                oss << printInfo << "client port " << scs.subsession->clientPortNum()
                    << "-" << scs.subsession->clientPortNum() + 1 << ")";
                printInfo = oss.str();
            }

            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));

            // Continue setting up this subsession, by sending a RTSP "SETUP" command.
            rtspClient->sendSetupCommand(*scs.subsession, continueAfterSETUP, false, REQUEST_STREAMING_OVER_TCP);
        }
        return;
    }

    // We've finished setting up all of the subSessions.
    // Now, send a RTSP "PLAY" command to start the streaming.
    if (scs.session->absStartTime() != NULL) {
        // Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command.
        rtspClient->sendPlayCommand(*scs.session,
                                    continueAfterPLAY,
                                    scs.session->absStartTime(),
                                    scs.session->absEndTime());
    } else {
        scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
        rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY);
    }
}

/**
 * After subsession's data transfer is complete, handle the transaction.
 * @param clientData
 */
void MyRTSPClient::subsessionAfterPlaying(void* clientData) {
    MediaSubsession* subsession = (MediaSubsession*) clientData;
    RTSPClient* rtspClient = (RTSPClient*) (subsession->miscPtr);

    // Begin by closing this subsession's stream.
    Medium::close(subsession->sink);
    subsession->sink = NULL;

    // Next, check whether *all* subsessions' streams have now been closed.
    MediaSession& session = subsession->parentSession();
    MediaSubsessionIterator iter(session);
    while ((subsession = iter.next()) != NULL) {
        if (subsession->sink != NULL) {
			return;
		}
    }

    DummySink::shutdownStream(rtspClient);
}

/**
 * Subsession receives bye, processing the corresponding thing.
 * @param clientData
 */
void MyRTSPClient::subsessionByeHandler(void* clientData) {
    MediaSubsession* subsession = (MediaSubsession*) clientData;
    RTSPClient* rtspClient = (RTSPClient*) subsession->miscPtr;
    UsageEnvironment& env = rtspClient->envir();

    printInfo = "Received RTCP \"BYE\" on subsession";
    RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));

    // Now act as if the subsession had closed.
    subsessionAfterPlaying(subsession);
}

/**
 * Set a timer to be handled at the end of the stream's expected duration.
 * @param clientData
 */
void MyRTSPClient::streamTimerHandler(void* clientData) {
    SubRTSPClient* rtspClient = (SubRTSPClient*) clientData;
    StreamClientState& scs = rtspClient->scs;

    scs.streamTimerTask = NULL;

    DummySink::shutdownStream(rtspClient);
}

#ifdef __cplusplus
}
#endif


