// Ԥ����ͷ�ļ�
// author����ɽ��
// date��2013-9-1

#ifndef _PREDICTOR_H
#define _PREDICTOR_H

#include "Global.h"

namespace RemoteRenderer {

	// Ԥ����
	class Predictor {
		Predictor(ServerPipeline *pipeline, Client *client) {
			_pipeline = pipeline;
			_client = client;
		}
	protected:
		Client *_client;
		ServerPipeline *_pipeline;
	};
}

#endif