package com.example.vino;

import android.util.Log;

import java.net.Socket;
import java.io.IOException;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;

//import android.util.Log;


class DataType
{
	public static final byte VINO_CONFIG_SYN  = 0;
	public static final byte VINO_CONFIG_VIEW_FRUSTUM = 0x08;
	public static final byte VINO_CONFIG_VIEW_PERSPECTIVE = 0x09;
	public static final byte VINO_CONFIG_VIEW_RESOLUTION = 0x0A;
	public static final byte VINO_CONFIG_CAMERA_STANDARD = 0x10;
	public static final byte VINO_CONFIG_QUALITY_UPSAMPLE = 0x18;
	public static final byte VINO_MODEL_3DIMAGE = 0x40;
	public static final byte VINO_MOTION_TOUCH_STANDARD = (byte) 0x80;
}


class DataPacket
{
	public int  _cLen;
	public int  _dSrcLen;
	public int	_dDstLen;
	public float[]  _mat;
	public byte[]	_cData;
	public byte[]	_dData;
	
	DataPacket(int len1, int len2, int len3)
	{
		_cLen = len1;
		_dSrcLen = len2;
		_dDstLen = len3;
		_mat = new float[16];
		_cData = new byte[len1];
		_dData = new byte[len3];
	}
}


public class TransferAdapter 
{
	private Socket		_socket;
	private BufferedInputStream 	_input;
	private BufferedOutputStream	_output;
	boolean _isConnected;

