package com.example.vino;

import java.util.concurrent.Semaphore;

public class MotionDataBuffer
{
	private static final int MAXSIZE = 1000;
	private MotionData[] _buffer = null;
	
	private int _end;
	private int _top;
	private Semaphore _full = null;
	private Semaphore _empty = null;
	
	MotionDataBuffer()
	{
		_buffer = new MotionData[MAXSIZE];
		_end = 0;
		_top = 0;
		_full = new Semaphore(0);
		_empty = new Semaphore(MAXSIZE);
	}
	
	public MotionData getMotion()
	{
		MotionData tmp = null;
		try
		{
			_full.acquire();
			tmp = _buffer[_end];
			_end = (_end + 1) % MAXSIZE;
		}
		catch(InterruptedException e){
			e.printStackTrace();
		}
		_empty.release();
		return tmp;
	}
	
	public void putMotion(MotionData m)
	{	
		try
		{
			_empty.acquire();
			_buffer[_top] = m;
			_top = (_top + 1) % MAXSIZE;
		}
		catch(InterruptedException e)
		{
			e.printStackTrace();
		}
		_full.release();
	}
}
