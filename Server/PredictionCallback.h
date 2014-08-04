#ifndef		__PREDICTION_CALLBACK_H__
#define		__PREDICTION_CALLBACK_H__

#include "Global.h"
#include "Client.h"
class Client;
class RemoteViewer;


class PredictionCallback : public osg::Camera::DrawCallback
{
public:
	static int framecount;
	static void startCompress(RemoteViewer* view);
	static void startClientCompress(Client* _client);
	virtual void operator () (osg::RenderInfo& renderInfo) const;
	static unsigned int StringToId(string id);
	//static void localWarping();
};

#endif