    Socket getSocket()
    {
        return _socket;
    }
    BufferedInputStream getInputstream()
    {
        return _input;
    }
    BufferedOutputStream getOutputStrem()
    {
        return _output;
    }
	TransferAdapter()
	{
		_socket = null;
		_input = null;
		_output = null;
		_isConnected = false;
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
            Log.i("SOCKET",e.toString());
            e.printStackTrace();
           // Log.i("VIVO","connection exception~~~~");
		}
		return _isConnected;
	}
	
	public boolean isConnected()
	{
		return _isConnected;
	}
	
	
	public DataPacket receiveOnePacket()
	{
		byte[] mType = new byte[1];
		byte[] mHead = new byte[76];
		DataPacket dp = null;
		int readSize;
		readSize = fullRead(mType, 0, mType.length);
		if(readSize != 1) return dp;
		if(mType[0] == DataType.VINO_MODEL_3DIMAGE)
		{
			
			readSize = fullRead(mHead, 0, mHead.length);
			//Log.i( "VINO", "head:" + String.valueOf(readSize));
			
			dp = parse3DImageHead(mHead);
			
			readSize = fullRead(dp._cData, 0, dp._cData.length);
			//Log.i( "VINO", "color:" + String.valueOf(readSize));
			
			readSize = fullRead(dp._dData, 0, dp._dData.length);
			//Log.i( "VINO", "depth:" + String.valueOf(readSize));
		}
		return dp;
	  }
	
	protected int fullRead(byte[] data, int off, int len)
	{
		try
		{
			int nextLen = len, pos = off,readLen;
			while(nextLen > 0)
			{
				readLen = _input.read(data, pos, nextLen);
				if(readLen >= 0) 
				{
					nextLen -= readLen;
					pos += readLen;
				}
			}
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		return len;
	}
	
	protected DataPacket parse3DImageHead(byte[] head)
	{
		int len1 = toLocalInt(head, 0);
		int len2 = toLocalInt(head, 4);
		int len3 = toLocalInt(head, 8);
		
		//Log.i( "VINO", "Len:" + String.valueOf(len1) + "," + String.valueOf(len2) + "," +String.valueOf(len3));
		DataPacket dp = new DataPacket(len1, len2, len3);
		int i;
		for(i = 0; i < 16; ++i)
			dp._mat[i] = Float.intBitsToFloat( toLocalInt(head, 12 + i * 4) );
		return dp;
	}
	
	public void sendOnePacket(int type,int size)
    {
        byte[] mType = toNetworkStream(type);
       // mType[0] = type;
        byte[] mLength=toNetworkStream(size);
        try
        {
          //  Log.i("VIVO","!!!!!sendonePacket!!!!");
            _output.write(mLength);
            _output.write(mType);
            _output.flush();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
	
	public void sendOnePacket(byte type)
	{
		byte[] mType = new byte[1];
		mType[0] = type;
		try
		{
			_output.write(mType);
			_output.flush();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}
	public void sendOnePacket(byte type, stdCamPos pos)
	{
		byte[] mType = new byte[1];
		mType[0] = type;
		byte[] mData = toNetworkStream(pos);
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
	}
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
	}
	public void sendOnePacket(byte type, ViewPerspective vp)
	{
		byte[] mType = new byte[1];
		mType[0] = type;
		byte[] mData = toNetworkStream(vp);
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
	}
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
	}
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
	}
	public void sendOnePacket(byte type, MotionData md)
	{
		byte[] mType = new byte[1];
		mType[0] = type;
		byte[] mData = toNetworkStream(md);
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
	}
	
	public static int toLocalInt(byte[] data, int pos)
	{
		int tmp = 0;
		tmp |= ((data[pos] << 24) & 0xff000000);
		tmp |= ((data[pos + 1] << 16) & 0xff0000);
		tmp |= ((data[pos + 2] << 8) & 0xff00);
		tmp |= (data[pos + 3] & 0xff);
		return tmp;
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
	
	protected byte[] toNetworkStream(Vector3 v)
	{
		byte[] a = toNetworkStream(v.x);
		byte[] b = toNetworkStream(v.y);
		byte[] c = toNetworkStream(v.z);
		byte[] d = new byte[12];
		System.arraycopy(a, 0, d, 0, a.length);
		System.arraycopy(b, 0, d, 4, b.length);
		System.arraycopy(c, 0, d, 8, c.length);
		return d;
	}
	
	protected byte[] toNetworkStream(MotionData m)
	{
		byte[] a1 = toNetworkStream(m._x1);
		byte[] b1 = toNetworkStream(m._y1);
		byte[] a2 = toNetworkStream(m._x2);
		byte[] b2 = toNetworkStream(m._y2);
		byte[] c = new byte[17];
		c[0] = m._type;
		System.arraycopy(a1, 0, c, 1, a1.length);
		System.arraycopy(b1, 0, c, 5, b1.length);
		System.arraycopy(a2, 0, c, 9, a2.length);
		System.arraycopy(b2, 0, c, 13, b2.length);
		return c;
	}
	
	protected byte[] toNetworkStream(stdCamPos pos)
{
    byte[] a = toNetworkStream(pos._eye);
    byte[] b = toNetworkStream(pos._center);
    byte[] c = toNetworkStream(pos._up);
    byte[] d = new byte[stdCamPos.SIZE];
    System.arraycopy(a, 0, d, 0, a.length);
    System.arraycopy(b, 0, d, 12, b.length);
    System.arraycopy(c, 0, d, 24, c.length);
    return d;
}
	protected byte[] toNetworkStream(ViewFrustum vf)
	{
		byte[][] a = new byte[6][];
		a[0] = toNetworkStream(vf._left);
		a[1] = toNetworkStream(vf._right);
		a[2] = toNetworkStream(vf._bottom);
		a[3] = toNetworkStream(vf._top);
		a[4] = toNetworkStream(vf._near);
		a[5] = toNetworkStream(vf._far);
		
		byte[] d = new byte[ViewFrustum.SIZE];
		int i;
		for(i = 0; i < 6; ++i) 
			System.arraycopy(a[i], 0, d, i*4, 4);
		return d;
	}
	protected byte[] toNetworkStream(ViewPerspective vp)
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
	protected byte[] toNetworkStream(ViewResolution vr)
	{
		byte[] a = toNetworkStream(vr._width);
		byte[] b = toNetworkStream(vr._height);
		byte[] d= new byte[ViewResolution.SIZE];
		System.arraycopy(a, 0, d, 0, a.length);
		System.arraycopy(b, 0, d, 4, b.length);
		return d;
	}
	protected byte[] toNetworkStream(UpsampleFactor uf)
	{
		byte[] a = toNetworkStream(uf._factX);
		byte[] b = toNetworkStream(uf._factY);
		byte[] d= new byte[UpsampleFactor.SIZE];
		System.arraycopy(a, 0, d, 0, a.length);
		System.arraycopy(b, 0, d, 4, b.length);
		return d;
	}
}
