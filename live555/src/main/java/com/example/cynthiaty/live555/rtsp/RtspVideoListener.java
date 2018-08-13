package com.example.cynthiaty.live555.rtsp;

/**
 * Created by ShangPingping on 2018/8/13.
 */

public interface RtspVideoListener {
    void videoCallBack(byte[] data, int len);
}
