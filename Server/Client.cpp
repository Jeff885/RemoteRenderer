#include "Client.h"
#include "Global.h"
#include "Compressor.h"
extern osg::ref_ptr<RemoteViewer> _remoteviewer;
//double Client::_interval=600000; //清理时间
double Client::_interval=6000; 
Client::Client(ViewpointMgrTask* vmt):
_pred_period(200.0),
_frame_time(0.0),
_trans_time(100.0),
ishaveCam(false),
 _predictionInterval(200.0),
_averageFrameTime(0.0),
_isDataProcessed(false),
_isInitial(false),
_isAdd(false),
_isfirstRequest(true),
_isNiceconnect(true),
_isRunning(true),
_vmt(vmt),
_isDied(false)
{
	DATAPATH="E:\\WorkStation\\OpenSceneGraph-Data-3.0.0\\";
	_resolution._width=600;
	_resolution._height=800;
	_compressor=NULL;
	_mi=NULL;
	_lastPredictionTime=osg::Timer::instance()->tick();
	_baseTime=osg::Timer::instance()->tick();
	_touchBuffer=new TouchEventBuffer;
	
	
}
/*Client::Client()
{

}*/
bool Client::isNiceConnect()
{
	return _isNiceconnect;
}
void Client::setPredictionToggle(bool tag)
{
	for(unsigned int i=0;i<toggle->getNumChildren();i++)
		toggle->setValue(i,tag);
}
bool Client::isTimeForPrediction()
{

	osg::Timer_t currentTime = osg::Timer::instance()->tick();
	double deltaTime = osg::Timer::instance()->delta_m(_lastPredictionTime,currentTime);
	double clearTime = osg::Timer::instance()->delta_m(_baseTime,currentTime);
	/*
	Touch_Event
	*/
	_touchBuffer->swapBuffer();
	int storecnt=_touchBuffer->getStoredCount();
	//int width=_resolution._width,height=_resolution._height;
	//int width=1080,height=1920;

	int width = 768, height = 1024;
	for(int i=0;i<storecnt;i++)
	{
		
		TouchEvent te=_touchBuffer->getMotionData(i);
		te._x1=(2*te._x1-width)/width;
		te._y1=(height-2*te._y1)/height;
		te._x2=(2*te._x2-width)/width;
		te._y2=(height-2*te._y2)/height;
		/*
		cout<<"~~~~~~~~~~~~"<<endl;
		cout<<"X1: "<<te._x1<<"Y1: "<<te._y1<<endl;
		cout<<"X2: "<<te._x2<<"Y2: "<<te._y2<<endl;
		cout<<"~~~~~~~~~~~~"<<endl;
		*/
/*		//12_9
#ifdef RFPM
		_rfpm->handle(te._x1,te._y1);

#else
		_manipulator->update(te);

#endif*/
		_mi->handleUpdate(te);

		
	}
	///////////////////////////////////

	

	if((deltaTime > _predictionInterval))
	{

		_lastPredictionTime=currentTime;

		//if(_isfirstRequest)
		if(!_isInitial)
		{
			cout<<"_isInitial Time For Prediction is    "<<false<<endl;
			return false;
		}
		if(_isfirstRequest)
		{
			_isfirstRequest=false;
			return true;
		}
		if(_averageFrameTime >_predictionInterval)
		{
			cout<<"_averageFrameTime >_predictionInterval Time For Prediction is   "<<false<<endl;
			return false;
		}
		if(_isDataProcessed)
		{
			//cout<<"_isDataProcessed Time For Prediction is    "<<false<<endl;
			return false;
		}
		/*
#ifdef RFPM

		if(!_rfpm->isViewpointChanged())
		{

			return false;

		}
#else
		//12_9
		if(!_manipulator->isViewpointChanged())
		{

			//cout<<"isViewpointChanged Time For Prediction is     "<<false<<endl;
			return false;
		}
#endif*/
		if(clearTime > _interval)
		{
			cout<<"interval 时间到"<<endl;
			_baseTime=currentTime;
			return true;
		}else
		{
			
			if(!_mi->isViewpointChanged())
			{
				return false;
			}

		}
		//cout<<"Time For Prediction is  "<<true<<endl;
		return true;
	}
	return false;
}
void Client::UpdatePreViewMatrix()
{
	if(_isInitial)
	{
		if(_isfirstRequest)
		{


			//12_9
/*			
#ifdef RFPM
			_predViewMatrix=_rfpm->getInverseMatrix();
#else
			_predViewMatrix = _manipulator->getInverseViewMatrix();
#endif
*/
		    _predViewMatrix	=_mi->getInverseViewMatrix();
			_colorCam->setViewMatrix(_predViewMatrix);
			_depthCam->setViewMatrix(_predViewMatrix);


			_isfirstRequest=false;
			
		}
		else
		{
			osg::Vec3f eye,center,up;
/*			//12_9
#ifdef RFPM
			_predViewMatrix=_rfpm->getPredInverseMatrix();
#else
		
			_predViewMatrix=_manipulator->getPredInverseViewMatrix(_pred_period+_trans_time+_frame_time);
			
			
#endif*/

			_predViewMatrix=_mi->getPredInverseMatrix(_pred_period+_trans_time+_frame_time);
			

			//_predViewMatrix.getLookAt(eye,center,up);

			//cout<<"ViewMatrix:"<<eye.x()<<"	"<<eye.y()<<"	"<<eye.z()<<endl;
			//cout<<"ViewMatrix:"<<center.x()<<"	"<<center.y()<<"	"<<center.z()<<endl;
			//cout<<"ViewMatrix:"<<up.x()<<"	"<<up.y()<<"	"<<up.z()<<endl;

			_colorCam->setViewMatrix(_predViewMatrix);
			_depthCam->setViewMatrix(_predViewMatrix);
			//_manipulator->
			//cout<<"Update viewMatrix"<<endl;

/*			//12_9
#ifndef RFPM
			_manipulator->resetPrediction();
#endif
*/ 
			_mi->resetPrediction();

		}

		//_remoteviewer->getCamera()->setViewMatrix(_predViewMatrix);//2014_7_24
		
	}

}
void Client::addClientsToggleToMainCamera(osg::ref_ptr<osg::Group> SceneGroup)
{
	if(toggle!=NULL && !_isAdd)
	{
		SceneGroup->addChild(toggle.get());
		_isAdd=true;
	}

}
string Client::id2stirng()
{
	//char ch=_client_id+'0';
	//string str(&ch);

	//return str;
	unsigned int id=_client_id;
	string r;
	if(id ==0)
	{
		r.push_back('0');
		return r;

	}
	while(id!=0)
	{
		unsigned int remainder=id%10;
		id=id/10;
		r.push_back('0'+remainder);
	}
	std::reverse(r.begin(),r.end());
	return r;
}
osg::ref_ptr<osg::Camera> Client::getColorCamera()
{
	return _colorCam;
}
void Client::setLightBoundingBox(osg::Node* node)
{
	if(node!=NULL)
	{
		osg::BoundingBox bb;
		osg::BoundingSphere bs=node->getBound();
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
			ls->setStateSetModes(*_colorCam->getOrCreateStateSet(),osg::StateAttribute::ON);
		}

	}
	
}
void Client::createPredictionCamera(osg::Node* node)
{
	toggle=new osg::Switch;
	_colorImage=new osg::Image;
	_colorImage->allocateImage(_upsResolution._width,_upsResolution._height,1,GL_RGB,GL_UNSIGNED_BYTE);
	_colorImage->setInternalTextureFormat(GL_RGB);

	_colorCam=new osg::Camera;
	_colorCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_colorCam->setViewport(0, 0, _upsResolution._width, _upsResolution._height);

	//_colorCam->setClearColor(osg::Vec4(0.2f, 0.2f, 0.4f, 1.0f));

	_colorCam->setClearColor(osg::Vec4(0.153f, 0.659f, 0.843f, 1.0f));
	_colorCam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_colorCam->setRenderOrder(osg::Camera::PRE_RENDER,3);
	_colorCam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	
	//_colorCam->setViewMatrix(this->getCameraManipulator()->getInverseMatrix());
	_colorCam->setProjectionMatrix(_pred_proj_matrix);
	_colorCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_colorCam->attach(osg::Camera::COLOR_BUFFER, _colorImage.get());
	_colorCam->setPostDrawCallback(new PredictionCallback);
	_colorCam->addDescription(id2stirng());
	//cout<<"!!!!addDescription:  "<<id2stirng()<<endl;
	_colorCam->addChild(node);
	toggle->addChild(_colorCam.get());

	_depthTex =new osg::Texture2D;
	_depthTex->setTextureSize(_pred_resolution._width,_pred_resolution._height);
	_depthTex->setInternalFormat(GL_LUMINANCE32F_ARB);
	_depthTex->setSourceFormat(GL_LUMINANCE);
	_depthTex->setSourceType(GL_FLOAT);
	_depthTex->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
	_depthTex->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
	_depthTex->setResizeNonPowerOfTwoHint(false);


	_depthCam=new osg::Camera;
	_depthCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_depthCam->setViewport(0,0,_pred_resolution._width,_pred_resolution._height);
	_depthCam->setClearColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	_depthCam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_depthCam->setRenderOrder(osg::Camera::PRE_RENDER,1);
	_depthCam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	_depthCam->setProjectionMatrix(_pred_proj_matrix);
	_depthCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_depthCam->attach(osg::Camera::COLOR_BUFFER,_depthTex.get());
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
	osg::ref_ptr<osg::Uniform> texSize = new osg::Uniform("texSize", osg::Vec2(_pred_resolution._width,_pred_resolution._height));
	osg::ref_ptr<osg::Uniform> isShow = new osg::Uniform("isShow", false);
	quad->getOrCreateStateSet()->addUniform(texDepth.get());
	quad->getOrCreateStateSet()->addUniform(texSize.get());
	quad->getOrCreateStateSet()->addUniform(isShow.get());

	// 创建深度边图像
	_edgeImage = new osg::Image;
	_edgeImage->allocateImage(
		_pred_resolution._width,
		_pred_resolution._height,
		1,
		GL_LUMINANCE_ALPHA,
		GL_FLOAT
		);
	_edgeImage->setInternalTextureFormat(GL_LUMINANCE_ALPHA32F_ARB);

	// 创建深度边相机，添加到switch节点
	_edgeCam = new osg::Camera;
	_edgeCam->setViewport(0, 0,_pred_resolution._width,_pred_resolution._height);
	_edgeCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_edgeCam->setProjectionMatrixAsOrtho2D(-1.0, 1.0, -1.0, 0.0);
	_edgeCam->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	_edgeCam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_edgeCam->setRenderOrder(osg::Camera::PRE_RENDER, 2);
	_edgeCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_edgeCam->attach(osg::Camera::COLOR_BUFFER, _edgeImage.get());
	_edgeCam->addChild(quad.get());
	toggle->addChild(_edgeCam.get(),true);
	ishaveCam=true;
}

