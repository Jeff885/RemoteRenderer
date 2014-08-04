#include "RemoteViewer.h"
#include "ClientTable.h"
#include "Global.h"

ClientTable::ClientTable():isClear(true)
{
	next_client_ID=INVALID_CLIENT_ID+1;
}
// 构造方法
size_t ClientTable::getNewClientID()
{
	return next_client_ID++;
}
//返回Client
Client* ClientTable::getClient(size_t id)
{
	for(int i=0;i<_clients.size();i++)
	{

		if((_clients[i])->_client_id ==id)
		{

			return _clients[i];
		}
	}
	return NULL;
}
void ClientTable::removeClient(size_t id)
{
	cout<<"执行removeClinet"<<endl;
	for(vector<Client* >::iterator i=_clients.begin();i!=_clients.end();)
	{
		if((*i)->_client_id== id)
		{

			cout<<"删除客户端"<<id<<endl;
			delete *i;
			*i=NULL;
			_clients.erase(i);
			return;
		}else
			i++;
	}
	return;

}
void ClientTable::addClient(Client* client)
{
	_clients.push_back(client);
}

void ClientTable::setClientsSwitch(osg::Switch* node)
{

	for(int i=0;i<_clients.size();i++)
	{
		if((_clients[i])->toggle ==NULL)
		{
			(_clients[i])->toggle=node;
		}
	}
}
int ClientTable::ClientSize()
{
	return _clients.size();
}

void ClientTable::createClientCam(osg::Node* node)
{

	for(int i=0;i<_clients.size();i++)
	{

		if(!(_clients[i]->ishaveCam))
		{
			_clients[i]->createPredictionCamera(node);
		}
	}
}
void ClientTable::setPredictionToggle(bool tag)
{
	typedef vector<Client* >::iterator Itreator;
	for(Itreator i=_clients.begin();i<_clients.end();i++)
	{
		if((*i)->toggle!=NULL)
		{
			/*for(int j=0;j<=_clients[i]->toggle->getNumChildren();j++)
			{
				//cout<<j<<endl;
				_clients[i]->toggle->setValue(j,tag);
			}*/
			if(tag)
				(*i)->toggle->setAllChildrenOn();
			else
				(*i)->toggle->setAllChildrenOff();
		}
		
	}
}
void ClientTable::setTimeForPredictionToggleToTrue()
{
	
	for(int i=0;i<_clients.size();i++)
	{

		if(_clients[i]->isTimeForPrediction())
		{
			//_clients[i]->
			_clients[i]->setPredictionToggle(true);
			_clients[i]->UpdatePreViewMatrix();
			
			//_clients[i]->_lastPredictionTime=osg::Timer::instance()->tick();
		}
	}
}
void ClientTable::updateClientscamera()
{

	for(int i=0;i<_clients.size();i++)
	{
		_clients[i]->UpdatePreViewMatrix();

	}
}
bool ClientTable::addClientsToggleToMainCamera(osg::ref_ptr<osg::Group> SceneGroup)
{
	for(vector<Client* >::iterator i=_clients.begin();i!=_clients.end();i++)
	{
		if(!(*i)->_isDied && (*i)->isNiceConnect())
		{
			if((*i)->ishaveCam)
			{
				(*i)->addClientsToggleToMainCamera(SceneGroup);
			}

		}else
		{
			if(!(*i)->_isDied)
			{
				StopMessage* msg=new StopMessage;
				(*i)->mq->push(msg);
			}else
			{
				if(!(*i)->_isDataProcessed /*&& (*i)->_isInitial*/)
				{
					vector<Client* >::iterator it=i;
					delete *it;
					*it=NULL;
					i=_clients.erase(i);


				}
			}
			
			
		}
		if(i == _clients.end())
			break;
	}
	return true;
}
void ClientTable::Clearer()
{
}