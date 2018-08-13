package com.example.cynthiaty.hellotcpsocket;

import android.content.Intent;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * 作者：尚萍萍
 * 功能：TcpClient
 * 时间：2018-3-21
 */

public class TcpClient implements Runnable {

    private String TAG = "TcpClient";
    private String  serverIP = "192.168.1.252";
    private int serverPort = 1935;
    private Socket socket = null;
    private PrintWriter pw = null;
    private DataInputStream dis = null;
    private boolean isRun = true;

    public TcpClient(String ip, int port) {
        this.serverIP = ip;
        this.serverPort = port;
    }

    @Override
    public void run() {
        byte[] buff = new byte[4096];

        try {
            //主动向服务端发送连接请求
            socket = new Socket(serverIP, serverPort);
            socket.setSoTimeout(5000);
            //向服务端发送数据，true->代表即时刷新
            pw = new PrintWriter(socket.getOutputStream(),true);

            //读取服务端回送的数据
            dis = new DataInputStream(socket.getInputStream());
            while(isRun) {
                try {
                    int rcvLen = dis.read(buff);
                    String rcvMsg = new String(buff, 0, rcvLen,"utf-8");
                    Intent intent = new Intent();
                    intent.setAction("tcpClientReceiver");
                    intent.putExtra("tcpClientReceiver", rcvMsg);
                    MainActivity.context.sendBroadcast(intent); //将消息发送给主界面
                    if (rcvMsg.equals("QuitClient")) {  //服务器要求客户端结束
                        isRun = false;
                    }
                } catch(IOException e) {
                    e.printStackTrace();
                }

            }
        } catch(IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if(pw != null) pw.close();
                if(dis != null) dis.close();
                if(socket != null) socket.close();
            } catch(IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void send(String msg) {
        pw.println(msg);
        pw.flush();
    }

    public void closeSelf() {
        isRun = false;
    }
}

