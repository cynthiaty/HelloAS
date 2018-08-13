//
// Created by ShangPingping on 2018/6/21.
//

#include <jni.h>
#include <malloc.h>
#include <string>
#include "DummySink.h"
#include "SubRTSPClient.h"
#include "RTSPCommon.h"

int size = 0;
int len = 0;
unsigned char *temp_buffer = NULL;
unsigned char *temp_vps = NULL;
unsigned char *temp_vps_sps = NULL;
unsigned char *temp_vps_sps_pps = NULL;
unsigned char start_code[4] = {0x00, 0x00, 0x00, 0x01};

extern JNIEnv* _env;
extern jclass _clazz;
extern jobject _obj;
extern jmethodID _methodID1;
extern jmethodID _methodID2;

extern RTSPClient* _rtspClient;
extern unsigned char runningFlag;
extern char eventLoopWatchVariable;
extern Boolean isRunning;
extern std::string printInfo;

/**
 * Constructor.
 * @param env
 * @param subsession
 * @param streamId
 */
DummySink::DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
        : MediaSink(env), fSubsession(subsession) {
    fStreamId = strDup(streamId);
    fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
}

/**
 * Destructor.
 */
DummySink::~DummySink() {
    delete[] fReceiveBuffer;
    delete[] fStreamId;
}

/**
 * Create.
 * @param env
 * @param subsession
 * @param streamId
 * @return
 */
DummySink* DummySink::createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId) {
    return new DummySink(env, subsession, streamId);
}

/**
 * A static method to deal with the received frame.
 * @param clientData
 * @param frameSize
 * @param numTruncatedBytes
 * @param presentationTime
 * @param durationInMicroseconds
 */
void DummySink::afterGettingFrame(void* clientData,
                                  unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned durationInMicroseconds) {
    DummySink* sink = (DummySink*) clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

/**
 * Dealing with the received frame.
 * @param frameSize
 * @param numTruncatedBytes
 * @param presentationTime
 */
void DummySink::afterGettingFrame(unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned /*durationInMicroseconds*/) {
    if (strcmp(fSubsession.mediumName(), "video") == 0) {
        if (runningFlag == 1) {
            /*NALU_type:
            H265:
            int type = (code & 0x7e) >> 1;
            0x40 (0100 0000) VPS  type = 32
            0x42 (0100 0010) SPS  type = 33
            0x44 (0100 0100) PPS  type = 34
            0x26 (0010 0110) IDR  type = 19
            0x02 (0000 0010) P    type = 1
            H264:
            int type = code & 0x1f;
            0x67 (0110 0111) SPS  type = 7
            0x68 (0110 1000) PPS  type = 8
            0x65 (0110 0101) IDR  type = 5
            0x61 (0100 0001) I    type = 1
            0x41 (0100 0001) P    type = 1
            0x01 (0000 0001) B    type = 1
            0x06 (0000 0110) SEI  type = 6*/

            if (fReceiveBuffer[0] == 0x40) { // vps
                len = frameSize + 4;
                temp_vps = (unsigned char *) malloc(len);
                memcpy(temp_vps, start_code, 4);
                memcpy(temp_vps + 4, fReceiveBuffer, frameSize);
            }
            if (fReceiveBuffer[0] == 0x42) { // vps + sps
                int _len = len;
                len = len + frameSize + 4;

                temp_vps_sps = (unsigned char *) malloc(len);
                memcpy(temp_vps_sps, temp_vps, _len);
                memcpy(temp_vps_sps + _len, start_code, 4);
                memcpy(temp_vps_sps + _len + 4, fReceiveBuffer, frameSize);

                free(temp_vps);
                temp_vps = NULL;
                _len = 0;
            }
            if (fReceiveBuffer[0] == 0x44) { // vps + sps + pps
                int _len = len;
                len = len + frameSize + 4;

                temp_vps_sps_pps = (unsigned char *) malloc(len);
                memcpy(temp_vps_sps_pps, temp_vps_sps, _len);
                memcpy(temp_vps_sps_pps + _len, start_code, 4);
                memcpy(temp_vps_sps_pps + _len + 4, fReceiveBuffer, frameSize);

                free(temp_vps_sps);
                temp_vps_sps = NULL;
                _len = 0;
            }
            if (fReceiveBuffer[0] == 0x26) {
                runningFlag = 0;
                if (_methodID2 == NULL) {
                    if (_clazz == NULL) {
                        _clazz = _env->FindClass("com/example/cynthiaty/live555/rtsp/RTSPClient");
                        if (_clazz == NULL) {
                            printInfo = "can't find class: \"com/example/cynthiaty/live555/rtsp/RTSPClient\"";
                            RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
                        }
                    }
                    if (_obj == NULL && _clazz != NULL) {
                        _obj = _env->AllocObject(_clazz);
                    }

                    _methodID2 = _env->GetMethodID(_clazz, "OnNativeCallBack", "([BI)V");
                    if (_methodID2 == NULL) {
                        printInfo = "can't find method: public void OnNativeCallBack(byte[], int)";
                        RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));
                    } else {
                        if (len == 0) {
                            jbyteArray jbArray = _env->NewByteArray(sizeof(start_code));
                            _env->SetByteArrayRegion(jbArray, 0, sizeof(start_code), (jbyte*) start_code);
                            _env->CallVoidMethod(_obj, _methodID2, jbArray, sizeof(start_code));
                            _env->DeleteLocalRef(jbArray);
                        } else {
                            jbyteArray jbArray = _env->NewByteArray(len);
                            _env->SetByteArrayRegion(jbArray, 0, len, (jbyte*) temp_vps_sps_pps);
                            _env->CallVoidMethod(_obj, _methodID2, jbArray, len);
                            _env->DeleteLocalRef(jbArray);

                            len = 0;
                            free(temp_vps_sps_pps);
                            temp_vps_sps_pps = NULL;
                        }
                    }
                }
                size = frameSize + 4;
                temp_buffer = (unsigned char *) malloc(size);
                memcpy(temp_buffer, start_code, 4);
                memcpy(temp_buffer + 4, fReceiveBuffer, frameSize);

                if (runningFlag == 0 && _methodID2 != NULL) {
                    jbyteArray jbArray = _env->NewByteArray(size);
                    _env->SetByteArrayRegion(jbArray, 0, size, (jbyte*) temp_buffer);

                    _env->CallVoidMethod(_obj, _methodID2, jbArray, size);
                    _env->DeleteLocalRef(jbArray);
                }
            }
        } else {
            if (fReceiveBuffer[0] != 0x40 && fReceiveBuffer[0] != 0x42
                && fReceiveBuffer[0] != 0x44 && fReceiveBuffer[0] != 0x26) {
                size = frameSize + 4;
                temp_buffer = (unsigned char *) malloc(size);
                memcpy(temp_buffer, start_code, 4);
                memcpy(temp_buffer + 4, fReceiveBuffer, frameSize);

                if (runningFlag == 0 && _methodID2 != NULL) {
                    jbyteArray jbArray = _env->NewByteArray(size);
                    _env->SetByteArrayRegion(jbArray, 0, size, (jbyte*) temp_buffer);

                    _env->CallVoidMethod(_obj, _methodID2, jbArray, size);
                    _env->DeleteLocalRef(jbArray);
                }
            }
        }

        free(temp_buffer);
        temp_buffer = NULL;
        size = 0;
    }

    // Then continue, to request the next frame of data.
    if (!isRunning) {
        shutdownStream(_rtspClient);
    } else {
        continuePlaying();
    }
}

