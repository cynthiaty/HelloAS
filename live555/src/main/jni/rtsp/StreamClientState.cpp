//
// Created by ShangPingping on 2018/6/21.
//

#include "StreamClientState.h"

/**
 * Constructor.
 */
StreamClientState::StreamClientState()
        : iter(NULL), session(NULL), subsession(NULL), streamTimerTask(NULL), duration(0.0) {
}

/**
 * Destructor.
 */
StreamClientState::~StreamClientState() {
    delete iter;
    if (session != NULL) {
        UsageEnvironment& env = session->envir();
        env.taskScheduler().unscheduleDelayedTask(streamTimerTask);
        Medium::close(session);
    }
}
