package com.example.vino;


public class VinoGlobal {

}


class stdCamPos
{
	public final static int SIZE = 36;
	
	public Vector3 _eye = null;
	public Vector3 _center = null;
	public Vector3 _up = null;
}

class ViewFrustum
{
	public final static int SIZE = 24;
	
	public float _left = -1.0f;
	public float _right =  1.0f;
	public float _bottom = -1.0f;
	public float _top = 1.0f;
	public float _near = 1.0f;
	public float _far = 10000.0f;
}

class ViewPerspective
{
	public final static int SIZE = 16;
	public float _fov = 45.0f;
	public float _aspect = 1.0f;
	public float _near = 1.0f;
	public float _far = 10000.0f;
}

class ViewResolution
{
	public final static int SIZE = 8;
	public int _width = 1200;//
	public int _height = 1600;//
}

class UpsampleFactor
{
	public final static int SIZE = 8;
	public float _factX = 1.0f;
	public float _factY = 1.0f;
}


class MotionData
{
	public byte _type = 0;
	public float _x1 = 0.0f;
	public float _y1 = 0.0f;
	public float _x2 = 0.0f;
	public float _y2 = 0.0f;
}
