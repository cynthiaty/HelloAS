package com.example.cynthiaty.helloworld;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;

public class MainActivity extends AppCompatActivity {
//    private TextView tvHello;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @BindView(R.id.tv_hello)
    TextView tvHello;

    private native String stringFromJNI();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);

        initVar();
    }

    private void initVar() {
//        tvHello = (TextView) findViewById(R.id.tv_hello);
        tvHello.setText(stringFromJNI());
    }

    @Override
    protected void onStop() {
        super.onStop();

//        // Restart.
//        Intent launch = getBaseContext().getPackageManager()
//                .getLaunchIntentForPackage(getBaseContext().getPackageName());
//        launch.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
//        startActivity(launch);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
