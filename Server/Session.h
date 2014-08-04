// 会话头文件
// author：胡山峰
// date：2013-8-30

#ifndef _Session_H
#define _Session_H

#include "Global.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "ClientTable.h"
#include "ServerPipeline.h"
#include "io_service_pool.h"
//#include "AppEngineClient.h"


typedef boost::asio::ip::tcp::socket  tcp_Socket;
typedef  boost::asio::io_service   io_service;
using boost::asio::ip::tcp;

/*#define write_char(buf, at, cp) ((buf+at)[0]=((unsigned char*)cp)[0])

#define write_uchar(buf, at, ucp) ((buf+at)[0]=((unsigned char*)ucp)[0])

#define write_short(buf, at, sp) ((buf+at)[0]=((unsigned char*)sp)[0],\
	(buf+at)[1]=((unsigned char*)sp)[1])

#define write_ushort(buf, at, usp) ((buf+at)[0]=((unsigned char*)usp)[0],\
	(buf+at)[1]=((unsigned char*)usp)[1])

#define write_int(buf, at, ip) ((buf+at)[0]=((unsigned char*)ip)[0],\
	(buf+at)[1]=((unsigned char*)ip)[1],\
	(buf+at)[2]=((unsigned char*)ip)[2],\
	(buf+at)[3]=((unsigned char*)ip)[3])

#define write_uint(buf, at, uip) ((buf+at)[0]=((unsigned char*)uip)[0],\
	(buf+at)[1]=((unsigned char*)uip)[1],\
	(buf+at)[2]=((unsigned char*)uip)[2],\
	(buf+at)[3]=((unsigned char*)uip)[3])

#define write_float(buf, at, fp) ((buf+at)[0]=((unsigned char*)fp)[0],\
	(buf+at)[1]=((unsigned char*)fp)[1],\
	(buf+at)[2]=((unsigned char*)fp)[2],\
	(buf+at)[3]=((unsigned char*)fp)[3])

#define write_double(buf, at, dp) (buf[0]=((unsigned char*)dp)[0],\
	buf[1]=((unsigned char*)dp)[1],\
	buf[2]=((unsigned char*)dp)[2],\
	buf[3]=((unsigned char*)dp)[3],\
	buf[4]=((unsigned char*)dp)[4],\
	buf[5]=((unsigned char*)dp)[5],\
	buf[6]=((unsigned char*)dp)[6],\
	buf[7]=((unsigned char*)dp)[7])

#define read_char(buf, at, cp) (*cp = *((char*)(buf+at)))

#define read_uchar(buf, at, ucp) (*ucp = *((unsigned char*)(buf+at)))

#define read_short(buf, at, sp) (*sp = *((short*)(buf+at)))

#define read_ushort(buf, at, usp) (*usp = *((unsigned short*)(buf+at)))

#define read_int(buf, at, ip) (*ip = *((int*)(buf+at)))

#define read_uint(buf, at, uip) (*uip = *((unsigned int*)(buf+at)))

#define read_float(buf, at, fp) (*fp = *((float*)(buf+at)))

#define read_double(buf, at, dp) (*dp = *((double*)(buf+at)))
*/
#define INVALID_CLIENT_ID 0

class ServerPipeline;