osg::Program* Client::createProgram(const std::string vert, const std::string frag)
{
	osg::ref_ptr<osg::Program> program = new osg::Program;
	osg::ref_ptr<osg::Shader> vertShader = osgDB::readShaderFile(vert);
	osg::ref_ptr<osg::Shader> fragShader = osgDB::readShaderFile(frag);
	program->addShader(vertShader.get());
	program->addShader(fragShader.get());
	return program.release();
}
void Client::sentOnePacket( 
	DataType dataType, 
	unsigned int cLen, 
	unsigned char* cBuf, 
	unsigned int dsLen, 
	unsigned int ddLen,
	char* dBuf,
	osg::Matrixf vp)
{
	unsigned char tmpBuffer[100];
	// 写入类型和三个长度
	tmpBuffer[0] = 0;
	tmpBuffer[0] |= dataType;
	putInt(&tmpBuffer[1], cLen);
	putInt(&tmpBuffer[5], dsLen);
	putInt(&tmpBuffer[9], ddLen);

	// 写入变换矩阵的16个元素
	int i,j;
	unsigned int* mtData = reinterpret_cast<unsigned int*>(vp.ptr());
	for(i = 0; i < 4; ++i)
		for(j = 0; j < 4; ++j) 
			putInt(&tmpBuffer[13 + i*16 + 4*j], mtData[4*i + j]);
	try
	{
		// 发送数据包和颜色、深度缓冲区内容
		write(*_socket, boost::asio::buffer(tmpBuffer, 77));
		write(*_socket,  boost::asio::buffer(cBuf, cLen));
		write(*_socket,  boost::asio::buffer(dBuf, ddLen));
	}
	catch(std::exception& e)
	{
		cout << "Packet  writing failed: "<< e.what() <<"\n";

		_isNiceconnect=false;

	}
	cout << ">>>>>>>>  compressed data has been transmitted~. "
		<< "cLen: " << cLen  << " dsLen: " << dsLen << " ddLen: " << ddLen << " <<<<<<<<\n";
}

