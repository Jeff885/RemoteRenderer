// �ͻ�����ͷ�ļ�
// author����ɽ��
// date��2013-8-29

#ifndef _CLIENT_H
#define _CLIENT_H

#include "Global.h"
#include "Manipulator.h"
#include "ScreenQuad.h"
#include "PredictionCallback.h"
#include "Message.h"
#include "TouchEventBuffer.h"
#include "RemoteFirstPersonManipulator.h"
#include "ServerTask.h"


extern enum DataType;
class Compressor;
class Manipulator;
class TouchEventBuffer;
class ViewpointMgrTask;
class MessageQueue;
///��������ά���Ŀͻ��˳���ṹ��
class Client {
public:
		Client(ViewpointMgrTask* vmt);
		//Client();
		virtual ~Client();
		//2014_8_4
		void setMessageQueue(MessageQueue* mq);
		void UpdatePreViewMatrix();
		void createPredictionCamera(osg::Node* node);
		bool isTimeForPrediction();
		void setPredictionToggle(bool tag);
		void addClientsToggleToMainCamera(osg::ref_ptr<osg::Group> SceneGroup);
		void setLightBoundingBox(osg::Node* node);
		void sentOnePacket( 
			DataType dataType, 
			unsigned int cLen, 
			unsigned char* cBuf, 
			unsigned int dsLen, 
			unsigned int ddLen,
			char* dBuf,
			osg::Matrixf vp);
		void putInt(unsigned char* data, const unsigned int& val);
	    osg::Matrixf getPredInvMVP();
		void setUpCompressor(Resolution& r);
		osg::ref_ptr<osg::Camera> getColorCamera();
		bool isNiceConnect();
		
		//void calPredictionconfig();
		string id2stirng();
		osg::Program* createProgram(const std::string vert, const std::string frag);
		// --------����ʱ��ʼ��---------
		tcp_Socket *_socket;
		io_service *_service;

		size_t _client_id;// ��ʼ����ID
		
		//size_t _read_buffer_len, _write_buffer_len;
		//unsigned char *_read_buffer;
		//unsigned char *_write_buffer;

		double _pred_period;

		double _frame_time;

		double _trans_time;

		//Manipulator * _manipulator;///��������

		//RemoteFirstPersonManipulator* _rfpm;

		//2014_7_7
		ManipulatorInterface* _mi;
		InteractionType _it;

		TouchEventBuffer* _touchBuffer;///touchBuffer
		Camera _initial_camera;//��ʼ���ӵ�λ��

		Perspective _perspective, _pred_perspective;//͸��ͶӰ
		osg::Matrixf _proj_matrix, _pred_proj_matrix;
		osg::Matrixf   _predViewMatrix;
		Resolution _resolution, _pred_resolution,_upsResolution;//�ӿ�
		Compressor* _compressor;

		// ��ͼ����
		// osg::Matrixd _view_matrix, _pred_view_matrix;

		// ��ɫ����Ⱥͱ�ͼ��
		osg::ref_ptr<osg::Image> _colorImage;
		osg::ref_ptr<osg::Image> _edgeImage;
		osg::ref_ptr<osg::Texture2D> _depthTex;
		osg::Timer_t _lastPredictionTime;
		osg::Timer_t _baseTime;
		// ��ɫ����Ⱥͱ����
		osg::ref_ptr<osg::Camera> _colorCam;
		osg::ref_ptr<osg::Camera> _depthCam;
		osg::ref_ptr<osg::Camera> _edgeCam;

		// ����ͼ���ؽڵ㣬����������������Ԥ��ʱ������ʱ�򿪿��أ���Ⱦ�ο�֡ͼ������ͼ
		// Ȼ����б߼��
		osg::ref_ptr<osg::Switch>		toggle;
		
		string DATAPATH;
		bool ishaveCam;
		double _predictionInterval;
		double _averageFrameTime;
		double _isDataProcessed;
		static double _interval;
		//bool _isPerspective;
		bool _isInitial;
		bool _isAdd;
		bool _isfirstRequest;
		bool _isNiceconnect;
		bool _isRunning;
		bool _isDied;
		ViewpointMgrTask* _vmt;
		MessageQueue* mq;//2014_8_4

};


#endif