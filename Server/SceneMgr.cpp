#include <Windows.h>
#include <osg/Node>
#include "SceneMgr.h"
#include <string>

bool Scene_Mgr::loadMode=false;
bool Scene_Mgr::loadStatus=false;
Scene_Mgr* Scene_Mgr::_scene_mgr=NULL;
MapScene Scene_Mgr::_sceneMap;
Scene_Mgr::Scene_Mgr()
{

}
osg::ref_ptr<osg::Node> Scene_Mgr::getSceneByName(string name)
{
	MapScene::iterator it= _sceneMap.find(name);
	osg::ref_ptr<osg::Node> result=it->second;
	return result;
}
Scene_Mgr* Scene_Mgr::instance()
{
	LoadScene();
	if(_scene_mgr==NULL)
		_scene_mgr=new Scene_Mgr;

	return _scene_mgr;
}
bool Scene_Mgr::LoadScene()
{
	if(!loadStatus)
	{
		cout<<"场景加载中...."<<endl;
		osg::ref_ptr<osg::Node> node_paris=osgDB::readNodeFile("D:\\sceneData\\paris_no_tree.osg");
		osg::ref_ptr<osg::Node> node_engine=osgDB::readNodeFile("D:\\sceneData\\engine2.osg");
		osg::ref_ptr<osg::Node> node_cessna=osgDB::readNodeFile("D:\\sceneData\\cessna.osg");
		osg::ref_ptr<osg::Node> node_armadillo=osgDB::readNodeFile("D:\\sceneData\\armadillo.ply");
		_sceneMap.insert(pair<string,osg::ref_ptr<osg::Node>>("paris",node_paris));
		_sceneMap.insert(pair<string,osg::ref_ptr<osg::Node>>("engine",node_engine));
		_sceneMap.insert(pair<string,osg::ref_ptr<osg::Node>>("cessna",node_cessna));
		_sceneMap.insert(pair<string,osg::ref_ptr<osg::Node>>("armadillo",node_armadillo));
		loadStatus=true;
		cout<<"场景加载完成...."<<endl;
	}

	return loadStatus;
}
bool Scene_Mgr::getLoadStatus()
{
	return loadStatus;
}