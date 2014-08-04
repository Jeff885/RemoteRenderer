#include "ServerPipeline.h"
#include "Client.h"
#include "ClientTable.h"

float ServerPipeline::maxRotateRate = 25.0;
int ServerPipeline::referenceFramerate = 5;

ServerPipeline::ServerPipeline(ClientTable* _clientTable)
{
	this->_clientTable=_clientTable;
	_vmt=new ViewpointMgrTask();
	//_vmt=new RemoteRenderer::ViewpointMgrTask(this);

}
ClientTable* ServerPipeline::getClientTable()
{

	return _clientTable;
}
void ServerPipeline::startViewMsgTask(ServerPipeline* piple)
{

	//std::cout<<"startViewMsgTask"<<std::endl;
	_vmt->start(this);
}
void ServerPipeline::sendSockettoViewMgr(tcp_Socket* socket)
{
	_vmt->_socket=socket;

}

void ServerPipeline::sendMsgToViewMgr(Message *msg)
{
	ConsoleLogger logger;
	//logger.WARN("ServerPipeline sendMsgToViewMgr","sendMsgToViewMgr is running...\n");
	size_t id=msg->getClientID();
	if(id == INVALID_CLIENT_ID)
	{
		startViewMsgTask(this);//开启任务；
		MessageQueue* _mq=_vmt->getMessageQueue();
		_mq->push(msg);
	}else
	{
		MessageQueue* _mq=_vmt->getMessageQueue();
		_mq->push(msg);

	}

}

void ServerPipeline::sendMsgToLogger(const Message *msg)
{
	ConsoleLogger logger;
	logger.WARN("ServerPipeline sendMsgToLogger","sendMsgToLogger is running...\n");

}
void ServerPipeline::addPredictionToggle(osg::ref_ptr<osg::Switch> node)
{

	
}
osg::Node* ServerPipeline::getDataNode()
{
	
	return NULL;
}
float ServerPipeline::getMaxRotateRate()
{
	return maxRotateRate;
}
int ServerPipeline::getRefFrameRate()
{
	return referenceFramerate;
}
ServerPipeline::~ServerPipeline()
{
	cout<<"ServerPipeline 析构"<<endl;
	//delete _vmt;
}