// 	// 会话类
// 	class Session {
// 
// 	public:
// 		Session(Client *client,boost::asio::io_service& io_serv):_socket(io_serv){
// 			_client = client;
// 			
// 			//_socket = new tcp_Socket(io_serv);
// 			//_service=new io_service;
// 			//_socket= new tcp_Socket(*_service);
// 			//_socket new tcp_Socket
// 			
// 		}
// 
// 		//author : 顾敏杰
// 		//绑定Socket
// 		/*
// 		int binding()
// 		{
// 			using namespace boost;
// 			using namespace boost::asio;
// 			using namespace boost::asio::ip;
// 
// 			tcp::acceptor acpt(*_service,tcp::endpoint(ip::address::from_string("219.224.168.73"),5588));
// 
// 			acpt.accept(*_socket);
// 			RemoteRenderer::ConsoleLogger Logger;
// 			Logger.INFO("Session binding","socket is binding\n");
// 			return 0;
// 		}
// 		*/
// 		
// 		// 从客户端接收一个消息
// 
// 		//??
// 		int receiveMsg(Message *&msg) {
// 			// 读取长度
// 			
// 			size_t len;
// 			read(*(_client->_socket), boost::asio::buffer(&len, 4));
// 
// 			// 读取完整消息体
// 			if(len > _client->_read_buffer_len){
// 				// 扩充读缓冲区
// 				delete[] _client->_read_buffer;
// 				size_t new_len = len+len/10;
// 				_client->_read_buffer = new unsigned char[new_len];
// 			}
// 			write_uint(_client->_read_buffer, 0, &len);
// 			read(*(_client->_socket), 
// 				boost::asio::buffer(_client->_read_buffer+4, len-4));
// 			
// 			// 根据消息类型构造消息
// 
// 			size_t msg_type;
// 			read_uint(_client->_read_buffer, 4, &msg_type);
// 			switch(msg_type){
// 			case CONTROL_START_MSG: msg = new StartMessage(); break;
// 			case CONTROL_STOP_MSG:  msg = new StopMessage();  break;
// 
// 			case INITIAL_CAMERA_MSG: msg = new CameraMessage(); break;
// 			case INITIAL_PERSPECTIVE_MSG: msg = new PerspectiveMessage(); break;
// 			case INITIAL_RESOLUTION_MSG: msg = new ResolutionMessage(); break;
// 
// 			case RUNTIME_TOUCH_EVENT_MSG: msg = new TouchEventMessage(); break;
// 
// 			default:
// 				// 未知消息类型
// 				return 0;
// 			}
// 
// 			// 反序列化
// 			msg->deserialize(_client->_read_buffer);
// 
// 			// 设置客户端ID
// 			msg->setClientID(_client->_client_id);
// 
// 			return 1;
// 		}
// 
// 		// 向客户端发送一个消息
// 
// 		//??
// 		int sendMsg(Message *msg) {
// 			size_t len = msg->getSize();
// 			if(len > _client->_write_buffer_len){
// 				// 扩充写缓冲区
// 				delete[] _client->_write_buffer;
// 				size_t new_len = len+len/10;
// 				_client->_write_buffer = new unsigned char[new_len];
// 			}
// 
// 			// 序列化
// 			size_t send_len = msg->serialize(_client->_write_buffer);
// 			assert(send_len == len);
// 
// 			// 发送
// 			write(*(_clien
// 			return 1;
// 		}
// 
// 		inline void startRead()
// 		{
// 		
// 			_socket.async_read_some(data,
// 				boost::bind(&Session::handle_read,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
// 			
// 		}
// 		inline void startWrite()
// 		{
// 			_socket.async_write_some(data,
// 				boost::bind(&Session::handle_write,this,boost::asio::placeholders::error));
// 		}
// 		void handle_read(const boost::system::error_code& error,size_t bytes_transferred)
// 		{
// 			if(!error)
// 			{
// 				/*
// 				size_t msg_type;
// 				read_uint(_client->_read_buffer, 4, &msg_type);
// 				//清空消息体
// 				if(msg != NULL)
// 					delete msg;
// 
// 				switch(msg_type){
// 				case CONTROL_START_MSG: msg = new StartMessage(); break;
// 				case CONTROL_STOP_MSG:  msg = new StopMessage();  break;
// 
// 				case INITIAL_CAMERA_MSG: msg = new CameraMessage(); break;
// 				case INITIAL_PERSPECTIVE_MSG: msg = new PerspectiveMessage(); break;
// 				case INITIAL_RESOLUTION_MSG: msg = new ResolutionMessage(); break;
// 
// 				case RUNTIME_TOUCH_EVENT_MSG: msg = new TouchEventMessage(); break;
// 
// 				default:
// 				}
// 				*/
// 				//继续异步非阻塞
// 				startWrite();
// 			}
// 			else
// 			{
// 				delete this;
// 			}
// 
// 		}
// 		void handle_write(const boost::system::error_code& error)
// 		{
// 			if(!error)
// 			{
// 				/*
// 				   服务器端开始向客户端发送数据，然后非阻塞监听
// 				*/
// 				//  do something();
// 				startRead();
// 			}else
// 			{
// 				delete this;
// 			}
// 
// 		}
// 		tcp::socket& socket()
// 		{
// 			return _socket;
// 		}
// 		virtual ~Session()
// 		{
// 			//boost::mutex::scoped_lock lock(cout_mtx);
// 			delete _client;
// 
// 		}
// 
// 	protected:
// 		Client *_client;
// 		Message* msg;  //消息
// 		tcp_Socket _socket; //每一个Session 一个套接字
// 		char data[4096];   ///数据
// 		//io_service *_service;
// 
// 	};t->_socket), boost::asio::buffer(_client->_write_buffer, len));
// 

enum {MAX_PACKET_LEN = 4096};
class Session
{
public:
	Session(boost::asio::io_service& io_service);

	virtual ~Session();


	tcp::socket& socket();
	
	inline void start();
	
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	
	void patchMsg(Message* msg);

	void  bufferCopy(unsigned char * source,unsigned char * dest,int len);

	Message* translateReceivedDataToMsg(size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);
private:
	ServerPipeline* _pipeline;
	tcp::socket socket_;
	unsigned char data_[MAX_PACKET_LEN];
	int seq;///客户端包的顺序
};
#endif