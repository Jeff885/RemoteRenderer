//2014_7_28  发动机三维结构展示
#pragma once
#ifndef APPENGINECLIENT_H
#define APPENGINECLIENT_H
#include "Global.h"
#include <osg/Node>
#include "Client.h"
#include <osg/NodeCallback>
#include <osg/NodeVisitor>
#include <osg/MatrixTransform>


class Client;
class AppEngineClient : public Client
{
public:
	class TransfromCallback :public osg::NodeCallback
	{
	public:
		TransfromCallback();
		virtual void operator() (osg::Node* node, osg::NodeVisitor* nv);
	private:
		float velocity;
		float step;
		float max_distance;
		float distance_07;//灰色盖板_07;
		float distance_06;//蓝色垫片_06;
		float distance_05;//红色螺丝_05;
		float distance_04;//褐色螺丝_04;
		float distance_03;//绿色盖板_03;
		float distance_02;//橘色螺丝_02;
		float distance_01;//黄色螺丝_01;
		float distance_00;//黑色背板_00;
		float distance_0_;//灰色轴杆_0-;
		bool first;
	};
	class MatrixTransformExtractor: public osg::NodeVisitor
	{
	public:
		MatrixTransformExtractor();
		void apply(osg::Geode& geode);
	};
public:
	AppEngineClient(ViewpointMgrTask* vt);
	virtual ~AppEngineClient();
	osg::ref_ptr<osg::Node> DeepCopyModelData(osg::Node* node);
	void addTransfromCallback();
private:
	osg::ref_ptr<osg::Node> _node;
};
#endif