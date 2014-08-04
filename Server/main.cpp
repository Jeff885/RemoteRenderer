

#ifndef WINVER // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE // Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0601 // Change this to the appropriate value to target IE 5.0 or later.
#endif

#include "Test.h"
#include "Global.h"
#include "iostream"
//#include "ServerTask.h"
#include "Session.h"
#include "Server.h"
#include "RemoteViewer.h"
//#include "SceneMgr.h"
//RemoteViewer rv;
int main()
{
	//
	//
	//Scene_Mgr scene;
	server s(5588,5);
	s.run();

    //rv.start(&rv);
	


	//ClientTable t;
	//t.removeClient(10);

	//rv.run();
	//osg::ref_ptr<osg::Node> model=osgDB::readNodeFile("santa.ply");
	//osgViewer::Viewer viewer;
	//viewer.setUpViewInWindow(20,50,300,500);
	//viewer.setSceneData(model.get());
	//return viewer.run();
}