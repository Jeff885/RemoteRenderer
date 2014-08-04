package com.example.vino;

import android.util.Log;

public class LocalInfoCapturer {
	
	
	private static final float MaxRotateRate = 25.0f;
	private static final int RefFramerate = 5;
	
	private ViewResolution _dspyRes;
	private UpsampleFactor  _upFact;
	private ViewPerspective _persp;
	private ViewResolution _recCRes;
	private ViewResolution _recDRes;

	private boolean _isCalcPrediction;
	
	private boolean _isPerspective;
	
	LocalInfoCapturer()
	{
		_dspyRes = new ViewResolution();
		_upFact = new UpsampleFactor();
		_persp = new ViewPerspective();
		_recCRes = new ViewResolution();
		_recDRes = new ViewResolution();
		
		_isCalcPrediction = false;
		_isPerspective = true;
	}
	
	public void setDispalyRes(int width, int height)
	{
		_dspyRes._width = width;
		_dspyRes._height = height;
		
		_isCalcPrediction = false;
	}
	
	public void setPerspective(float fov, float aspect, float near, float far)
	{
		_persp._fov = fov;
		_persp._aspect = aspect;
		_persp._near = near;
		_persp._far = far;
		
		_isCalcPrediction = false;
	}
	
	public void setUpsFactor(float factX, float factY)
	{
		_upFact._factX = factX;
		_upFact._factY = factY;
		
		_isCalcPrediction = false;
	}
	
	private void calcPredictionConfig()
	{
		if(_isCalcPrediction) return;
		
		if(_isPerspective)
		{
			float pFov = _persp._fov + 2 * MaxRotateRate / RefFramerate;
			_recDRes._height = (int)((Math.tan(pFov / 360.0 * Math.PI) 
					/ Math.tan(_persp._fov / 360.0 * Math.PI)) * _dspyRes._height);
			_recDRes._width = (int)(_persp._aspect * _recDRes._height);
		}
		
		_recCRes._width = (int)(_recDRes._width / _upFact._factX);
		_recCRes._height = (int)(_recDRes._height / _upFact._factY);
		
		_isCalcPrediction = true;
		
		Log.i("VINO_LOCAL_INFO_CAPTURER", "predicted Res: " + String.valueOf(_recDRes._width ) + ":"+ String.valueOf(_recDRes._height ));
	}
	public stdCamPos   getCameraPos()
	{
		stdCamPos pos = new stdCamPos();

       // 宸撮粠鍩庡競
		//pos._eye = new Vector3(-223.832f,-814.06f,11.2053f);
		//pos._center = new Vector3(-223.733f, -813.074f, 11.0723f);
		//pos._up = new Vector3(0.0132886f, 0.132357f, 0.99113f);


		/*pos._eye = new Vector3(-29584.476219f, -7017.597593f, 16677.337705f);
		pos._center = new Vector3(10109.515625f, 1296.968750f, 1770.148438f);
		pos._up = new Vector3(0.308999f, 0.194102f, 0.931045f);*/
		
		 // 巴黎城市
		
		  pos._eye = new Vector3(-223.832f,-814.06f,11.2053f);
		  pos._center = new Vector3(-223.832f, -813.074f, 11.2053f);
		  pos._up=new Vector3(0.0f,0.0f,1.0f);

       /*
       椋炴満
		air

       eye: -9.34912 -87.8533 11.4387
   ``` center: -9.24417 -86.8671 11.3103
`   ```up: 0.0404265 0.124776 0.991361
        */
     // pos._eye = new Vector3(-9.34912f,-87.8533f, 11.4387f);
     //  pos._center = new Vector3(-9.24417f, -86.8671f, 11.3103f);
   //   pos._up = new Vector3(0, 0, 1);

        /*
        arm
        eye: 58.5793 13.945 -506.653
        center: 58.4645 13.9597 -505.66
        up: -0.0197017 0.999661 -0.0170254
         */
     //pos._eye = new Vector3( 31.42787f, 2.121211f ,-300.818994f);
     //pos._center = new Vector3( 0.000008f, 21.419441f ,-0.000252f);
    // pos._up = new Vector3(0.046085f,0.997184f,-0.059157f);
		
		
	 //pos._eye = new Vector3( 58.5793f,13.945f, -506.653f);
	// pos._center = new Vector3( 58.4645f,13.9597f,-505.66f);
	// pos._up = new Vector3(-0.0197017f, 0.999661f, -0.0170254f);
		
      //pos._eye = new Vector3( 58.5793f, 13.945f, -506.653f);
     //pos._center = new Vector3(0f, 0f, 0f);
     //pos._up = new Vector3(0f, 1f, 0f);
		
		
		//pos._eye = new Vector3(31.427871f, 2.121211f, -300.818994f);
		//pos._center = new Vector3(0.000008f, 21.419441f, -0.000252f);
		//pos._up = new Vector3(0.046085f, 0.997184f, -0.059157f);
		//视点的中心不能设置的太小！！！！会出现花屏
		//pos._eye = new Vector3(58.5793f,13.945f, -350.653f);//8.713f , -280.11f
	    // pos._center = new Vector3(10.7122f, 21.75803f, -20.118f);
	   //pos._up = new Vector3(0.0204012f, 0.998862f, -0.042993f);
		
		//发动机
		// pos._center=new Vector3(12128.1f,  -7189.28f , -1211.33f);
		// pos._up=new Vector3(0f,0f,1f);
		// pos._eye=new Vector3(37784.2f , -17011.1f  ,3181.53f);
	
		  
	   return pos;
	}
	
	public ViewPerspective getPerpective()
	{
		ViewPerspective per = new ViewPerspective();
		per._fov = _persp._fov;
		per._aspect = _persp._aspect;
		per._near = _persp._near;
		per._far = _persp._far;
	
		return per;
	}
	public ViewResolution getResolution()
	{
		ViewResolution res = new ViewResolution();
		res._width = _dspyRes._width;
		res._height = _dspyRes._height;
		return res;
	}
	public UpsampleFactor getUpsample()
	{
		UpsampleFactor ups = new UpsampleFactor();
		ups._factX = _upFact._factX;
		ups._factY = _upFact._factY;
		return ups;
	}
	
	public ViewResolution getRecvColorRes()
	{
		if(!_isCalcPrediction) calcPredictionConfig();
		
		ViewResolution res = new ViewResolution();
		res._width = _recCRes._width;
		res._height = _recCRes._height;
		return res;
	}
	public ViewResolution getRecvDepthRes()
	{
		if(!_isCalcPrediction) calcPredictionConfig();
		
		ViewResolution res = new ViewResolution();
		res._width = _recDRes._width;
		res._height = _recDRes._height;
		return res;
	}
}