/**
 * Continue playing.
 * @return
 */
Boolean DummySink::continuePlaying() {
    if (fSource == NULL) {
        return false;
    }

    // Request the next frame of data from our input source.
    fSource->getNextFrame(fReceiveBuffer,
                          DUMMY_SINK_RECEIVE_BUFFER_SIZE,
                          afterGettingFrame,
                          this,
                          onSourceClosure,
                          this);
    return true;
}

/**
 * Closing the stream.
 * @param rtspClient
 * @param exitCode
 */
void DummySink::shutdownStream(RTSPClient* rtspClient, int exitCode) {
    UsageEnvironment& env = rtspClient->envir(); // alias
    StreamClientState& scs = ((SubRTSPClient*) rtspClient)->scs;

    // First, check whether any subsessions have still to be closed.
    if (scs.session != NULL) {
        Boolean someSubsessionsWereActive = false;
        MediaSubsessionIterator iter(*scs.session);
        MediaSubsession* subsession;

        while ((subsession = iter.next()) != NULL) {
            if (subsession->sink != NULL) {
                Medium::close(subsession->sink);
                subsession->sink = NULL;

                if (subsession->rtcpInstance() != NULL) {
                    subsession->rtcpInstance()->setByeHandler(NULL, NULL);
                }

                someSubsessionsWereActive = true;
            }
        }

        if (someSubsessionsWereActive) {
            rtspClient->sendTeardownCommand(*scs.session, NULL);
        }
    }

    printInfo = "Closing the stream.";
    RTSPCommon::infoCallBack(RTSPCommon::charToJstring(_env, printInfo.c_str()));

    Medium::close(rtspClient);
    eventLoopWatchVariable = 1;
    isRunning = false;

    if (_clazz != NULL && _obj != NULL) {
        _env->DeleteLocalRef(_clazz);
        _env->DeleteLocalRef(_obj);
    }

    _methodID1 = NULL;
    _methodID2 = NULL;

    _clazz = NULL;
    _obj = NULL;
}
