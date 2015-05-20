package com.advantech.advnfc;

import android.os.Bundle;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class AdvNFC extends Activity {

	private Button loadKeyBtn;
	private Button readDataBtn;
	private Button connectBtn;
	
	private BluetoothAdapter mBltAdapter;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_advnfc);
		loadKeyBtn=(Button) findViewById(R.id.loadKeyBtn);
		loadKeyBtn.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                	//doInit();
                	Log.e("yixuan", "yixuan");
                }
        });

		readDataBtn=(Button) findViewById(R.id.readDataBtn);
		readDataBtn.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                	
                }
        });

		connectBtn=(Button) findViewById(R.id.connectBtn);
		connectBtn.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                	mBltAdapter = BluetoothAdapter.getDefaultAdapter();
                	if(!mBltAdapter.isEnabled()){
                		mBltAdapter.enable();
                	}
                	BluetoothDevice remoteBltDevice = mBltAdapter.getRemoteDevice(address);
                }
        });
	}
	
	// JNI
	private native boolean loadKey();
	private native String readCard(); 
	static {
		System.loadLibrary("advnfc");
	}
}
