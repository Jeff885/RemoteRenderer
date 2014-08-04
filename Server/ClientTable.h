// �ͻ��˱���ͷ�ļ�
// author����ɽ��
// date��2013-8-29

#ifndef _CLIENT_TABLE_H
#define _CLIENT_TABLE_H

#include <vector>
#include"Client.h"
#include <osg/Switch>
#include "RemoteViewer.h"
using namespace std;
//�ͻ����б�
class Client;
class ClientTable {

	public:
	ClientTable();
		// ���췽��
	size_t getNewClientID();

	size_t getClientSize();
	
	//����Client
	Client* getClient(size_t id);
	
	void removeClient(size_t id);
	
	void addClient(Client* client);
	

	void setClientsSwitch(osg::Switch* node);
	
	int ClientSize();
	void startRemoteRenderer();

	void createClientCam(osg::Node* node);
	void setPredictionToggle(bool tag);
	void setTimeForPredictionToggleToTrue();
	void updateClientscamera();
	bool addClientsToggleToMainCamera(osg::ref_ptr<osg::Group> SceneGroup);
	void Clearer();
	//void setClientsisDataProcessed();
	
	//int 	
private:
		// ��һ��δ��ʹ�õ�ID
	bool isClear;
	std::vector<Client*> _clients;
	size_t next_client_ID;
};
#endif