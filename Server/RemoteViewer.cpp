#include "RemoteViewer.h"
#include "PredictionCallback.h"
#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>
#include "SceneMgr.h"
#include "PreDrawCallback.h"
std::string RemoteViewer::DATAPATH ="E:\\WorkStation\\OpenSceneGraph-Data-3.0.0\\";
extern ClientTable* _gClientTable;
//osg::Node* node=osgDB::readNodeFile("D:\\sceneData\\paris_no_tree.osg");
//osg::Node* node=osgDB::readNodeFile("D:\\sceneData\\engine2.osg");
//osg::Node* node=osgDB::readNodeFile("D:\\sceneData\\cessna.osg");
//osg::Node* node=osgDB::readNodeFile("D:\\sceneData\\armadillo.ply");
RemoteViewer::RemoteViewer():
_isDataProcessed(false),
_averageFrameTime(0.0),
_predictionInterval(200.0),
_compressor(NULL),
_clientTable(_gClientTable),	
mode_node(NULL)
{
	this->setLightingMode(osg::View::LightingMode::NO_LIGHT);
}
osg::Camera* RemoteViewer::createMasterCamera()
{

	osg::ref_ptr<osg::GraphicsContext::Traits> traits=new osg::GraphicsContext::Traits;
	traits->x=100;
	traits->y=100;
	traits->width=500;
	traits->height=500;
	traits->windowDecoration=true;
	traits->doubleBuffer =true;
	traits->sharedContext=0;
	traits->supportsResize=false;
	traits->alpha=8;

	osg::ref_ptr<osg::GraphicsContext> gc =osg::GraphicsContext::createGraphicsContext(traits.get());
	osg::ref_ptr<osg::Camera> cam=new osg::Camera;

	cam->setGraphicsContext(gc.get());
	cam->setViewport(0,0,traits->width,traits->height);
	cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam->setClearColor(osg::Vec4(0.2f,0.2f,0.4f,1.0f));
	cam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	//cam->setPreDrawCallback(new PreDrawCallBack);
	
	//cam->setViewMatrixAsLookAt(osg::Vec3f(-223.832,-814.06,11.2053),osg::Vec3f(-223.733,-813.074,11.0723),osg::Vec3f(0.0132886,0.132357,0.991113));
	//cam->setViewMatrixAsLookAt(osg::Vec3( -66.3779,-6565.93,32.1885),osg::Vec3(-66.4034,-6564.93,32.1785),osg::Vec3( -0.000254609,0.00997634,0.99995));
	cam->setProjectionMatrixAsPerspective(29.1484,traits->width/traits->height,10,500000);
	
	GLenum buffer=gc->getTraits()->doubleBuffer ? GL_BACK :GL_FRONT;
	cam->setDrawBuffer(buffer);
	cam->setReadBuffer(buffer);
	return cam.release();
}
void RemoteViewer::initRemoteViewer()
{

	osg::ref_ptr<osg::Camera> ca=createMasterCamera();

	this->setCamera(ca.get());
}
void RemoteViewer::setMasterCameraLight()
{
	if(mode_node!=NULL)
	{
		osg::BoundingSphere bs=mode_node->getBound();
		osg::BoundingBox bb;
		bb.expandBy(bs);
		for(int i=7;i>=0;i--)
		{
			osg::ref_ptr<osg::LightSource> ls=new osg::LightSource;
			osg::ref_ptr<osg::Light> lt=new osg::Light;
			lt->setLightNum(i);
			lt->setPosition(osg::Vec4(bb.corner(i),1.0));
			lt->setAmbient(osg::Vec4(0.5,0.5,0.5,1.0));
			lt->setDiffuse(osg::Vec4(1.0,1.0,1.0,1.0));
			ls->setLight(lt);
			ls->setStateSetModes(*this->getCamera()->getOrCreateStateSet(),osg::StateAttribute::ON);
		}

	}
	
}
void RemoteViewer::start(RemoteViewer* viewer)
{
	viewer->initRemoteViewer();
	viewer->run();
}

