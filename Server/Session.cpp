#include "Session.h"
#include "AppEngineOperatorMessage.h"
#include "InterfaceMessage.h"
ClientTable* _gClientTable =new ClientTable();
Session::Session(boost::asio::io_service& io_service)
		: socket_(io_service)
	{
		_pipeline = new ServerPipeline(_gClientTable);
		seq=CONTROL_START_MSG;

	}
	 Session::~Session()
	{
		cout<<" Session 析构"<<endl;
		delete _pipeline;

		//boost::mutex::scoped_lock lock(cout_mtx);

	}
	tcp::socket& Session::socket()
	{
		return socket_;
	}

	inline void Session::start()
	{
		//Session开启读缓存的异步非阻塞，当一个客户端数据包过来，就读取网络数据包
		//并且触发handle_read;
		// 		boost::asio::async_read(socket_,boost::asio::buffer(data_, MAX_PACKET_LEN),
		// 			boost::bind(&Session::handle_read, this,
		// 			boost::asio::placeholders::error,
		// 			boost::asio::placeholders::bytes_transferred));
		/*boost::asio::async_read(socket_,boost::asio::buffer(data_, 100),boost::bind(&Session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));*/
		int size=MAX_PACKET_LEN;
		switch(seq)
		{
		case CONTROL_START_MSG:
			size=12;
			break;
		case INITIAL_PERSPECTIVE_MSG:
			size=28;
			break;
		case INITIAL_CAMERA_MSG:
			size=48;
			break;
		case RUNTIME_TOUCH_EVENT_MSG:
			size=29;
			break;
		case APPLICATIONMSG:
			size=29;
			break;
		case CAMERAPOSITION:
		case CAMERACENTER:
		case CAMERAUP:
			size=29;
			break;
		//2014_7_7
		case INITIAL_INTERACTION_MSG:
			size=16;
			break;
		case INITIAL_SCENE_MSG:
			size=22;
			break;
		case CONTROL_STOP_MSG:
			size=8;
			break;

		default:
			break;
		}
		socket_.async_read_some(boost::asio::buffer(data_, size),
			boost::bind(&Session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (!error)
		{
			
			//printf("transferred :%d\n",bytes_transferred);
			//消息解析处理
			
			patchMsg(translateReceivedDataToMsg(bytes_transferred));
			//触发一个写的异步阻塞
			//boost::asio::async_write(socket_, boost::asio::buffer(data_, bytes_transferred),
				//boost::bind(&Session::handle_write, this, boost::asio::placeholders::error));
			start();

		}
		else
		{
			delete this;
		}
	}
	void Session::patchMsg(Message* msg)
	{

		if(msg==NULL)
		{
			cout<<"patchMsg:Message is NULL"<<endl;
			return;
		}
		size_t msg_type=msg->getType();

		switch(msg_type)
		{
		
		case CONTROL_START_MSG:
			{
				
				msg->setClientID(INVALID_CLIENT_ID);
				//分派消息给视点管理模块
				_pipeline->sendSockettoViewMgr(&socket_);
				_pipeline->sendMsgToViewMgr(msg);
				seq=INITIAL_PERSPECTIVE_MSG;
				break;
			}
		case CONTROL_STOP_MSG:
			{

				cout<<"STOP消息"<<endl;
				_pipeline->sendMsgToViewMgr(msg);
				break;
			}
		case INITIAL_SCENE_MSG:
			{
				cout<<"初始化场景消息"<<endl;
				_pipeline->sendMsgToViewMgr(msg);
				seq=RUNTIME_TOUCH_EVENT_MSG;
				break;
			}
		case INITIAL_CAMERA_MSG:
			{
				//客户端发送的包，包含长度，类型，Id，内容
				_pipeline->sendMsgToViewMgr(msg);
				//2014_7_7
				//seq=CONTROL_START_MSG;
				seq=INITIAL_INTERACTION_MSG;
				break;
				
			}
		case INITIAL_PERSPECTIVE_MSG:
			{
				cout<<"接受视点投影参数"<<endl;
				_pipeline->sendMsgToViewMgr(msg);
				seq=INITIAL_CAMERA_MSG;
				break;

			}
		case INITIAL_INTERACTION_MSG:
			{
				cout<<"交互模式"<<endl;
				_pipeline->sendMsgToViewMgr(msg);
				seq=INITIAL_SCENE_MSG;
				break;
			}
		case INITIAL_RESOLUTION_MSG:
			{


			}
		case RUNTIME_TOUCH_EVENT_MSG:
			{ 
				//cout<<"..........RUNTIME_TOUCH_EVENT_MSG........."<<endl;
				//长度，类型，Id和值

				//Manipulator  更新Touch_event
				_pipeline->sendMsgToViewMgr(msg);

				seq=RUNTIME_TOUCH_EVENT_MSG;
				break;
			}
		case APPLICATIONMSG:
			{
				_pipeline->sendMsgToViewMgr(msg);

				seq=APPLICATIONMSG;

				break;
			}
		case CAMERAPOSITION:
			{

				seq=CAMERAPOSITION;
				_pipeline->sendMsgToViewMgr(msg);
				cout<<"postion"<<endl;
				break;
			}
		case CAMERACENTER:
			{
				seq=CAMERACENTER;
				_pipeline->sendMsgToViewMgr(msg);
				cout<<"center"<<endl;
				break;
			}
		case CAMERAUP:
			{
				seq=CAMERAUP;
				_pipeline->sendMsgToViewMgr(msg);
				cout<<"up"<<endl;
				break;
			}
		default:
			break;
		}
	}

	void  Session::bufferCopy(unsigned char * source,unsigned char * dest,int len)
	{
		if(source== NULL || dest == NULL)
		{
			std::cout<<"buffer Copy error"<<std::endl;
			return;
		}

		for(int i=0;i<len;i++)
		{
			dest[i]=source[i];	
		}

	}

	Message* Session::translateReceivedDataToMsg(size_t bytes_transferred)
	{
		//std::cout<<"translateReceivedDataToMsg"<<std::endl;
		// 读取长度		
		Message* msg;
		size_t len=bytes_transferred;
		unsigned char* _buffer=new unsigned char[len];
		//strcpy(_buffer,data_);
		//read(*(_client->_socket), boost::asio::buffer(&len, 4));
		//write_uint(_buffer, 0, &len);

		//将data_中的数据复制到_buffer中
		this->bufferCopy(data_,_buffer,len);
		//read(socket_, boost::asio::buffer(_buffer+4, len-4));

		// 根据消息类型构造消息
		unsigned int msg_type;
		int mylength;

		read_int(_buffer,0,&mylength);
		read_uint(_buffer, 4, &msg_type);
		//cout<<"!!!!!消息类型："<<msg_type<<endl;
		//cout<<"消息长度：" <<mylength<<endl;
		switch(msg_type){
		case CONTROL_START_MSG: msg = new StartMessage(); break;
		case CONTROL_STOP_MSG:  msg = new StopMessage();  break;
        
		case INITIAL_SCENE_MSG: {msg=new StringMessage(mylength-12); 
			msg->setType(INITIAL_SCENE_MSG); 
			//cout<<"消息长度："<<mylength<<endl;
			break;
		}
		case INITIAL_CAMERA_MSG: msg = new CameraMessage(); break;
		case INITIAL_PERSPECTIVE_MSG: msg = new PerspectiveMessage(); break;
		case INITIAL_RESOLUTION_MSG:/* msg = new ResolutionMessage(); */break;
	
        //2014_7_7
		case INITIAL_INTERACTION_MSG: msg= new InteractionMessage();break;	
		case RUNTIME_TOUCH_EVENT_MSG: msg = new TouchEventMessage(); break;
		case APPLICATIONMSG: {msg=new AppEngineOperatorMessage();
			cout<<"应用层消息"<<endl;
			break;}
		case CAMERAPOSITION:msg =new CameraPositionMsg();break;
		case CAMERACENTER: msg=new CameraCenterMsg();break;
		case CAMERAUP: msg=new CameraUpMsg();break;

		default:
			// 未知消息类型
			return NULL;
		}

		// 反序列化
		msg->deserialize(_buffer);

		// 设置客户端ID
		//msg->setClientID(_client->_client_id);

		//return 1;
		delete _buffer;
		return msg;

	}
	void Session::handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			//printf("server say to client:%s\n",data_);
			start();
		}
		else
		{
			delete this;
		}
	}



