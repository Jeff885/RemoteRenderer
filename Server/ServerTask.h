// 流水线任务类文件
// author：胡山峰
// date：2013-8-30

#ifndef _SERVER_TASK_H
#define _SERVER_TASK_H


#include "Global.h"
#include "Message.h"
#include "Session.h"
#include "Client.h"
#include "MessageQueue.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "ServerPipeline.h"
class Client;
class ServerPipeline;
	// 流水线任务抽象类，所有具体的任务类需要继承该类以自定义具体工作
class ServerTask {

	public:
		// 返回该任务的消息队列
		virtual MessageQueue* getMessageQueue() =0;
		// 任务体
		virtual void run() = 0;

		// 析构方法
		 ~ServerTask()
		 {

		 }
		 
	};

	// 日志任务
	class LoggerTask : public ServerTask {

	public:
		LoggerTask() {
			_queue = new MessageQueue();
			_logger = new ConsoleLogger();
		}

		virtual MessageQueue* getMessageQueue(){
			return _queue;
		}

		Logger* getLogger() {
			return _logger;
		}

		virtual void run() {
			_logger->WARN("LoggerTask","LoggerTask is running....\n");
			while(true){
				// 出队消息
				Message *msg;
				_queue->wait_and_pop(msg);

				// 处理消息
				size_t msg_type = msg->getType();
				if(msg_type == RUNTIME_LOG_MSG){
					// log
					LogMessage *log_msg = dynamic_cast<LogMessage*>(msg);
					LogRecord log;
					log_msg->getLogArgs(log);
					_logger->doLog(log);
				}
			}
		}

		~LoggerTask() {
			delete _queue;
			delete _logger;
		}

	private:
		MessageQueue *_queue;
		Logger *_logger;
	};

	// 数据接收任务
	/*
	class DataReceiverTask : public ServerTask {



	public:
		DataReceiverTask(RemoteRenderer::ServerPipeline *_pipeline,RemoteRenderer::Session* _session) {
			this->_session = _session;
			this->_pipeline = _pipeline;


		}

		virtual MessageQueue* getMessageQueue() const {
			return NULL;
		}

		virtual void run() {
				// 从客户端读取消息
				Message *msg;
				//tcp_Socket *_socket;
				//io_service *_service;
				//
				//_session->binding();
				try{
					//_session->receiveMsg(msg);

				}catch(exception &ex){
					// 出现异常
					_pipeline->sendMsgToViewMgr(new StopMessage(_client->_client_id));
					//proceed = false;
					delete msg;
					//break;
				}

				// 处理消息
				size_t msg_type = msg->getType();
				switch(msg_type){
				case CONTROL_START_MSG:
					{
						// start
						StartMessage *start_msg = dynamic_cast<StartMessage*>(msg);
						_pipeline->sendMsgToLogger(new StartMessage(*start_msg));
						_pipeline->sendMsgToViewMgr(new StartMessage(*start_msg));
						delete msg;
						break;
					}
					

				case CONTROL_STOP_MSG:
					{
						// stop
						StopMessage *stop_msg = dynamic_cast<StopMessage*>(msg);
						_pipeline->sendMsgToLogger(new StopMessage(*stop_msg));
						_pipeline->sendMsgToViewMgr(new StopMessage(*stop_msg));
						//proceed = false;
						delete msg;
						break;

					}
					

				default:
					// others
					_pipeline->sendMsgToViewMgr(msg);
					break;
				}

				
			}
		~DataReceiverTask() {
			delete _queue;
			delete _session;
		}

	private:
		RemoteRenderer::Session * _session;
		RemoteRenderer::ServerPipeline * _pipeline;
	};
		
		*/


		//socket以及io_service
		
/*
	// 数据发送任务
	class DataSenderTask : public ServerTask {

	public:
		DataSenderTask(ServerPipeline *pipeline, Client *client) {
			_queue = new MessageQueue();
			_client = client;
//			_session = new Session(client);
			_pipeline = pipeline;
		}

		virtual MessageQueue* getMessageQueue() const{
			return _queue;
		}

		virtual void run() {
			while(true){

				bool proceed = true;

				// 出队消息
				Message *msg;
				_queue->wait_and_pop(msg);

				// 处理消息
				size_t msg_type = msg->getType();
				switch(msg_type){
				case CONTROL_START_MSG:
					// start
					delete msg;
					break;

				case CONTROL_STOP_MSG:
					// stop
					proceed = false;
					delete msg;
					break;

				default:
					// others
					try{
					//	_session->sendMsg(msg);
					}catch(exception &ex){
						_pipeline->sendMsgToViewMgr(new StopMessage(_client->_client_id));
					}
					proceed = false;
					delete msg;
					break;
				}

				if(!proceed){
					break;
				}
			}
		}

		~DataSenderTask() {
			delete _queue;
			delete _session;
		}

	private:
		RemoteRenderer::MessageQueue *_queue;
		RemoteRenderer::Client *_client;
		RemoteRenderer::Session *_session;
		RemoteRenderer::ServerPipeline *_pipeline;
	};
	*/


	// 视点管理任务
	class ViewpointMgrTask : public ServerTask {

	public:
		 ViewpointMgrTask();
		virtual MessageQueue* getMessageQueue();
		void start(ServerPipeline* pl);
		virtual void run();
		~ViewpointMgrTask();
		void initClient(Client *client);
		void initCameraPos();
		Client* getClient();
		bool _isRunning;
		tcp_Socket* _socket;
		bool _runSwitch;
	private:

		void initPerspective(Client *client);
		// 初始化控制器
		void initManipulator(Client *client);
		// 初始化场景节点
		void initSceneNode(Client *client,string sceneName);
		// 加载shader
		osg::Program* loadShader(const string &vert, const string &frag);
		// 创建一个正方形几何节点
		osg::Geode* createQuad(Client *client);
	private:
		MessageQueue *_queue;
		ServerPipeline* _pipeline;
		boost::thread* _thread;
		Client*  _client;
		
	};
#endif