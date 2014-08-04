package com.example.vino;


import android.util.Log;

public class SendThread implements Runnable {
	
	private TransferAdapter _adapter;
	private MotionDataBuffer	_mdBuffer;
    private Transfer _transfer;
    private Motion _motion;
	
	SendThread(TransferAdapter  adapter, MotionDataBuffer buffer)
	{
		_adapter = adapter;
		_mdBuffer = buffer;
        _motion=new Motion();
        _transfer=Transfer.getInstance();
        //
        _transfer.set_isConnected(_adapter.isConnected());
        _transfer.set_socket(_adapter.getSocket());
        _transfer.setInputstream(adapter.getInputstream());
        _transfer.setOutputstream(adapter.getOutputStrem());
        //

	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		MotionData tmp = null;
        if(_motion ==null)
        {
            _motion =new Motion();
        }
		while(true)
		{
			tmp = _mdBuffer.getMotion();

            _motion._type=tmp._type;
            _motion._x1=tmp._x1;
            _motion._y1=tmp._y1;
            _motion._x2=tmp._x2;
            _motion._y2=tmp._y2;
           // Log.i("motion","x1:    "+tmp._x1+"y1:  "+tmp._y1+"x2:    "+tmp._x2+" y2:      "+tmp._y2);
            //_adapter.sendOnePacket(_motion._type, _motion);
            _transfer.sendOnePacket(MessageType.RUNTIME_TOUCH_EVENT_MSG,_motion);
		}
	}

}
