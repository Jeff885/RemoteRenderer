// 预测类头文件
// author：胡山峰
// date：2013-9-1

#ifndef _PREDICTOR_H
#define _PREDICTOR_H

#include "Global.h"

namespace RemoteRenderer {

	// 预测类
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