osg::Program* RemoteViewer::createProgram(const std::string vert, const std::string frag)
{
	osg::ref_ptr<osg::Program> program = new osg::Program;
	osg::ref_ptr<osg::Shader> vertShader = osgDB::readShaderFile(vert);
	osg::ref_ptr<osg::Shader> fragShader = osgDB::readShaderFile(frag);
	program->addShader(vertShader.get());
	program->addShader(fragShader.get());
	return program.release();
}
osg::Switch* RemoteViewer::createPredictionCamera(osg::Node* node)
{
	osg::ref_ptr<osg::Switch> toggle=new osg::Switch;
	_colorImage=new osg::Image;
	_colorImage->allocateImage(500,500,1,GL_RGB,GL_UNSIGNED_BYTE);
	_colorImage->setInternalTextureFormat(GL_RGB);

	_colorCam=new osg::Camera;
	_colorCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	
	_colorCam->setClearColor(osg::Vec4(0.2f, 0.2f, 0.4f, 1.0f));
	
	_colorCam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_colorCam->setRenderOrder(osg::Camera::PRE_RENDER,3);
	_colorCam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	_colorCam->setViewMatrix(this->getCameraManipulator()->getInverseMatrix());
	_colorCam->setProjectionMatrix(this->getCamera()->getProjectionMatrix());
	_colorCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_colorCam->attach(osg::Camera::COLOR_BUFFER, _colorImage.get());
	_colorCam->setPostDrawCallback(new PredictionCallback);
	_colorCam->addDescription("local");
	_colorCam->addChild(node);
	toggle->addChild(_colorCam.get());

	_depthTex =new osg::Texture2D;
	_depthTex->setTextureSize(500,500);
	_depthTex->setInternalFormat(GL_LUMINANCE32F_ARB);
	_depthTex->setSourceFormat(GL_LUMINANCE);
	_depthTex->setSourceType(GL_FLOAT);
	_depthTex->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
	_depthTex->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
	_depthTex->setResizeNonPowerOfTwoHint(false);


	_depthCam=new osg::Camera;
	_depthCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_depthCam->setViewport(0,0,500,500);
	_depthCam->setClearColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	_depthCam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_depthCam->setRenderOrder(osg::Camera::PRE_RENDER,1);
	_depthCam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	_depthCam->setViewMatrix(this->getCameraManipulator()->getInverseMatrix());
	_depthCam->setProjectionMatrix(this->getCamera()->getProjectionMatrix());
	_depthCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_depthCam->attach(osg::Camera::COLOR_BUFFER,_depthTex.get());///////////////////////////?????////////////////
	_depthCam->getOrCreateStateSet()->setAttributeAndModes(
		createProgram(DATAPATH + "MyShaders\\depth.vert", 
		DATAPATH + "MyShaders\\depth.frag")
		);
	_depthCam->addChild(node);
	toggle->addChild(_depthCam.get(),true);





	osg::ref_ptr<ScreenQuad> quad = new ScreenQuad;
	quad->getOrCreateStateSet()->setTextureAttributeAndModes(0, _depthTex.get());
	quad->getOrCreateStateSet()->setAttributeAndModes(
		createProgram(DATAPATH + "MyShaders\\detect.vert", 
		DATAPATH + "MyShaders\\detect.frag")
		);
	osg::ref_ptr<osg::Uniform> texDepth = new osg::Uniform("texDepth", 0);
	osg::ref_ptr<osg::Uniform> texSize = new osg::Uniform("texSize", osg::Vec2(500.0, 500.0));
	osg::ref_ptr<osg::Uniform> isShow = new osg::Uniform("isShow", true);
	quad->getOrCreateStateSet()->addUniform(texDepth.get());
	quad->getOrCreateStateSet()->addUniform(texSize.get());
	quad->getOrCreateStateSet()->addUniform(isShow.get());

	// 创建深度边图像
	_edgeImage = new osg::Image;
	_edgeImage->allocateImage(
		500,
		500,
		1,
		GL_LUMINANCE_ALPHA,
		GL_FLOAT
		);
	_edgeImage->setInternalTextureFormat(GL_LUMINANCE_ALPHA32F_ARB);

	// 创建深度边相机，添加到switch节点
	_edgeCam = new osg::Camera;
	_edgeCam->setViewport(0, 0,500,500);
	_edgeCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_edgeCam->setProjectionMatrixAsOrtho2D(-1.0, 1.0, -1.0, 0.0);
	_edgeCam->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	_edgeCam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_edgeCam->setRenderOrder(osg::Camera::PRE_RENDER, 2);
	_edgeCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_edgeCam->attach(osg::Camera::COLOR_BUFFER, _edgeImage.get());
	_edgeCam->addChild(quad.get());
	toggle->addChild(_edgeCam.get(),true);



	/*
	osg::ref_ptr<osg::GraphicsContext::Traits> traits=new osg::GraphicsContext::Traits;
	traits->x = 200;
	traits->y = 200;
	traits->width = 500;
	traits->height = 500;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->alpha=8;
	traits->supportsResize = false;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	osg::ref_ptr<osg::Texture2D> showdepthTex=new osg::Texture2D;
	showdepthTex->setImage(_edgeImage.get());
	showdepthTex->setInternalFormat(GL_LUMINANCE_ALPHA32F_ARB);
	showdepthTex->setResizeNonPowerOfTwoHint(false);

	osg::ref_ptr<ScreenQuad> quad1=new ScreenQuad;
	quad1->getOrCreateStateSet()->setTextureAttributeAndModes(0,showdepthTex.get());



	osg::ref_ptr<osg::Camera> edgeCamera=new osg::Camera;
	edgeCamera->setGraphicsContext(gc.get());
	edgeCamera->setViewport(0,0,500,500);
	edgeCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	edgeCamera->setClearColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	edgeCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);


	GLenum buffer =gc->getTraits()->doubleBuffer ? GL_BACK: GL_FRONT;
	edgeCamera->setDrawBuffer(buffer);
	edgeCamera->setReadBuffer(buffer);
	
	
	edgeCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	edgeCamera->setProjectionMatrixAsOrtho2D(-1.0, 1.0, -1.0, 1.0);
	edgeCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	edgeCamera->setRenderOrder(osg::Camera::POST_RENDER, 10);
	edgeCamera->addChild(quad1.get());
	

	this->addSlave(edgeCamera.get());*/
	//toggle->addChild(edgeCamera.get(),true);
	//
	//
	/*
	_depthImage=new osg::Image;
	_depthImage->allocateImage(500,500,1,GL_LUMINANCE_ALPHA,GL_FLOAT);
	_depthImage->setInternalTextureFormat(GL_LUMINANCE_ALPHA32F_ARB);
	_depthCam=new osg::Camera;
	_depthCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_depthCam->setViewport(0, 0,500,500);
	_depthCam->setClearColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_depthCam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_depthCam->setRenderOrder(osg::Camera::PRE_RENDER, 2);
	_depthCam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	//_depthCam->setProjectionMatrix(_predProjMatrix);
	_depthCam->setProjectionMatrix(this->getCamera()->getProjectionMatrix());
	_depthCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_depthCam->attach(osg::Camera::DEPTH_BUFFER, _depthImage.get());

	_depthCam->addChild(node);
	toggle->addChild(_depthCam.get(),true);*/
	
	return toggle.release();
}


