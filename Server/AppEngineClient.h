//2014_7_28  ��������ά�ṹչʾ
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
		float distance_07;//��ɫ�ǰ�_07;
		float distance_06;//��ɫ��Ƭ_06;
		float distance_05;//��ɫ��˿_05;
		float distance_04;//��ɫ��˿_04;
		float distance_03;//��ɫ�ǰ�_03;
		float distance_02;//��ɫ��˿_02;
		float distance_01;//��ɫ��˿_01;
		float distance_00;//��ɫ����_00;
		float distance_0_;//��ɫ���_0-;
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