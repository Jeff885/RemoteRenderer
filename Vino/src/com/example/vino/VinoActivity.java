package com.example.vino;


import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.os.Handler;
import android.util.Log;

public class VinoActivity extends Activity {

	public static final String ADDRESS= "219.224.168.81";
	public static final int PORT = 5588;
	
	//public static final int _interactionmode=Interaction.USER_DEFINE;
	//public static final int _interactionmode=Interaction.TRACKBALLMANIPULATOR;
	public static final int _interactionmode=Interaction.FIRSTPERSONMANIPULATOR;
	
	//public static final String _sceneName="armadillo";   //"paris"  "engine"  "cessna"
	//public static final String _sceneName="engine";
	public static final String _sceneName="paris";
	
	//2014_7_28
	//public static final int ApplicationID=ApplicationType.DEFAULTVALUE;//默认值
	public static final int ApplicationID=ApplicationType.ENGINESHOW;//发动机三维结构展示应用
	public static final boolean isAppmode=true;//是否会发送APPLICATIONMS类型的消息包
	
	private TransferAdapter _adapter = null;
	private LocalInfoCapturer _capturer = null;
	private VinoSurfaceView  _glView = null;
	private Handler _handler = null;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		_adapter = new TransferAdapter();
		
		_capturer = new LocalInfoCapturer();
		//N4 768,1280
		//_capturer.setDispalyRes(1200, 1600);
        _capturer.setDispalyRes(600, 800);
        /*
		宸撮粠
		city
        */
        _capturer.setPerspective(29.1484f, 0.75f, 10, 20000);

		_capturer.setUpsFactor(1.0f, 1.0f);
		/*
		椋炴満
		air
		*/

		//_capturer.setPerspective(30f, 0.75f, 10, 200f);
       //_capturer.setUpsFactor(1.0f, 1.0f);

		/*
		arm
		 */

        //_capturer.setPerspective(30f, 0.75f, 10, 5000f);
       // _capturer.setUpsFactor(1.0f, 1.0f);
       // _capturer.setPerspective(45.0f, 0.75f, 50.0f, 500.0f);
      	//_capturer.setUpsFactor(1.0f, 1.0f);
      	
        //_capturer.setPerspective(29.1484f, 0.75f, 10.0f, 50000.0f);
      	//_capturer.setUpsFactor(1.0f, 1.0f);
       
       //发动机
  	 // _capturer.setPerspective(29.1484f, 0.75f, 20000f, 100000f);

  	 // _capturer.setUpsFactor(1.0f, 1.0f);

		_handler = new Handler();
		
		ViewResolution[] ress = new  ViewResolution[3];
    	ress[0] = _capturer.getResolution();
    	ress[1] = _capturer.getRecvColorRes();
    	ress[2] = _capturer.getRecvDepthRes();
    	ViewPerspective psp = _capturer.getPerpective();
    	stdCamPos pos = _capturer.getCameraPos();
    	
    	VinoNativeRender.setGlobe(ress, psp, pos,_interactionmode);
    	Log.i("VINO",".........................");
		Thread mConnect = new Thread(new ConnectThread(this));
		mConnect.start();
        Log.i("VINO","~~~~~~~~~~~~~~~~~~~~~~~~~~");
       // getSocket
		setContentView(R.layout.activity_vino);
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		if(_glView != null ) _glView.onResume();
	}
	protected  void onDestroy()
    {
		if(_adapter._isConnected)
        {
        	_adapter.sendOnePacket(MessageType.CONTROL_STOP_MSG, 8);
        }
        super.onDestroy();
        Log.i("Destroy:","Destroy the activity!!!!!!");
    }
	@Override
	protected void onPause()
	{
		super.onPause();
		if(_glView != null ) _glView.onPause();
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.vino, menu);
		return true;
	}
	
	public void setupVinoView()
	{	
		Log.i("VINO", "setVinoView");
		_glView = new VinoSurfaceView(this, _adapter);
		_glView.getRender().setNewData();
		setContentView(_glView);
	}
	
	public TransferAdapter getAdapter()
	{
		return _adapter;
	}
	
	public LocalInfoCapturer getCapturer()
	{
		return _capturer;
	}
	
	public Handler getHandler()
	{
		return _handler;
	}
}  
