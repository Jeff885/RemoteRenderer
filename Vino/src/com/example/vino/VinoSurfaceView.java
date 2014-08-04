package com.example.vino;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import android.opengl.GLSurfaceView;
import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;

public class VinoSurfaceView extends GLSurfaceView {
	private static final String TAG = "VINO_SURFACE_VIEW";
	private static final int ShaderCnt = 12;
	private static boolean DEBUG = false;
	private static final int NOID = -24;
	
	private MotionDataBuffer _mdBuffer;
	private TransferAdapter _adapter;
	private VinoRender _render;
	
	private Thread _recvThd;
	private Thread _sendThd;
	
	private String[] _shdSrcs;
	private int[] _pointID;
	
	public VinoSurfaceView(Context context)
	{
		super(context);
	}
	
	public VinoSurfaceView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}
	
	public VinoSurfaceView(Context context, AttributeSet attrs, TransferAdapter adapter)
	{
		super(context, attrs);
		_mdBuffer = new MotionDataBuffer();
		_adapter = adapter;
		_pointID = new int[2];
		_pointID[0] = _pointID[1] = NOID;
		init();
		setupThreads();
		getShaderSources(context);
		
	}
	
	public VinoSurfaceView(Context context,TransferAdapter adapter)
	{
		super(context);
		_mdBuffer = new MotionDataBuffer();
		_adapter = adapter;
		_pointID = new int[2];
		_pointID[0] = _pointID[1] = NOID;
		init();
		setupThreads();
		getShaderSources(context);
	}
	
	public void setupThreads()
	{
		_sendThd = new Thread(new SendThread(_adapter, _mdBuffer));
		_recvThd = new Thread(
				new ReceiveThread(_adapter, this));
	}
	
	public void surfaceCreated(SurfaceHolder holder) 
    {
    	super.surfaceCreated(holder);
    	
    	VinoNativeRender.onCreate(
    			_shdSrcs
    			);
    	
    	_sendThd.start();
    	_recvThd.start();
    }
	public void surfaceDestroyed(SurfaceHolder holder)
	{
		super.surfaceDestroyed(holder);
		VinoNativeRender.onDestory();
	}
	public VinoRender getRender()
	{
		return _render;
	}
	
	protected void init()
	{
		this.getHolder().setFormat(PixelFormat.RGBA_8888);
		
		setEGLContextFactory(new VinoContextFactory());
		
		setEGLConfigChooser(new VinoConfigChooser());
		
		_render = new VinoRender();
		setRenderer(_render);
		
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}
	
	private void getShaderSources(Context context)
	{
		_shdSrcs = new String[ShaderCnt];
		for(int i = 0; i < ShaderCnt; ++i)
		{
			_shdSrcs[i] = new String();
			String fName = new String("shader" + i/2);
			if((i & 1) == 0) fName += ".vert";
			else fName +=".frag";
			 try
			 {
				BufferedReader src = new BufferedReader(
						new InputStreamReader(context.getAssets().open(fName))
						);
				String tmp;
				while((tmp = src.readLine()) != null)
				{
					_shdSrcs[i] += tmp + "\n";
				}
				//Log.i("VINO", _shdSrcs[i]);
			 }
			 catch (IOException e) {
		        e.printStackTrace();
		     }
		}
	}
	
	@Override
	public boolean onTouchEvent(final MotionEvent event)
	{
		if(event.getPointerCount() > 3) return true;
		MotionData tmp = new MotionData();
		switch(event.getAction() & MotionEvent.ACTION_MASK)
		{
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
			{
				tmp._type = (byte)((tmp._type | (1 << 6)) & 0xff);
				int pID = event.getPointerId(event.getActionIndex());
				if(_pointID[0] == NOID)
					_pointID[0] = pID;
				else if(_pointID[1] == NOID)
					_pointID[1] = pID;
				break;
			}
			case MotionEvent.ACTION_MOVE:
			{
				tmp._type = (byte)((tmp._type | (3 << 6)) & 0xff);
				break;
			}
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			{
				tmp._type = (byte)((tmp._type | (2 << 6)) & 0xff);
				int tIdx = event.getActionIndex();
				int i;
				_pointID[0] = _pointID[1] = NOID;
				for(i = 0; i < event.getPointerCount(); ++i)
					if(i != tIdx)
					{
						int pID = event.getPointerId(i);
						if(_pointID[0] == NOID)
							_pointID[0] = pID;
						else if(_pointID[1] == NOID)
							_pointID[1] = pID;
					}
				break;
			}
		}
		if(_pointID[0] != NOID)
		{
			int idx = event.findPointerIndex(_pointID[0]);
			tmp._type = (byte)((tmp._type | 1) & 0xff);
			tmp._x1 = event.getX(idx);
			tmp._y1 = event.getY(idx);
		}
		if(_pointID[1] != NOID)
		{
			int idx = event.findPointerIndex(_pointID[1]);
			tmp._type = (byte)((tmp._type | 2) & 0xff);
			tmp._x2 = event.getX(idx);
			tmp._y2 = event.getY(idx);
		}
		_mdBuffer.putMotion(tmp);
		queueEvent(new MotionRunnable(tmp));
		requestRender();
		return true;
	}
	
	private class MotionRunnable implements Runnable
	{
		private MotionData _motion;
		
		public MotionRunnable(MotionData md)
		{
			_motion = md;
		}
		public void run()
		{
			//_render.setNewMotion(_motion);
			VinoNativeRender.onUpdate(_motion);
		}
	}
	
	private static class VinoContextFactory implements GLSurfaceView.EGLContextFactory 
	{
		//This enumeration is not supported by EGL10, but by a high level EGL14
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) 
        {
            Log.w(TAG, "creating OpenGL ES 2.0 context");
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            checkEglError("After eglCreateContext", egl);
            Log.w(TAG,"***********************************************************");
            return context;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) 
        {
            egl.eglDestroyContext(display, context);
        }
        
        private static void checkEglError(String prompt, EGL10 egl) 
        {
            int error;
            while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
                Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
            }
        }
    }
	
	private static class VinoConfigChooser implements GLSurfaceView.EGLConfigChooser
	{
		private int _redSize;
		private int _greenSize;
		private int _blueSize;
		private int _alphaSize;
		private int _depthSize;
		
		private int[] mValue = new int[1];
		
		public VinoConfigChooser()
		{
			_redSize = 8;
			_greenSize = 8;
			_blueSize = 8;
			_alphaSize = 8;
			_depthSize = 24;
		}
		
		//the same as EGL_CONTEXT_CLIENT_VERSION;
		private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
            EGL10.EGL_RED_SIZE, 5,
            EGL10.EGL_GREEN_SIZE, 6,
            EGL10.EGL_BLUE_SIZE, 5,
            EGL10.EGL_DEPTH_SIZE, 8,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL10.EGL_NONE
        };
        
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
        {
        	int[] num_config = new int[1];
        	egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);
        	
        	int numConfigs = num_config[0];
        	
        	if (numConfigs <= 0) {
                throw new IllegalArgumentException("No configs match configSpec");
            }
        	
        	EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);
            
            if (DEBUG) {
                printConfigs(egl, display, configs);
           }
            
           return chooseConfig(egl, display, configs);
        }
        
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs) {
            for(EGLConfig config : configs) {
                
                int r = findConfigAttrib(egl, display, config,
                        EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config,
                            EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config,
                            EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config,
                        EGL10.EGL_ALPHA_SIZE, 0);
                int d = findConfigAttrib(egl, display, config,
                        EGL10.EGL_DEPTH_SIZE, 0);
                
                if (r == _redSize && g == _greenSize && b == _blueSize && a == _alphaSize && d >= _depthSize)
                    return config;
            }
            return null;
        }
        
        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) {

            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }
        private void printConfigs(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs) {
                int numConfigs = configs.length;
                Log.w(TAG, String.format("%d configurations", numConfigs));
                for (int i = 0; i < numConfigs; i++) {
                    Log.w(TAG, String.format("Configuration %d:\n", i));
                    printConfig(egl, display, configs[i]);
                }
            }

            private void printConfig(EGL10 egl, EGLDisplay display,
                    EGLConfig config) {
                int[] attributes = {
                        EGL10.EGL_BUFFER_SIZE,
                        EGL10.EGL_ALPHA_SIZE,
                        EGL10.EGL_BLUE_SIZE,
                        EGL10.EGL_GREEN_SIZE,
                        EGL10.EGL_RED_SIZE,
                        EGL10.EGL_DEPTH_SIZE,
                        EGL10.EGL_STENCIL_SIZE,
                        EGL10.EGL_CONFIG_CAVEAT,
                        EGL10.EGL_CONFIG_ID,
                        EGL10.EGL_LEVEL,
                        EGL10.EGL_MAX_PBUFFER_HEIGHT,
                        EGL10.EGL_MAX_PBUFFER_PIXELS,
                        EGL10.EGL_MAX_PBUFFER_WIDTH,
                        EGL10.EGL_NATIVE_RENDERABLE,
                        EGL10.EGL_NATIVE_VISUAL_ID,
                        EGL10.EGL_NATIVE_VISUAL_TYPE,
                        0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                        EGL10.EGL_SAMPLES,
                        EGL10.EGL_SAMPLE_BUFFERS,
                        EGL10.EGL_SURFACE_TYPE,
                        EGL10.EGL_TRANSPARENT_TYPE,
                        EGL10.EGL_TRANSPARENT_RED_VALUE,
                        EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                        EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                        0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                        0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                        0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                        0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                        EGL10.EGL_LUMINANCE_SIZE,
                        EGL10.EGL_ALPHA_MASK_SIZE,
                        EGL10.EGL_COLOR_BUFFER_TYPE,
                        EGL10.EGL_RENDERABLE_TYPE,
                        0x3042 // EGL10.EGL_CONFORMANT
                };
                String[] names = {
                        "EGL_BUFFER_SIZE",
                        "EGL_ALPHA_SIZE",
                        "EGL_BLUE_SIZE",
                        "EGL_GREEN_SIZE",
                        "EGL_RED_SIZE",
                        "EGL_DEPTH_SIZE",
                        "EGL_STENCIL_SIZE",
                        "EGL_CONFIG_CAVEAT",
                        "EGL_CONFIG_ID",
                        "EGL_LEVEL",
                        "EGL_MAX_PBUFFER_HEIGHT",
                        "EGL_MAX_PBUFFER_PIXELS",
                        "EGL_MAX_PBUFFER_WIDTH",
                        "EGL_NATIVE_RENDERABLE",
                        "EGL_NATIVE_VISUAL_ID",
                        "EGL_NATIVE_VISUAL_TYPE",
                        "EGL_PRESERVED_RESOURCES",
                        "EGL_SAMPLES",
                        "EGL_SAMPLE_BUFFERS",
                        "EGL_SURFACE_TYPE",
                        "EGL_TRANSPARENT_TYPE",
                        "EGL_TRANSPARENT_RED_VALUE",
                        "EGL_TRANSPARENT_GREEN_VALUE",
                        "EGL_TRANSPARENT_BLUE_VALUE",
                        "EGL_BIND_TO_TEXTURE_RGB",
                        "EGL_BIND_TO_TEXTURE_RGBA",
                        "EGL_MIN_SWAP_INTERVAL",
                        "EGL_MAX_SWAP_INTERVAL",
                        "EGL_LUMINANCE_SIZE",
                        "EGL_ALPHA_MASK_SIZE",
                        "EGL_COLOR_BUFFER_TYPE",
                        "EGL_RENDERABLE_TYPE",
                        "EGL_CONFORMANT"
                };
                int[] value = new int[1];
                for (int i = 0; i < attributes.length; i++) {
                    int attribute = attributes[i];
                    String name = names[i];
                    if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
                        Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
                    } else {
                        // Log.w(TAG, String.format("  %s: failed\n", name));
                        while (egl.eglGetError() != EGL10.EGL_SUCCESS);
                    }
                }
            }
	}
}
