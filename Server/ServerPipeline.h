// 流水线类头文件
// author：胡山峰
// date：2013-8-30

#ifndef _SERVER_PIPELINE_H
#define _SERVER_PIPELINE_H

#include "Global.h"
#include "Logger.h"
#include "ClientTable.h"
#include "ServerTask.h"
#include "MessageQueue.h"

class ViewpointMgrTask;
class ClientTable;
class ServerPipeline{
	public:
		static float maxRotateRate;
		static int referenceFramerate;
	public:
		ServerPipeline(ClientTable* _clientTable);
		
		void startViewMsgTask(ServerPipeline* piple);
		
		ClientTable* getClientTable();

		void VERBOSE(const string &log_tag, const string &log_msg);

		void DEBUG(const string &log_tag, const string &log_msg);

		void INFO(const string &log_tag, const string &log_msg);

		void WARN(const string &log_tag, const string &log_msg); 

		void ERR(const string &log_tag, const string &log_msg);

		void FATAL(const string &log_tag, const string &log_msg);

		void sendMsgToViewMgr(Message *msg);
		
		//void ServerPipeline::sendMsgToViewMgr(Message msg);
		void sendMsgToViewQuery(const Message *msg);

		void sendMsgToViewUpdate(const Message *msg);

		void sendMsgToSceneRenderer(const Message *msg);

		void sendMsgToImageWarper(const Message *msg);

		void sendMsgToImageEncoder(const Message *msg);

		void sendMsgToLogger(const Message *msg);
		void sendSockettoViewMgr(tcp_Socket* socket);

		void sendMsgToDataSender(const Message *msg);

		void sendMsgToDataReceiver(const Message *msg);

		float getMaxRotateRate();
		int getRefFrameRate();

		osg::Node* getDataNode();

		void addPredictionToggle(osg::ref_ptr<osg::Switch> node);
		~ServerPipeline();
		
private:
	ClientTable* _clientTable;
	ViewpointMgrTask* _vmt;
};


#endif