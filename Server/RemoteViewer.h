#ifndef REMOTE_VIEWER
#define REMOTE_VIEWER

#include "Global.h"
#include <osgViewer/Viewer>
#include "Compressor.h"
#include "ScreenQuad.h"
#include "ClientTable.h"
#include <string>
class ClientTable;
class RemoteViewer : public osgViewer::Viewer
{	
public:
	RemoteViewer();
	osg::Camera* createMasterCamera();
	osg::Node* scenePrepare();
	static void start(RemoteViewer* viewer);
	virtual int run();
	osg::Node* loadModel();//����ģ��
	osg::Switch* createPredictionCamera(osg::Node* node);
	osg::Program* createProgram(const std::string vert, const std::string frag);
	void updataRemoteCamera();
	bool isTimeForPrediction();
	void setPredictionToggle(bool tag);
	void setUpClientNode();
	void setMasterCameraLight();
	void addClientsCamtoMainCamera();//Զ�̽ӿ�
	void  TimeForAllClientsPrediction();//Զ�̽ӿ�
	void setClientsToggle(bool tag);//Զ�̽ӿ�
	void updateClientsCamera();//Զ�̽ӿ�

	osg::ref_ptr<osg::Image> _colorImage;
	osg::ref_ptr<osg::Image> _depthImage;
	osg::ref_ptr<osg::Image> _edgeImage;
	osg::ref_ptr<osg::Texture2D> _depthTex;//���ͼ��
	osg::ref_ptr<osg::Switch> _predictionToggle;
	osg::ref_ptr<osg::Camera> _colorCam;//��ɫͼ��
	static std::string DATAPATH;
	Compressor* _compressor;
	bool _isDataProcessed;
	ClientTable*	_clientTable;
	osg::ref_ptr<osg::Group> sceneGroup;
private:
	virtual void frame(double simulationTime = USE_REFERENCE_TIME);
	virtual void initRemoteViewer();
	bool _isInit;
	osg::ref_ptr<osg::Node> mode_node;
	osg::ref_ptr<osg::Camera> _depthCam;
	osg::ref_ptr<osg::Camera> _edgeCam;
	osg::Timer_t _lastPredictionTime;
	
	double  _predictionInterval;
	double  _averageFrameTime;
	double  _dataTransmisionDelay;
	//friend PreDrawCallBack;
};
//extern osg::ref_ptr<osg::Node> node;
#endif