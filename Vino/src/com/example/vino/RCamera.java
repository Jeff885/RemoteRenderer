package com.example.vino;

import android.util.Log;

//2014_8_3  writen by Gmj
//User: Rmm 

public class RCamera {
	private float position_x;
	private float position_y;
	private float position_z;
	
	private float center_x;
	private float center_y;
	private float center_z;
	
	private float up_x;
	private float up_y;
	private float up_z;
	
	public RCamera()
	{
		center_x=0;
		center_y=1;
		this.center_z=0;
		
		this.position_x=0;
		this.position_y=0;
		this.position_z=0;
		
		this.up_x=0;
		this.up_y=0;
		this.up_z=1;
	}
	public void setCamera(stdCamPos c)
	{
		position_x=c._eye.x;
		position_y=c._eye.y;
		position_z=c._eye.z;
		
		center_x=c._center.x;
		center_y=c._center.y;
		center_z=c._center.z;
		
		up_x=c._up.x;
		up_y=c._up.y;
		up_z=c._up.z;
	}
	public void setPosition(float x,float y, float z)
	{
		this.position_x=x;
		this.position_y=y;
		this.position_z=z;
	}
	public void setCenter(float x, float y, float z)
	{
		this.center_x=x;
		this.center_y=y;
		this.center_z=z;
	}
	
	public void setUp(float x, float y, float z)
	{
		this.up_x=x;
		this.up_y=y;
		this.up_z=z;
	}
	public void sendPositionToServer(Transfer transfer)
	{
		if(transfer!=null)
			transfer.sendOnePacket(MessageType.CAMERAPOSITION, position_x, position_y, position_z);
	}
	public void sendCenterToServer(Transfer transfer)
	{
		if(transfer!=null)
		transfer.sendOnePacket(MessageType.CAMERACENTER, center_x, center_y, center_z);
	}
	public void sendUpToServer(Transfer transfer)
	{
		if(transfer!=null)
		transfer.sendOnePacket(MessageType.CAMERAUP, up_x, up_y, up_z);
	}
	
	public void sendCameraToServer(Transfer transfer)
	{
		if(transfer!=null)
		{
		  this.sendPositionToServer(transfer);
		  this.sendCenterToServer(transfer);
		  this.sendUpToServer(transfer);
		}
	}
	
	public void deliverToNDK()
	{
		Log.i("1SetCamera", "setcamera");
		stdCamPos ca = new stdCamPos();
		ca._eye=new Vector3();
		ca._center=new Vector3();
		ca._up=new Vector3();
		
		ca._eye.x=position_x;
		ca._eye.y=position_y;
		ca._eye.z=position_z;
		
		ca._center.x=center_x;
		ca._center.y=center_y;
		ca._center.z=center_z;
		
		ca._up.z=up_z;
		ca._up.y=up_y;
		ca._up.x=up_x;
		
		Log.i("2SetCamera", "setcamera");
		VinoNativeRender.SetCamera(ca);
	}
	
	public void SetCamAndsyncClientServer(Transfer transfer)
	{
		if(transfer!=null)
		{
			sendCameraToServer(transfer);
			deliverToNDK();
		}
	}
	public stdCamPos getCameraFromNdk()
	{
		stdCamPos cam=new stdCamPos();
		cam._eye=new Vector3();
		cam._center=new Vector3();
		cam._up=new Vector3();
		VinoNativeRender.getCamera(cam);
		setCamera(cam);
		return cam;
	}
	
	public stdCamPos  getCamera()
	{
		stdCamPos ca = new stdCamPos();
		ca._eye=new Vector3();
		ca._center=new Vector3();
		ca._up=new Vector3();
		
		ca._eye.x=position_x;
		ca._eye.y=position_y;
		ca._eye.z=position_z;
		
		ca._center.x=center_x;
		ca._center.y=center_y;
		ca._center.z=center_z;
		
		ca._up.z=up_z;
		ca._up.y=up_y;
		ca._up.x=up_x;
		
		return ca;
		
	}
	
}
