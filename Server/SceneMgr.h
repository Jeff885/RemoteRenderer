#ifndef SCENE_MGR
#define SCENE_MGR
#include <osgDB/ReadFile>
#include <map>
#include <string>
#include <iostream>
using namespace std;
typedef map<string,osg::ref_ptr<osg::Node>> MapScene;
class Scene_Mgr
{
	static MapScene _sceneMap;
	static bool loadStatus;
	static Scene_Mgr* _scene_mgr;
	static bool loadMode;

public:
	osg::ref_ptr<osg::Node> getSceneByName(string name);
	
	static Scene_Mgr* instance();
	
	static bool LoadScene();

	static bool getLoadStatus();
	
public:
	Scene_Mgr();
};

#endif