osg::Node* RemoteViewer::scenePrepare()
{
	sceneGroup=new osg::Group;
	mode_node =loadModel();
	sceneGroup->addChild(mode_node.get());

	_predictionToggle = createPredictionCamera(mode_node.get());
	_predictionToggle->setValue(0,true);
	sceneGroup->addChild(_predictionToggle.get());

	return sceneGroup.get();
}
//未声明
//创建客户端的Switch节点，每一个客户端维护一个switch节点

void RemoteViewer::setUpClientNode()
{
	
}
osg::Node* RemoteViewer::loadModel()
{
	//osg::ref_ptr<osg::Node> node=Scene_Mgr::instance()->getSceneByName("armadillo");
	//osg::ref_ptr<osg::Node> node=Scene_Mgr::instance()->getSceneByName("armadillo");
	osg::ref_ptr<osg::Node> node=Scene_Mgr::instance()->getSceneByName("engine");
	return node;
}
int RemoteViewer::run()
{

	_predictionInterval = 1000.0 / 5;
	if(!_compressor) 
		_compressor = new Compressor(500,500);
	this->setCameraManipulator(new osgGA::TrackballManipulator);
	this->setSceneData(scenePrepare());
	//this->getCameraManipulator()->setAutoComputeHomePosition(true);
	//this->getCameraManipulator()->computeHomePosition(this->getCamera(),true);
	this->setUpViewInWindow(20,20,1000,1000);
	this->setMasterCameraLight();
	/*
	if(!_isInit)
	{
		
		_isInit =true;
	}
	cout<<"run"<<endl;
	initRemoteViewer();
	this->setSceneData(scenePrepare());
	//initRemoteViewer();
	setReleaseContextAtEndOfFrameHint(false);*/
	setReleaseContextAtEndOfFrameHint(false);
	return ViewerBase::run();
}
void RemoteViewer::TimeForAllClientsPrediction()
{

	_clientTable->setTimeForPredictionToggleToTrue();
	


}
bool RemoteViewer::isTimeForPrediction()
{
	osg::Timer_t  currentTime = osg::Timer::instance()->tick();
	double deltaTime = osg::Timer::instance()->delta_m(_lastPredictionTime, currentTime);
	if( (deltaTime > _predictionInterval))
	{
		_lastPredictionTime = currentTime;

		//if(_averageFrameTime > _predictionInterval) 
		//{
			//VinoGlobal::vinoCout("Remote engine is too slow, prediction failed~");
			//return false;
		//}
		//if(_isDataProcessed)
		//{
			//VinoGlobal::vinoCout("3D image data is still processing , prediction failed~");
		//	return false;
		//}
		//if(!_manipulator->isViewportChanged())  
		//{
			//VinoGlobal::vinoCout("Client view point had not  been changed, prediction failed~");
		//	//return false;
		////}
		return true;
	}
	return false;


}
void RemoteViewer::setPredictionToggle(bool tag)
{

	unsigned int i;
	for(i =0; i < _predictionToggle->getNumChildren(); ++i)
		_predictionToggle->setValue(i, tag);

	/*
	设置客户端camera Switch节点
	*/

	//_clientTable->setPredictionToggle(tag);

}
void RemoteViewer::addClientsCamtoMainCamera()
{
	//_clientTable->createClientCam(mode_node.get());
	//cout<<"sceneGroup 孩子数目："<<sceneGroup->getNumChildren()<<endl;
	_clientTable->addClientsToggleToMainCamera(sceneGroup);
	
	
}
void RemoteViewer::updataRemoteCamera()
{
	/*
	更新RemoteCamera
	*/
	//cout<<"^^^^^^^^^^^^^^^^^^^"<<_clientTable->ClientSize()<<"^^^^^^^^^^^^^^^^^^"<<endl;
	if(isTimeForPrediction())
	{
		//cout<<this->getCamera()->getProjectionMatrix().
		//cout<<this->getCamera().getProjectionMatrix().getPerspective();
		setPredictionToggle(true);


	}
}
void RemoteViewer::setClientsToggle(bool tag)
{
	_clientTable->setPredictionToggle(tag);
}
void RemoteViewer::updateClientsCamera()
{
	_clientTable->updateClientscamera();

}
void RemoteViewer::frame(double simulationTime)
{
	/*
	统计时间
	*/
	/*
	static int cnt =0;
	static osg::Timer_t pre=osg::Timer::instance()->tick();
	++cnt;
	if(cnt == 100)
	{
		osg::Timer_t tt=osg::Timer::instance()->tick();
		double tmp=osg::Timer::instance()->delta_m(pre,tt);
		pre =tt;
		cout<<"绘制时间"<<tmp/100<<"\n";
		cnt=0;
	}*/
	//if(_isDone) _done =true;

	if(_done) return;

	if(_firstFrame)
	{

		viewerInit();
		if(!isRealized())
		{

			realize();
		}
		_firstFrame=false;
		//_lastPredictionTime =osg::Timer::instance()->tick();
	}
	advance(simulationTime);

	
	//setPredictionToggle(false);
	//updateRemoteCamera();
	/*
	_colorCam->setViewMatrix(this->getCameraManipulator()->getInverseMatrix());
	_depthCam->setViewMatrix(this->getCameraManipulator()->getInverseMatrix());/*
	*/
	//_edgeCam->setViewMatrix(this->getCameraManipulator()->getInverseMatrix());
	//cout<<"旋转X:"<<this->getCameraManipulator()->getInverseMatrix().getRotate().x()<<endl;
	//double fovy,asp,tnear,tfar;
	//this->getCameraManipulator()->get
	
	/*
	_colorCam->setProjectionMatrix(this->getCamera()->getProjectionMatrix());
	_depthCam->setProjectionMatrix(this->getCamera()->getProjectionMatrix());
	*/
	//_colorCam->setViewMatrix();
	//_depthCam->setView();
	/*
	osg::Vec3f eye;
	osg::Vec3f center;
	osg::Vec3f up;
	float distance;
	double fovy;
	double asp;
	double tnear;
	double tfar;
	
	this->getCamera()->getViewMatrixAsLookAt(eye,center,up);
	
	cout<<"Eye: "<<eye.x()<<" "<<eye.y()<<" "<<eye.z()<<endl;
	cout<<"Center: "<<center.x()<<" "<<center.y()<<" "<<center.z()<<endl;
	cout<<"Up: "<<up.x()<<" "<<up.y()<<" "<<up.z()<<endl;

	//cout<<"fovy:"<<fovy<<"    asp:"<<asp<<"   tnear:"<<tnear<<"   tfar:"<<tfar<<endl;
	this->getCamera()->getProjectionMatrix().getPerspective(fovy,asp,tnear,tfar);
	cout<<"fovy:"<<fovy<<"    asp:"<<asp<<"   tnear:"<<tnear<<"   tfar:"<<tfar<<endl;*/
	
	addClientsCamtoMainCamera();//
	//createClientsCam();//远程
	setClientsToggle(false);// 
	//updateClientsCamera();//
	TimeForAllClientsPrediction();//


	
	//setClientsPrediction(false);
	//setPredictionToggle(false);//本地
	//updataRemoteCamera();//本地

	
	eventTraversal();
	updateTraversal();
	renderingTraversals();
	
	//osgViewer::frame(simulationTime);
}