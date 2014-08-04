package com.example.vino;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * Created by hu on 13-9-25.
 */

class MessageType
{
    public static final  int CONTROL_START_MSG=0;
    public static final  int CONTROL_STOP_MSG=1;
    public static final  int  CONTROL_SYN_MSG=2;

   
    public static final  int INITIAL_SCENE_MSG=3;
    public static final  int INITIAL_CAMERA_MSG=4;
    public static final  int INITIAL_PERSPECTIVE_MSG=5;
    public static final  int INITIAL_RESOLUTION_MSG=6;
    public static final  int INITIAL_INTERACTION_MSG=7;

    
    public static final  int RUNTIME_TOUCH_EVENT_MSG=8;
    public static final  int RUNTIME_REF_FRAME_MSG=9;
    public static final  int RUNTIME_VIEW_QUERY_MSG=10;
    public static final  int RUNTIME_SCENE_RENDERING_MSG=11;
    public static final  int RUNTIME_IMAGE_WARPING_MSG=12;
    public static final  int RUNTIME_VIEW_UPDATE_MSG=13;
    public static final  int RUNTIME_REF_ENCODING_MSG=14;
    public static final  int RUNTIME_LOG_MSG=15;
    
    public static final int APPLICATIONMSG=16;    //29
    public static final int  CAMERAPOSITION=17;  //29
	public static final int  CAMERACENTER=18;   //长度29字节
	public static final int  CAMERAUP=19;        //长度29字节

}
class Interaction
{
	public static final int TRACKBALLMANIPULATOR = 0;
	public static final int FIRSTPERSONMANIPULATOR =1;
	public static final int USER_DEFINE=2;
};
//2014_7_28
class ApplicationType
{
	public static final int DEFAULTVALUE = 0;//缺省值为1
	public static final int ENGINESHOW=1;//发动机结构展示
};
class Camera
{
  public   float _eyex;
  public   float _eyey;
  public  float  _eyez;
  public  float  _centerx;
  public float _centery;
  public float _centerz;
  public float  _upx;
  public float _upy;
  public float _upz;
  public int SIZE=36;

}

class Perspective
{
    public float _fov;
    public float _aspect;
    public float _near;
    public float _far;
    public int SIZE=16;
}
class Motion
{

    public byte _type;
    public float _x1 = 0.0f;
    public float _y1 = 0.0f;
    public float _x2 = 0.0f;
    public float _y2 = 0.0f;
    public static final int SIZE=17;
}
class InteractionMode
{
	public int mode;
	public static final int SIZE=4;
}
public class Transfer {
	public static Transfer adapter=null;

    private Socket _socket;
    private BufferedInputStream _input;
    private BufferedOutputStream _output;
    boolean _isConnected;
    public static Transfer getInstance()
    {
    	if(adapter == null)
    	{
    		adapter=new Transfer();
    	}
    	
    	return adapter;
    }
    public void set_socket(Socket s )
    {
        _socket=s;
    }
   public void set_isConnected(boolean connected)
   {
       _isConnected=connected;
   }
   public  void  setInputstream(BufferedInputStream input)
   {
       _input=input;
   }
    public void setOutputstream(BufferedOutputStream out)
    {
        _output=out;
    }
   private  Transfer()
    {
        _socket=null;
        _input=null;
        _output=null;
        _isConnected=false;

    }

