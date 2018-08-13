package com.example.cynthiaty.hellotcpsocket;

import android.annotation.SuppressLint;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.lang.ref.WeakReference;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 作者：尚萍萍
 * 功能：MainActivity
 * 时间：2018-3-21
 */

public class MainActivity extends AppCompatActivity {

    private final String TAG = "MainActivity";
    @SuppressLint("StaticFieldLeak")    //@SuppressLint("all")
    public static Context context;
    private Button btnStartClient, btnCloseClient, btnCleanClientSend, btnCleanClientRcv, btnClientSend;
    private TextView txtRcv, txtSend;
    private EditText editClientSend, editClientPort, editClientIp;
    private static TcpClient tcpClient = null;
    private MyBtnClicker myBtnClicker = new MyBtnClicker();
    private final MyHandler myHandler = new MyHandler(this);
    private MyBroadcastReceiver myBroadcastReceiver = new MyBroadcastReceiver();
    private ExecutorService exec = Executors.newCachedThreadPool();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        context = this;
//        bindID();
//        bindListener();
//        bindReceiver();
//        Init();
        byte[] pBuf = {0x24, 0x24, 0x02, 0x00, 0x0d, 0x02, 0x00, 0x00, 0x01, 0x01, 0x01};
        int result = crc16(pBuf);
        Log.i(TAG, "result = " + result);
        Log.i(TAG, "result = " + Integer.toHexString(result));
    }

    int crc16(byte[] bytes) {
        int crc = 0x0000;
        int poly = 0xa001;

        for (int i = 0; i < bytes.length; i++) {
            crc ^= bytes[i];
            for (int j = 0; j < 8; j++) {
                if((crc & 1) > 0) {
                    crc >>= 1;
                    crc ^= poly;
                }
                else crc >>= 1;
            }
        }

        return crc;
    }

    private void bindID() {
        btnStartClient = (Button) findViewById(R.id.btn_tcpClientConn);
        btnCloseClient = (Button) findViewById(R.id.btn_tcpClientClose);
        btnCleanClientRcv = (Button) findViewById(R.id.btn_tcpCleanClientRecv);
        btnCleanClientSend = (Button) findViewById(R.id.btn_tcpCleanClientSend);
        btnClientSend = (Button) findViewById(R.id.btn_tcpClientSend);
        editClientPort = (EditText) findViewById(R.id.edit_tcpClientPort);
        editClientIp = (EditText) findViewById(R.id.edit_tcpClientIp);
        editClientSend = (EditText) findViewById(R.id.edit_tcpClientSend);
        txtRcv = (TextView) findViewById(R.id.txt_ClientRcv);
        txtSend = (TextView) findViewById(R.id.txt_ClientSend);
    }
    private void bindListener() {
        btnStartClient.setOnClickListener(myBtnClicker);
        btnCloseClient.setOnClickListener(myBtnClicker);
        btnCleanClientRcv.setOnClickListener(myBtnClicker);
        btnCleanClientSend.setOnClickListener(myBtnClicker);
        btnClientSend.setOnClickListener(myBtnClicker);
    }
    private void bindReceiver() {
        IntentFilter intentFilter = new IntentFilter("tcpClientReceiver");
        registerReceiver(myBroadcastReceiver,intentFilter);
    }
    private void Init() {
        btnCloseClient.setEnabled(false);
        btnClientSend.setEnabled(false);
    }

    private class MyBtnClicker implements OnClickListener {

        @Override
        public void onClick(View view) {
            switch (view.getId()){
                case R.id.btn_tcpClientConn:
                    Log.i(TAG, "onClick: 开始");
                    btnStartClient.setEnabled(false);
                    btnCloseClient.setEnabled(true);
                    btnClientSend.setEnabled(true);
                    tcpClient = new TcpClient(editClientIp.getText().toString(),
                            Integer.parseInt(editClientPort.getText().toString()));
                    exec.execute(tcpClient);
                    break;
                case R.id.btn_tcpClientClose:
                    tcpClient.closeSelf();
                    btnStartClient.setEnabled(true);
                    btnCloseClient.setEnabled(false);
                    btnClientSend.setEnabled(false);
                    break;
                case R.id.btn_tcpCleanClientRecv:
                    txtRcv.setText("");
                    break;
                case R.id.btn_tcpCleanClientSend:
                    txtSend.setText("");
                    break;
                case R.id.btn_tcpClientSend:
                    Message message = Message.obtain();
                    message.what = 2;
                    message.obj = editClientSend.getText().toString();
                    myHandler.sendMessage(message);
                    exec.execute(new Runnable() {
                        @Override
                        public void run() {
                            tcpClient.send(editClientSend.getText().toString());
                        }
                    });
                    break;
            }
        }
    }

    private class MyHandler extends Handler {

        private WeakReference<MainActivity> mActivity;
        MyHandler(MainActivity activity){
            mActivity = new WeakReference<MainActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            if (mActivity != null){
                editClientSend.setText("");
                switch (msg.what){
                    case 1:
                        txtRcv.append(msg.obj.toString());
                        break;
                    case 2:
                        txtSend.append(msg.obj.toString() + "\n");
                        break;
                }
            }
        }
    }

    private class MyBroadcastReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            String mAction = intent.getAction();
            switch (mAction){
                case "tcpClientReceiver":
                    String msg = intent.getStringExtra("tcpClientReceiver");
                    Message message = Message.obtain();
                    message.what = 1;
                    message.obj = msg;
                    myHandler.sendMessage(message);
                    break;
            }
        }
    }
}