// 将整形值从高到低4个字节放入字符缓冲区中
void Client::putInt(unsigned char* data, const unsigned int& val)
{
	unsigned int tmp;
	int i;
	for(i = 3; i >=0; --i)
	{
		tmp = (val >> (i*8)) & 0xff;
		data[3 -i] = 0;
		data[3 - i] |= tmp;
	}
}
osg::Matrixf Client::getPredInvMVP()
{
	
	osg::Matrixf tmp =  _predViewMatrix* _pred_proj_matrix;
	tmp = tmp * _depthCam->getViewport()->computeWindowMatrix();
	return  osg::Matrixf::inverse(tmp);
}
void Client::setUpCompressor(Resolution& r)
{
	if(_compressor==NULL)
	{
		_compressor=new Compressor(r._width,r._height);
	}

		
}
void Client::setMessageQueue(MessageQueue* mq)
{
	this->mq=mq;
}
Client::~Client()
{
	cout<<"~客户端析构"<<endl;
	_remoteviewer->sceneGroup->removeChild(toggle.get());
	if(_mi!=NULL)
	{
		delete _mi;
		_mi=NULL;
	}
	if(_compressor!=NULL)
	{
		delete _compressor;
		_compressor=NULL;
	}

	if(_touchBuffer!=NULL)
	{
		delete _touchBuffer;
		_touchBuffer=NULL;
	}
	
}