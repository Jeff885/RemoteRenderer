package com.example.vino;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;



public class VinoRender implements Renderer {
	
	
	private MotionData _motion;
	private boolean _isNewMotion;
	private boolean _isNewData;
	
	long preTime = System.currentTimeMillis();
	public VinoRender()
	{
		_isNewData = false;
		
		_motion = null;
		_isNewMotion = false;
	}
	
	public  void setNewData()
	{
		_isNewData = true;
	}
	public void setNewMotion(MotionData md)
	{
		_motion = md;
		_isNewMotion = true;
	}
	@Override
	public void onDrawFrame(GL10 gl) {
		// TODO Auto-generated method stub
		//long tmp = System.currentTimeMillis();
		
		//Log.i("VINO", "preFrame time:" + (tmp - preTime));
		
		//preTime = tmp;
		
		/*
		if(_isNewMotion)
		{
			VinoNativeRender.onUpdate(_motion);
			_isNewMotion = false;
		}
		*/
		
		if(_isNewData) 
		{
			VinoNativeRender.onProcess();
			_isNewData = false;
			
		}
		VinoNativeRender.onFrame();
		//long endTime = System.currentTimeMillis();
		//Log.i("VINO", "Total Frame time:" + (endTime - tmp));
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		// TODO Auto-generated method stub
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub
		VinoNativeRender.onInit();
	}

}


