package com.example.cynthiaty.live555.rtsp;

import com.example.cynthiaty.live555.Batch;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

/**
 * Created by ShangPingping on 2018/8/13.
 */

public class RecvRtspStream {
    private final int QUEUE_SIZE = 1000;

    private HandlingStreamTask handlingStreamTask;
    private BlockingQueue<byte[]> h26xDataQueue;

    private boolean isRunning = false;
    private boolean isDispensing = false;

    /**
     * Constructor--init params and vars.
     */
    public RecvRtspStream() {
        this.handlingStreamTask = new HandlingStreamTask();
        this.h26xDataQueue = new ArrayBlockingQueue<>(QUEUE_SIZE);
    }

    /**
     * Initialize the receive data thread and decode the output thread.
     */
    public void dtStartSelf() {
        isRunning = true;
        isDispensing = true;
    }

    /**
     * Close resource.
     */
    public void dtStopSelf() {
        isRunning = false;
        isDispensing = false;
        h26xDataQueue.clear();
        handlingStreamTask = null;
    }

    /**
     * Distribute data locally.
     * @param dataFrame
     */
    public void dtDispenseToLocal(byte[] dataFrame) {
        if (!isDispensing) return;

        try {
            h26xDataQueue.offer(dataFrame);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     *  Receiving the video data sent by the server in real time, and decoding.
     */
    private class HandlingStreamTask implements Runnable {
        private byte[] recvBuffer;

        public HandlingStreamTask() {
            this.recvBuffer = null;
        }

        @Override
        public void run() {
            while (isRunning) {
                // Receiving the video data sent by the server in real time.
                recvBuffer = h26xDataQueue.poll();
                if (recvBuffer == null) {
                    try {
                        Thread.sleep(10);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    continue;
                }

                Batch.saveFileToMemory("live555", recvBuffer); // Save the live data to file.
            }

            // Empty the cache.
            h26xDataQueue.clear();
            recvBuffer = null;
        }
    }
}
