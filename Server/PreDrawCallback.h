#ifndef RREDRAW_CALLBACK
#define RREDRAW_CALLBACK
#include "Global.h"
#include <osg/Camera>
#include "RemoteViewer.h"
class PreDrawCallBack :public osg::Camera::DrawCallback
{
public:
	virtual void operator() (osg::RenderInfo& renderInfo) const;

};
#endif