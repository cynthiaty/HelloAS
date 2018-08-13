package com.example.cynthiaty.live555;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.cynthiaty.live555.rtsp.RecvRtspStream;
import com.example.cynthiaty.live555.rtsp.RtspClient;
import com.example.cynthiaty.live555.rtsp.RtspInfoListener;
import com.example.cynthiaty.live555.rtsp.RtspVideoListener;

public class MainActivity extends AppCompatActivity {
	private EditText etUrl;
	private Button btnStart, btnStop;
	private ScrollView slvInfo;
	private TextView tvInfo;
	private RecvRtspStream recvRtspStream;

	static {
		System.loadLibrary("rtsplive555");
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		initVar();
		binderListener();
		setRtspVideoListener();
		setRtspInfoListener();
	}


	/**
	 * Init the vars.
	 */
	private void initVar() {
		etUrl = (EditText) findViewById(R.id.et_url);
		btnStart = (Button) findViewById(R.id.btn_start);
		btnStop = (Button) findViewById(R.id.btn_stop);
		slvInfo = (ScrollView) findViewById(R.id.slv_info);
		tvInfo = (TextView) findViewById(R.id.tv_info);
	}

	/**
	 * Binding the listeners.
	 */
	private void binderListener() {
		btnStart.setOnClickListener(new MyClickListener());
		btnStop.setOnClickListener(new MyClickListener());
	}

	/**
	 * Set listener to receive rtsp real-time streaming data.
	 */
	private void setRtspVideoListener() {
		RtspClient.setRtspVideoListener(new RtspVideoListener() {
			@Override
			public void videoCallBack(byte[] data, int len) {
				if(recvRtspStream != null) {
					recvRtspStream.dtDispenseToLocal(data);
				}
			}
		});
	}

	/**
	 * Set listener to show the rtsp information.
	 */
	private void setRtspInfoListener() {
		RtspClient.setRtspInfoListener(new RtspInfoListener() {
			@Override
			public void infoCallBack(final String msg) {
				runOnUiThread(new Runnable() {
					public void run() {
						tvInfo.append(msg + "\n");
						slvInfo.fullScroll(ScrollView.FOCUS_DOWN);
					}
				});
			}
		});
	}

	/**
	 * Inner class -- MyClickListener.
	 */
	private class MyClickListener implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			switch (v.getId()) {
				case R.id.btn_start:
					tvInfo.setText("");

					recvRtspStream = new RecvRtspStream();
					recvRtspStream.dtStartSelf();
					Batch.exec.execute(new Runnable() {
						@Override
						public void run() {
							RtspClient.start(etUrl.getText().toString().trim());
						}
					});

					Toast.makeText(MainActivity.this, "Start", Toast.LENGTH_SHORT).show();
					break;
				case R.id.btn_stop:
					tvInfo.setText("");

					if(recvRtspStream != null) {
						recvRtspStream.dtStopSelf();
						recvRtspStream = null;
					}
					RtspClient.stop();

					Toast.makeText(MainActivity.this, "Stop", Toast.LENGTH_SHORT).show();
					break;
				default:
					break;
			}
		}
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		if(recvRtspStream != null) {
			recvRtspStream.dtStopSelf();
			recvRtspStream = null;
		}
		RtspClient.stop();
	}
}
