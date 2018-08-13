package com.example.cynthiaty.live555.rtsp;

/**
 * Created by ShangPingping on 2018/8/13.
 */

public class RtspClient {
    private static RtspVideoListener rtspVideoListener;
    private static RtspInfoListener rtspInfoListener;

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public static native int start(String path);
    public static native void stop();

    /**
     * Native layer log output.
     * @param info
     */
    public void onNativeInfo(String info) {
        if (info.isEmpty()) return;

        if(rtspInfoListener != null) {
            rtspInfoListener.infoCallBack(info);
        }
    }

    /**
     * Native layer callback.
     * @param data
     * @param len
     */
    public void onNativeCallBack(byte[] data, int len) {
        if (rtspVideoListener != null) {
            rtspVideoListener.videoCallBack(data, len);
        }
    }

    /**
     * Set listener to receive rtsp real-time streaming data.
     * @param listener
     */
    public static void setRtspVideoListener(RtspVideoListener listener) {
        rtspVideoListener = listener;
    }

    /**
     * Set listener to show the info at java lever.
     * @param listener
     */
    public static void setRtspInfoListener(RtspInfoListener listener) {
        rtspInfoListener = listener;
    }
}
