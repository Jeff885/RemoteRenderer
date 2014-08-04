#include "ServerTask.h"
#include "SceneMgr.h"
#include "RemoteFirstPersonManipulator.h"
#include "AppEngineClient.h"
#include "AppEngineOperatorMessage.h"
#include "InterfaceMessage.h"
#include "CameraOperator.h"
//#include "RemoteViewer.h"

//extern osg::Node* node;
extern ClientTable* _gClientTable;
ViewpointMgrTask::ViewpointMgrTask(){
	_isRunning=false;
	_queue = new MessageQueue();
	_client=NULL;
	_runSwitch=true;

}
Client* ViewpointMgrTask::getClient()
{
	return _client;
}
MessageQueue* ViewpointMgrTask::getMessageQueue()
{
	return _queue;
}
 void  ViewpointMgrTask::start(ServerPipeline* pl)
 {
	 if(!_isRunning)
	 {
		 _isRunning=true;
		 _pipeline=pl;
		 cout<<"开启视点管理任务"<<endl;
		 _thread= new boost::thread((boost::bind(&ViewpointMgrTask::run,this)));
		 _thread->detach();
		 
	 }

 }
  void ViewpointMgrTask::run() {
	 //2014_8_4
	 
	 Camera cam;//临时数据
	 while(_runSwitch){
		 //cout<<".........."<<endl;
		 // 出队一个消息
		 Message *msg;
		 _queue->wait_and_pop(msg);

		 // 查询客户端
		 //ClientTable *client_table =new Client(1);
		 //RemoteRenderer::Client *client = new RemoteRenderer::client();
		 //assert(client != NULL);
		 //Client* client =new Client();

		 // 处理消息
		 switch(msg->getType()){
		 case CONTROL_START_MSG:
			 {
				 if(_client!=NULL)
					 break;

				 int type= (dynamic_cast<StartMessage*>(msg))->getAppType();
				 // start，执行客户端初始化工作
				 cout<<"执行客户端 "<<type<<" 初始化工作"<<endl;
				 //2014_7_28
				 if(type == DEFAULTVALUE)
				 {
					 _client=new Client(this);
				 }else if(type ==ENGINESHOW)
				 {
					 //_client=new Client(this);
					 _client=new AppEngineClient(this);
					 cout<<typeid(_client).name()<<endl;
					 cout<<typeid(*_client).name()<<endl;
					 cout<<typeid(AppEngineClient).name()<<endl;
				 }
				 
				_client->_socket=_socket;
				 _client->_client_id=_gClientTable->getNewClientID();
				 //initClient(_client);
				 _gClientTable->addClient(_client);
				  _client->setMessageQueue(_queue);
				 delete msg;
				 break;
			 }
		

		 case CONTROL_STOP_MSG:
			 {
				 //stop
				 //client_table->removeClient(client);
				 //delete client;
				 //_pipeline->sendMsgToViewQuery(msg);
				 //cout<<"stop执行析构 "<<endl;
				 _runSwitch=false;
				 if(_client == NULL)
					 break;
				 _client->_isDied=true;//客户端死掉
				 //_gClientTable->removeClient(_client->_client_id);
				 delete msg;
				 break;
			 }
		 case INITIAL_SCENE_MSG:
			 {
				 if(_client == NULL)
					 break;

				 string s= (dynamic_cast<StringMessage*>(msg))->getString();
				 cout<<"场景配置 : "<<s <<"长度： "<<s.length()<<endl;
				  initSceneNode(_client,s);
				  _client->_isInitial=true;
				 delete msg;
				 break;
			 }
		 case INITIAL_CAMERA_MSG:
			 {

				 // camera
				 if(_client == NULL)
					 break;
				  cout<<"INITIAL_CAMERA_MSG"<<endl;
				  CameraMessage *camera_msg = dynamic_cast<CameraMessage*>(msg);
				  camera_msg->getCameraArgs(_client->_initial_camera);
				  //cout<<"INITIAL_CAMERA_MSG: "<<" camera.centerX:  "<<_client->_initial_camera._centerx
					 // <<" camera.eyeX: "<<_client->_initial_camera._eyex<<" camera.upx: "<<_client->_initial_camera._upx<<endl;
				  //initClient(_client);
				
				 
				 // _client->_isInitial=true;
				  //initCamera(_client);
				  //_client->createPredictionCamera();
				  delete camera_msg;
				  break;
			 }


		 case INITIAL_PERSPECTIVE_MSG:
			 {
				 // perspective
				 if(_client ==NULL)
					 break;

				 cout<<"INITIAL_PERSECTIVE_MSG"<<endl;
				 PerspectiveMessage *proj_msg = dynamic_cast<PerspectiveMessage*>(msg);
				 proj_msg->getPerspectiveArgs(_client->_perspective);
				 initPerspective(_client);
				// _client->_isPerspective=true;
				
				 delete msg;
				 break;
			 }
		 case INITIAL_INTERACTION_MSG:
			 {
				 if(_client == NULL)
					 break;
				 cout<<"INITIAL_INTERACTION_MSG"<<endl;
				 InteractionMessage* im=dynamic_cast<InteractionMessage*>(msg);
				 im->getInteractionType(_client->_it);
				 initClient(_client);
				 delete msg;
				 break;

			 }


		 case INITIAL_RESOLUTION_MSG:
			 {

				 // resolution
				 //ResolutionMessage *res_msg = dynamic_cast<ResolutionMessage*>(msg);
				 //res_msg->getResolutionArgs(client->_resolution);
				 //delete msg;
				 //break;

			 }

		 case RUNTIME_TOUCH_EVENT_MSG:
			 {

				 //更新视点
				 // touch event
				 TouchEventMessage *touch_msg = dynamic_cast<TouchEventMessage*>(msg);
				 TouchEvent touch_event;
				 touch_msg->getTouchEventArgs(touch_event);

				 //_client->_manipulator->update(touch_event); 
				 _client->_touchBuffer->putEventData(touch_event);
				 delete msg;
				 break;

			 }
		 case APPLICATIONMSG:
			 {
				 AppEngineOperatorMessage* app_msg=dynamic_cast<AppEngineOperatorMessage*>(msg);
				 cout<<"应用层消息: "<<endl;
				 cout<<app_msg->getSeqId()<<"   "<<app_msg->getOp()<<endl;
				 delete msg;
				 break;
			 }
		 case CAMERACENTER:
			 {
				 CameraCenterMsg* cm=dynamic_cast<CameraCenterMsg* >(msg);
				 cam._centerx=cm->getX();
				 cam._centery=cm->getY();
				 cam._centerz=cm->getZ();
				 delete msg;
				 break;
			 }
		 case CAMERAPOSITION:
			 {
				 CameraPositionMsg* cm=dynamic_cast<CameraPositionMsg* >(msg);
				 cam._eyex=cm->getX();
				 cam._eyey=cm->getY();
				 cam._eyez=cm->getZ();
				 delete msg;
				 break;

			 }
		 case CAMERAUP:
			 {
				 CameraUpMsg* cm=dynamic_cast<CameraUpMsg* >(msg);

				 cam._upx=cm->getX();
				 cam._upy=cm->getY();
				 cam._upz=cm->getZ();

				 if(_client!=NULL && _client->_mi!=NULL && _client->_it._type==USER_DEFINE)
					 _client->_mi->setByLookAt(cam);
				 delete msg;
				 break;
			 }

		 default:
			 // others，忽略
			 break;
		 }
	 }
	 cout<<"循环退出"<<endl;
	 this->~ViewpointMgrTask();
 }
  void ViewpointMgrTask::initClient(Client *client) {
	  cout<<"执行initClient"<<endl;

	  // 初始化投影和预测投影参数
	  //initPerspective(client);

	  // 初始化控制器
	  initManipulator(client);

	  // 初始化场景节点
	  //initSceneNode(client);

	  // 初始化视点预测器
	  //initPredictor(client);
  }
  ViewpointMgrTask::~ViewpointMgrTask() {
			cout<<"~ViewpointMgrTask"<<endl;
			delete _queue;
  }

  void ViewpointMgrTask::initPerspective(Client *client) {
	  // 扩大预测相机的视角和分辨率
	  client->_pred_perspective = client->_perspective;

	  client->_pred_perspective._fov = client->_perspective._fov +
		  2*_pipeline->getMaxRotateRate()/_pipeline->getRefFrameRate();

	  client->_pred_resolution._height = (int)(
		  tan(client->_pred_perspective._fov / 360 * osg::PI)/
		  tan(client->_perspective._fov / 360 * osg::PI)*
		  client->_resolution._height);
	  client->_pred_resolution._width = (int)(client->_perspective._aspect *
		  client->_pred_resolution._height);

	  client->_upsResolution._width=int(client->_pred_resolution._width /1);
	  client->_upsResolution._height=int(client->_pred_resolution._height/1);

	  //JPEG 宽度和高度为偶数

	  if(client->_upsResolution._width & 1) client->_upsResolution._width -= 1;
	  if(client->_upsResolution._height & 1) client->_upsResolution._height -= 1;


	  
	  client->setUpCompressor(client->_pred_resolution);
		

	  cout<<"_upsResolution:width "<<client->_upsResolution._width<<"_upsResolution;height "<<client->_upsResolution._height<<endl;

	  cout<<"_pred_resolution:width "<<client->_pred_resolution._width<<"_upsResolution;height "<<client->_pred_resolution._height<<endl;



	  // 创建投影矩阵
	  client->_proj_matrix.makePerspective(
		  client->_perspective._fov,
		  client->_perspective._aspect,
		  client->_perspective._near,
		  client->_perspective._far);

	  client->_pred_proj_matrix.makePerspective(
		  client->_pred_perspective._fov,
		  client->_pred_perspective._aspect,
		  client->_pred_perspective._near,
		  client->_pred_perspective._far);

  }
  void ViewpointMgrTask::initManipulator(Client *client) {
/*2014_7_7	  
#ifdef RFPM 
	  //12_9
	  client->_rfpm=new RemoteFirstPersonManipulator;
	  client->_rfpm->setByLookAt(client->_initial_camera);
#else
	  client->_manipulator = new Manipulator(client->_initial_camera);
	  client->_manipulator->resetPrediction();

#endif
*/	
	  switch(client->_it._type)
	  {
		  case FIRSTPERSONMANIPULATOR:
		  {
			  client->_mi=new RemoteFirstPersonManipulator(client->_initial_camera);
			  break;

		  }case TRACKBALLMANIPULATOR:
		  {
			  client->_mi=new Manipulator(client->_initial_camera);
			  break;
		  }
		  case USER_DEFINE:
			  {
				   client->_mi=new CameraOperator(client->_initial_camera);
				  break;
			  }

		  default:
			  break;
	  }
	   client->_mi->resetPrediction();

	 
  }

  void ViewpointMgrTask::initSceneNode(Client *client,string sceneName) {

	  //osg::ref_ptr<osg::Node> node=osgDB::readNodeFile("D:\\sceneData\\paris_no_tree.osg");
	  Scene_Mgr* scenemgr=Scene_Mgr::instance();
	  

	  //2014_7_28
	  if(typeid(*client) == typeid(Client))
	  {
		  osg::ref_ptr<osg::Node> node;
		  node=scenemgr->getSceneByName(sceneName);
		  client->createPredictionCamera(node);
		  client->setLightBoundingBox(node);
	  }else if(typeid(*client) == typeid(AppEngineClient))
	  {
		   osg::ref_ptr<osg::Node> node;
		   node=(dynamic_cast<AppEngineClient* >(client))->DeepCopyModelData(scenemgr->getSceneByName(sceneName));
		   client->createPredictionCamera(node);
		   client->setLightBoundingBox(node);
	  }
	  
	  
	  cout<<"initSceneNode"<<endl;
	  client->ishaveCam=true;
  }
  osg::Program* ViewpointMgrTask::loadShader(const string &vert, const string &frag) {
	  osg::ref_ptr<osg::Program> program = new osg::Program();
	  osg::ref_ptr<osg::Shader> vertShader = osgDB::readShaderFile(vert);
	  osg::ref_ptr<osg::Shader> fragShader = osgDB::readShaderFile(frag);
	  program->addShader(vertShader.get());
	  program->addShader(fragShader.get());
	  return program.release();
  }
  osg::Geode* ViewpointMgrTask::createQuad(Client *client) {
	  /*
	  osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	  vertices->push_back(osg::Vec3(-1.0f, -1.0f, 0.0f));
	  vertices->push_back(osg::Vec3(1.0f, -1.0f, 0.0f));
	  vertices->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
	  vertices->push_back(osg::Vec3(-1.0f, 1.0f, 0.0f));

	  osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	  texcoords->push_back(osg::Vec2(0.0f, 0.0f));
	  texcoords->push_back(osg::Vec2(1.0f, 0.0f));
	  texcoords->push_back(osg::Vec2(1.0f, 1.0f));
	  texcoords->push_back(osg::Vec2(0.0f, 1.0f));

	  osg::ref_ptr<osg::Geometry> quad = new osg::Geometry();
	  quad->setVertexArray(vertices.get());
	  quad->setTexCoordArray(0, texcoords.get());
	  quad->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );

	  osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	  geode->addDrawable(quad);
	  geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, client->_depth_texture);
	  geode->getOrCreateStateSet()->setAttributeAndModes(
		  loadShader("", "")
		  );
	  osg::ref_ptr<osg::Uniform> texDepth = new osg::Uniform("texDepth", 0);
	  osg::ref_ptr<osg::Uniform> texSize = new osg::Uniform("texSize", 
		  osg::Vec2(float(client->_pred_resolution._width), float(client->_pred_resolution._height)));
	  osg::ref_ptr<osg::Uniform> isShow = new osg::Uniform("isShow", false);
	  geode->getOrCreateStateSet()->addUniform(texDepth.get());
	  geode->getOrCreateStateSet()->addUniform(texSize.get());
	  geode->getOrCreateStateSet()->addUniform(isShow.get());
	  return geode;
	  */
	 return NULL;
  }


