package com.example.vino;
import android.util.Log;





public class ConnectThread implements Runnable {

	private VinoActivity  _activ;
	private TransferAdapter _adapter;
	private String _address;
	private int _port;
	private LocalInfoCapturer _capturer;
    private  Transfer   _transfer;

    private Camera _cam;//鎽勫儚澶�
    private Perspective _per;//閫忚鍙傛暟
    private Motion _motion;//鐢ㄦ埛瑙﹀睆鏁版嵁
    
    
    //2014_7_7
    private InteractionMode _im;

	ConnectThread(VinoActivity activ)
	{
		_activ = activ;
		_adapter = activ.getAdapter();
		_address = VinoActivity.ADDRESS;
		_port = VinoActivity.PORT;
		_capturer = activ.getCapturer();
        _transfer = Transfer.getInstance();

        _cam=new Camera();
        _per=new Perspective();
        _motion=new Motion();
        
        //2014_7_7
        _im=new InteractionMode();
	}
	
	@Override
	public void run() {
            Log.i("VIVO","!!!!!!!");
                // TODO Auto-generated method stub
           if(_adapter.connect(_address, _port))
            {
                Log.i("VIVO","connected!!");


                // Camera cam;
               /*
			_adapter.sendOnePacket(DataType.VINO_CONFIG_CAMERA_STANDARD, _capturer.getCameraPos());
            _adapter.sendOnePacket(DataType.VINO_CONFIG_VIEW_PERSPECTIVE, _capturer.getPerpective());
            _adapter.sendOnePacket(DataType.VINO_CONFIG_VIEW_RESOLUTION, _capturer.getResolution());
           _adapter.sendOnePacket(DataType.VINO_CONFIG_QUALITY_UPSAMPLE, _capturer.getUpsample());
            _adapter.sendOnePacket(DataType.VINO_CONFIG_SYN);
            */

            //Camera

            _transfer.set_socket(_adapter.getSocket());
            _transfer.set_isConnected(true);
            _transfer.setInputstream(_adapter.getInputstream());
            _transfer.setOutputstream(_adapter.getOutputStrem());

            _cam._eyex= _capturer.getCameraPos()._eye.x;
            _cam._eyey= _capturer.getCameraPos()._eye.y;
            _cam._eyez= _capturer.getCameraPos()._eye.z;
            _cam._centerx= _capturer.getCameraPos()._center.x;
            _cam._centery= _capturer.getCameraPos()._center.y;
            _cam._centerz=_capturer.getCameraPos()._center.z;
            _cam._upx=_capturer.getCameraPos()._up.x;
            _cam._upy= _capturer.getCameraPos()._up.y;
            _cam._upz= _capturer.getCameraPos()._up.z;
            //Perspective
            _per._fov=_capturer.getPerpective()._fov;
            _per._aspect=_capturer.getPerpective()._aspect;
            _per._near= _capturer.getPerpective()._near;
            _per._far=_capturer.getPerpective()._far;
            //交互模式
            _im.mode=VinoActivity._interactionmode;
            Log.i("VINO", "send first data");
            //
            //_motion._type=MessageType.RUNTIME_TOUCH_EVENT_MSG;
           // _motion._x1=
            //_motion._x1=__
            
            //_transfer.sendOnePacket(MessageType.CONTROL_STOP_MSG,8);
            //_transfer.sendOnePacket(MessageType.CONTROL_START_MSG,8);
            _transfer.sendOnePacket(MessageType.CONTROL_START_MSG,12,VinoActivity.ApplicationID);//2014_7_28
            
           
            _transfer.sendOnePacket(MessageType.INITIAL_PERSPECTIVE_MSG,_per);
            _transfer.sendOnePacket(MessageType.INITIAL_CAMERA_MSG,_cam);
            _transfer.sendOnePacket(MessageType.INITIAL_INTERACTION_MSG,_im);
            _transfer.sendOnePacket(MessageType.INITIAL_SCENE_MSG,VinoActivity._sceneName);
            if(VinoActivity.isAppmode)
            {
            	_transfer.sendOnePacket(MessageType.APPLICATIONMSG, 29, 1, 1);
            	//_transfer.sendOnePacket(MessageType.APPLICATIONMSG, 29, 9, 1);
            }
            
            //test 2014_8_3
            RCamera ca = new RCamera();
            ca.setCamera(_capturer.getCameraPos());
            //ca.sendCameraToServer(_transfer);
            ca.SetCamAndsyncClientServer(_transfer);
            
            
            Log.i("Camera eye %d %d %d",_cam._eyex+"  "+ _cam._eyey +"    "+_cam._eyez);
            Log.i("Camera eye %d %d %d",_cam._centerx+"  "+ _cam._centery +"    "+_cam._centerz);
            Log.i("Camera eye %d %d %d",_cam._upx+"  "+ _cam._upy +"    "+_cam._upz);
            
            
            stdCamPos cm=ca.getCameraFromNdk();
            
            Log.i("Camera eye %d %d %d",cm._eye.x+"  "+ cm._eye.y +"    "+cm._eye.z);
            Log.i("Camera eye %d %d %d",cm._center.x+"  "+ cm._center.y +"    "+cm._center.z);
            Log.i("Camera eye %d %d %d",cm._up.x+"  "+ cm._up.y +"    "+cm._up.z);
            
            


		}
		
     	DataPacket rcData = _adapter.receiveOnePacket();
		
		Log.i("VINO", "receive first data```````````````````````````````````````````````````````````````````````````");
		
		VinoNativeRender.setData(rcData);
		
		_activ.getHandler().post(new Runnable() {
		    public void run()
				{
					_activ.setupVinoView();
				}
		});
	}
}
