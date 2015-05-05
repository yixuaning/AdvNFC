package com.advantech.advnfc;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class AdvNFC extends Activity {

	private Button initBtn;
	private Button scanBtn;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_advnfc);
		initBtn=(Button) findViewById(R.id.initbtn);
		initBtn.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                	doInit();
                	Log.e("yixuan", "yixuan");
                }
        });

		scanBtn=(Button) findViewById(R.id.scanbtn);
		scanBtn.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                	
                }
        });

	}
	
	// JNI
	private native void doInit();
	private native String scan();
	public native void close();
	static {
		System.loadLibrary("advnfc");
	}
}
