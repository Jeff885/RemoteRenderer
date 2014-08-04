//#include <Windows.h>
#include "PredictionCallback.h"
////#include "Global.h"
//#include <boost/thread/thread.hpp>
#include "RemoteViewer.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include "Global.h"
#include <boost/thread/thread.hpp>
int PredictionCallback::framecount=0;

void PredictionCallback::operator() (osg::RenderInfo& renderInfo) const
{
	
	RemoteViewer* view = dynamic_cast<RemoteViewer*> (renderInfo.getView());
	string s=renderInfo.getCurrentCamera()->getDescription(0);
	//cout<<"Description: "<<s<<endl;



	assert(view != NULL);
	if((++framecount)/100 == 0)
	{
		//osgDB::writeImageFile(*(view->_colorImage.get()),"abc.bmp");
		//osgDB::writeImageFile(*(view->_edgeImage.get()),"edgeImage.bmp");
		
		framecount=0;

		

	}
	ClientTable* ct=view->_clientTable;
	//s.compare(;
	if(!s.compare("local") && false)
	{
		
		view->_isDataProcessed = true;
		//if((++framecount)/10 == 0)
		{
			osg::Timer_t timeTemp=osg::Timer::instance()->tick();
			
			//framecount=0;
			osg::Timer_t tt=osg::Timer::instance()->tick();
			cout<<"写文件时间："<<osg::Timer::instance()->delta_m(timeTemp,tt)<<endl;
			double fovy,asp,tnear,tfar;
			view->_colorCam->getProjectionMatrix().getPerspective(fovy,asp,tnear,tfar);
			cout<<"fovy:"<<fovy<<"    asp:"<<asp<<"   tnear:"<<tnear<<"   tfar:"<<tfar<<endl;
		}

		boost::thread thd(boost::bind(&startCompress, view));
		//count<<view->_colorImage-><<endl;;
		//view->_isDataProcessed = true;
		//view->setPredictionToggle(false);

		view->_isDataProcessed = false;
		

	}else
	{

		//cout<<"Client Predictioncallback"<<endl;
		//size_t id=s[0]-'0';
		size_t id=StringToId(s);
		Client* client=ct->getClient(id);
		if(client!=NULL)
		{
			
			if(!client->_isNiceconnect)
			{
				//view->sceneGroup->removeChild(client->toggle.get());
				//delete client;
				//client->_vmt->_runSwitch=false;
				//ct->removeClient(id);
				client->_isDied=true;//客户端死掉
				return;
			}

			client->_isDataProcessed=true;
			//osgDB::writeImageFile(*(client->_colorImage.get()),"client_abc.bmp");
			//view->_edgeImage->
			//osgDB::writeImageFile(*(view->_edgeImage.get()),"edge.bmp");
			//view->getCamera()->setViewMatrix(client->_manipulator->getInverseViewMatrix());
			boost::thread thd(boost::bind(&startClientCompress,client));
		}
	}
	
}
unsigned int PredictionCallback::StringToId(string id)
{
	//int id=
	unsigned int r=0;
	for(int i=id.size()-1,j=0;i>=0;i--,j++)
		r+=(id[i]-'0')*pow(10.0,j);

	return r;
}
void PredictionCallback::startClientCompress(Client* _client)
{
	cout<<"Client startCompress"<<endl;
	_client->_compressor->doCompress(_client->_colorImage.get(),_client->_edgeImage.get());


	_client->sentOnePacket(
		VINO_MODEL_3DIMAGE,
		_client->_compressor->getJpegSize(),
		_client->_compressor->getJpegBuffer(),
		_client->_compressor->getDepthSrcSize(),
		_client->_compressor->getDepthDstSize(),
		_client->_compressor->getDepthDstBuffer(),
		_client->getPredInvMVP()
		);
	_client->_isDataProcessed=false;



	///保存帧
	/*
		const float epsilon = 0.000005;
			int width = _client->_edgeImage->s();
			int height =_client->_edgeImage->t();
		 	float* data = reinterpret_cast<float*>(_client->_edgeImage->data());
		 	osg::ref_ptr<osg::Image> tmp = new osg::Image;
			tmp->allocateImage(
		 		width,
		 		height,
				1,
	 		GL_RGB,
	 		GL_UNSIGNED_BYTE
		 		);
		 	unsigned char* tmpData = reinterpret_cast<unsigned char*>(tmp->data());
		 	int i;
		 	for(i = 0; i < width * height; ++i) 
		 	{
		 		tmpData[3*i] = 0;
		 		tmpData[3*i + 1] = 0;
		 		tmpData[3*i + 2] = 0;
		 		if(data[2*i + 1] > epsilon)
		 		{
		 			tmpData[3*i] = 255;
		 			tmpData[3*i + 1] = 255;
		 			tmpData[3*i + 2] = 0;
		 		}
			}
		  	osgDB::writeImageFile(*(_client->_colorImage), "color.bmp");
		  	osgDB::writeImageFile(*tmp, "edge.bmp");
			*/
			///

	
}
void PredictionCallback::startCompress(RemoteViewer* view)
{

	view->_compressor->doCompress(view->_colorImage.get(), view->_edgeImage.get());
	
	//cout<<"startCompress"<<endl;
	/*
	view->_compressor->doCompress(view->_colorImage.get(), view->_edgeImage.get());
	if(view->_compressor->isDataAccess())
	{
		view->_adapter->sentOnePacket(
			VINO_MODEL_3DIMAGE,
			view->_compressor->getJpegSize(),
			view->_compressor->getJpegBuffer(),
			view->_compressor->getDepthSrcSize(),
			view->_compressor->getDepthDstSize(),
			view->_compressor->getDepthDstBuffer(),
			view->getPredInvMVP()
			);
		view->_compressor->banDataAccess();
	}
	view->_isDataProcessed = false;
	*/
	view->_isDataProcessed = false;
	// 	const float epsilon = 0.000005;
	// 	int width = view->_edgeImage->s();
	// 	int height =view->_edgeImage->t();
	// 	float* data = reinterpret_cast<float*>(view->_edgeImage->data());
	// 	osg::ref_ptr<osg::Image> tmp = new osg::Image;
	// 	tmp->allocateImage(
	// 		width,
	// 		height,
	// 		1,
	// 		GL_RGB,
	// 		GL_UNSIGNED_BYTE
	// 		);
	// 	unsigned char* tmpData = reinterpret_cast<unsigned char*>(tmp->data());
	// 	int i;
	// 	for(i = 0; i < width * height; ++i) 
	// 	{
	// 		tmpData[3*i] = 0;
	// 		tmpData[3*i + 1] = 0;
	// 		tmpData[3*i + 2] = 0;
	// 		if(data[2*i + 1] > epsilon)
	// 		{
	// 			tmpData[3*i] = 255;
	// 			tmpData[3*i + 1] = 255;
	// 			tmpData[3*i + 2] = 0;
	// 		}
	// 	}
	// 	osgDB::writeImageFile(*(view->_colorImage), "color.bmp");
	// 	osgDB::writeImageFile(*tmp, "edge.bmp");
	// 	view->_isDataProcessed = false;
}