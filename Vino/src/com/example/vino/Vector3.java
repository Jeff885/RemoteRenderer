package com.example.vino;

import java.lang.Math;

public class Vector3 {
	public float x, y ,z;
	public final float ZERO = 1e-10f;
	Vector3()
	{
		x = y = z = 0.0f;
	}
	Vector3(float x, float y ,float z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}
	Vector3(float[] a)
	{
		x = a[0];
		y = a[1];
		z = a[2];
	}
	
	public float length()
	{
		double dd = (double)x * x + (double)y * y + (double)z * z;
		return (float)(Math.sqrt(dd));
	}
	
	public void normalize()
	{
		if(isZero()) return;
		float l = length();
		x /= l;
		y /= l;
		z /= l;
	}
	
	public boolean isZero()
	{
		if(x <= ZERO && y <= ZERO && z <= ZERO) return true;
		return false;
	}
	public float dotProduct(Vector3 a)
	{
		return (a.x * x + a.y * y + a.z * z);
	}
	
	public Vector3 crossProduct(Vector3 a)
	{
		Vector3 tmp = new Vector3();
		tmp.x = y * a.z - z * a.y;
		tmp.y = z * a.x - x * a.z;
		tmp.z = x * a.y - y * a.x;
		return tmp;
	}
}