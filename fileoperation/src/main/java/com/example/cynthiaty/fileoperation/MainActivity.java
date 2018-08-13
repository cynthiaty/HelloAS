package com.example.cynthiaty.fileoperation;

import android.Manifest;
import android.annotation.TargetApi;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Map;

public class MainActivity extends AppCompatActivity {

    private final String TAG = "MainActivity";
    public static Context context;
    private EditText editServerIP, editServerPort;
    private Button btnSaveFile, btnReadFile;
    private TextView txtFile;
    private MyBtnClicker myBtnClicker = new MyBtnClicker();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        context = MainActivity.this;
        requestPermission();
        bindID();
        bindListener();
    }

    /**
     * Request permission.
     */
    private boolean hasPermission = false;
    @TargetApi(23)
    private void requestPermission() {
        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ||
                    checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,
                        Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
            } else {
                hasPermission = true;
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,  String[] permissions,  int[] grantResults) {
        // Avoid crash when user don't confirm the permission after pause/resume.
        if (requestCode == 1 && grantResults != null && grantResults.length > 0) {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                hasPermission = true;
            }
        }
    }

    private void bindID() {
        editServerIP = (EditText) findViewById(R.id.edit_serverIP);
        editServerPort = (EditText) findViewById(R.id.edit_serverPort);
        btnSaveFile = (Button) findViewById(R.id.btn_saveFile);
        btnReadFile = (Button) findViewById(R.id.btn_readFile);
        txtFile = (TextView) findViewById(R.id.txt_file);
    }
    private void bindListener() {
        btnSaveFile.setOnClickListener(myBtnClicker);
        btnReadFile.setOnClickListener(myBtnClicker);
    }

    private class MyBtnClicker implements View.OnClickListener {

        private final String fileName = "appConfig";
        @Override
        public void onClick(View view) {
            switch (view.getId()){
                case R.id.btn_saveFile:
                    saveFileToMemory();
                    break;
                case R.id.btn_readFile:
                    readFileAndShow();
                    break;
            }
        }

        /**
         * Read the file and show it.
         */
        private void readFileAndShow() {
            Map<String, String> mapConfig = Batch.readFileFromMemoryAndParse(fileName);

            if(mapConfig != null && !mapConfig.isEmpty()) {
                String configFile = "serverIP=" + mapConfig.get("serverIP") + "\r\n"
                        + "serverPort=" + Integer.parseInt(mapConfig.get("serverPort"));
                txtFile.setText(configFile);
            }
        }

        /**
         * Save the file to memory.
         */
        private void saveFileToMemory() {
            String serverIP = editServerIP.getText().toString();
            String serverPort = editServerPort.getText().toString();

            if(serverIP.isEmpty()) {
                Toast.makeText(context, "IP address cannot be empty!", Toast.LENGTH_SHORT).show();
                return;
            }
            if(serverPort.isEmpty()) {
                Toast.makeText(context, "The port number cannot be empty!", Toast.LENGTH_SHORT).show();
                return;
            }

            String fileContent = "serverIP=" + serverIP + "\r\n" + "serverPort=" + serverPort + "\r\n";
            Batch.saveFileToMemory(fileName, fileContent.getBytes());

            Toast.makeText(context, "写入文件成功！", Toast.LENGTH_SHORT).show();
        }
    }
}