    public boolean connect(String addr, int port)
    {
        try
        {
            _socket = new Socket(addr, port);
            _isConnected = true;
            _input = new BufferedInputStream(_socket.getInputStream());
            _output = new BufferedOutputStream(_socket.getOutputStream());
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
        return _isConnected;
    }

    public boolean isConnected()
    {
        return _isConnected;
    }
    
    //2014_8_3
    public void sendOnePacket(int type, float a, float b,float c)
    {
    	byte[] mType = toNetworkStream(type);
    	byte[] mLength= toNetworkStream(29);
    	byte[] mId     =toNetworkStream(0);
    	
    	byte[] mx=toNetworkStream(a);
    	byte[] my=toNetworkStream(b);
    	byte[] mz=toNetworkStream(c);
    	
    	byte[] mf=new byte[1];
    	mf[0]= (byte)0;
    	try {
			_output.write(mLength);
			_output.write(mType);
			_output.write(mId);
			_output.write(mx);
			_output.write(my);
			_output.write(mz);
			
			for(int i=0;i<5;i++)
			{
				_output.write(mf);
			}
			_output.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
    	
    }
    //204_7_7
    public void sendOnePacket(int type, InteractionMode im)
    {   
    	byte[] mType = toNetworkStream(type);
    	byte[] mLength =toNetworkStream(16);
    	byte[] mId=toNetworkStream(0);
    	byte[] mData=toNetworkStream(im.mode);
    	
    	try {
			_output.write(mLength);
			_output.write(mType);
			_output.write(mId);
			_output.write(mData);
			_output.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    }
    public void sendOnePacket(int type,int size,int apptype)
    {
        byte[] mType = toNetworkStream(type);
       // mType[0] = type;
        byte[] mLength=toNetworkStream(size);
        byte[] mApptype=toNetworkStream(apptype);
        try
        {
          //  Log.i("VIVO","!!!!!sendonePacket!!!!");
            _output.write(mLength);
            _output.write(mType);
            _output.write(mApptype);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
    //2014_7_31
    public void sendOnePacket(int type, int size,int partid, int op)
    {
    	 byte[] mType =toNetworkStream(type);
    	 byte[] mLength=toNetworkStream(size);
         byte[] mId=toNetworkStream(0);
         byte[] mPartid=toNetworkStream(partid);
         byte[] mOp=toNetworkStream(op);
         byte[] mFill=new byte[9];
         for(int i=0;i<=8;i++)
         {
        	 mFill[i]=((byte)0);
         }
         try
         {
        	 _output.write(mLength);
             _output.write(mType);
             _output.write(mId);
             _output.write(mPartid);
             _output.write(mOp);
             _output.write(mFill);
             _output.flush();
        	 
         }catch(IOException e)
         {
        	 e.printStackTrace();
         }
    	
    }
    public void sendOnePacket(int type,int size)
    {
        byte[] mType = toNetworkStream(type);
       // mType[0] = type;
        byte[] mLength=toNetworkStream(size);
        //byte[] mApptype=toNetworkStream(VinoActivity.ApplicationID);
        try
        {
          //  Log.i("VIVO","!!!!!sendonePacket!!!!");
            _output.write(mLength);
            _output.write(mType);
            //_output.write(mApptype);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
    protected byte[] toNetworkStream(int n)
    {
        byte[] a = new byte[4];
        a[0] = (byte)((n) & 0xff);
        a[1] = (byte)((n >> 8) & 0xff);
        a[2] = (byte)((n >> 16) & 0xff);
        a[3] = (byte)((n >> 24) & 0xff);
        return a;
    }
    protected byte[] toNetworkStream(float t)
    {
        return toNetworkStream(Float.floatToIntBits(t));
    }
    protected byte[] toNetworkStream(Camera pos)
    {
        byte[] a = toNetworkStream(pos._eyex);

        byte[] b = toNetworkStream(pos._eyey);

        byte[] c = toNetworkStream(pos._eyez);
        byte[] e=toNetworkStream(pos._centerx);
        byte[] f=toNetworkStream(pos._centery);
        byte[] g=toNetworkStream(pos._centerz);
        byte[] h=toNetworkStream(pos._upx);
        byte[] i=toNetworkStream(pos._upy);
        byte[] j=toNetworkStream(pos._upz);

        byte[] d = new byte[pos.SIZE];
        System.arraycopy(a, 0, d, 0, a.length);
        System.arraycopy(b, 0, d, 4, b.length);
        System.arraycopy(c, 0, d, 8, c.length);
        System.arraycopy(e,0,d,12,e.length);
        System.arraycopy(f,0,d,16,f.length);
        System.arraycopy(g,0,d,20,g.length);
        System.arraycopy(h,0,d,24,g.length);
        System.arraycopy(i,0,d,28,i.length);
        System.arraycopy(j,0,d,32,j.length);
        return d;
    }
    //2014_7_7
    public void sendOnePacket(int type, Camera pos)
    {
        byte[] mLength=toNetworkStream(48);
        byte[] mType =toNetworkStream(type);
        byte[] mId=toNetworkStream(0);
        byte[] mData = toNetworkStream(pos);
        try
        {
            _output.write(mLength);
            _output.write(mType);
            _output.write(mId);
            _output.write(mData);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
    /*
    public void sendOnePacket(byte type, ViewFrustum vf)
    {
        byte[] mType = new byte[1];
        mType[0] = type;
        byte[] mData = toNetworkStream(vf);
        try
        {
            _output.write(mType);
            _output.write(mData);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }*/

    protected byte[] toNetworkStream(Perspective vp)
    {
        byte[][] a = new byte[4][0];
        a[0] = toNetworkStream(vp._fov);
        a[1] = toNetworkStream(vp._aspect);
        a[2] = toNetworkStream(vp._near);
        a[3] = toNetworkStream(vp._far);
        byte[] d = new byte[ViewPerspective.SIZE];
        int i;
        for(i = 0; i < 4; ++i)
            System.arraycopy(a[i], 0, d, i*4, 4);
        return d;
    }
    public void sendOnePacket(int type, Perspective vp)
    {
        byte[] mLength=toNetworkStream(28);
        byte[] mType =toNetworkStream(type);
        byte[] mId =toNetworkStream(0);
        byte[] mData = toNetworkStream(vp);
        try
        {
            _output.write(mLength);
            _output.write(mType);
            _output.write(mId);
            _output.write(mData);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
    /*
    public void sendOnePacket(byte type, ViewResolution vr)
    {
        byte[] mType = new byte[1];
        mType[0] = type;
        byte[] mData = toNetworkStream(vr);
        try
        {
            _output.write(mType);
            _output.write(mData);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }*/
    /*
    public void sendOnePacket(byte type, UpsampleFactor uf)
    {
        byte[] mType = new byte[1];
        mType[0] = type;
        byte[] mData = toNetworkStream(uf);
        try
        {
            _output.write(mType);
            _output.write(mData);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }*/
    protected byte[] toNetworkStream(Motion m)
    {
        byte[] a1 = toNetworkStream(m._x1);
        byte[] b1 = toNetworkStream(m._y1);
        byte[] a2 = toNetworkStream(m._x2);
        byte[] b2 = toNetworkStream(m._y2);
        byte[] c = new byte[17];
        c[0]=m._type;
        System.arraycopy(a1, 0, c, 1, a1.length);
        System.arraycopy(b1, 0, c, 5, b1.length);
        System.arraycopy(a2, 0, c, 9, a2.length);
        System.arraycopy(b2, 0, c, 13, b2.length);
        return c;
    }

    public void sendOnePacket(int type, Motion md)
    {
        byte[] mLength = toNetworkStream(29);
        byte[] mType=toNetworkStream(type);
        byte[] mId=toNetworkStream(0);
        byte[] mData = toNetworkStream(md);
        try
        {
            _output.write(mLength);
            _output.write(mType);
            _output.write(mId);
            _output.write(mData);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
	public void sendOnePacket(int initialSceneMsg, String scenename) {
		int _fillLen=22-(scenename.length()+12);
		byte[] mLength = toNetworkStream(22);
		byte[] mType   = toNetworkStream(initialSceneMsg);
		byte[] mId     = toNetworkStream(0);
		byte[] mData   = toNetworkStream(scenename);
		byte[] mFill   = new byte[1];
		mFill[0]=(byte)(0);
		 try {
			_output.write(mLength);
			_output.write(mType);
	        _output.write(mId);
	        _output.write(mData);
	        for(int i=0;i<_fillLen;i++)
	        {
	        	_output.write(mFill);
	        }
	        _output.flush();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
         
	}
	private byte[] toNetworkStream(String scenename) {
		
		byte[] value=new byte[scenename.length()];
		for(int i=0;i<scenename.length();i++)
		{
			value[i]=(byte) ((scenename.charAt(i)) & 0xff);
			//value[2*i+1]=(byte) ((scenename.charAt(i)) & 0xff);
		}
		
		return value;
	}




}
