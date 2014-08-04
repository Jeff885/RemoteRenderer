#include "PreDrawCallback.h"
#include <osg/Vec3>
#include <iostream>
void PreDrawCallBack::operator() (osg::RenderInfo& renderInfo) const
{
	//RemoteViewer* rv=dynamic_cast<RemoteViewer* >(renderInfo.getView());
//	rv
	osg::Vec3f eye,center,up;
	renderInfo.getCurrentCamera()->getViewMatrixAsLookAt(eye,center,up);
	std::cout<<"eye: "<<eye.x()<<" "<<eye.y()<<" "<<eye.z()<<std::endl;
	std::cout<<"center: "<<center.x()<<" "<<center.y()<<" "<<center.z()<<endl;
	std::cout<<"up: "<<up.x()<<" "<<up.y()<<" "<<up.z()<<endl;

}