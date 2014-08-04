#ifndef MANIPULATORINTERFACE_H
#define MANIPULATORINTERFACE_H
#include "Global.h"
#include "Message.h"

class ManipulatorInterface
{
public:
	virtual void handleUpdate(TouchEvent &touch_event)=0;
	
	virtual bool isViewpointChanged()=0;
	
	virtual osg::Matrixd getInverseViewMatrix()=0;
	
	virtual osg::Matrixd getPredInverseMatrix(double delta_time)=0;
	
	virtual void resetPrediction()=0;
	virtual void setByLookAt(Camera c)=0;

	virtual ~ManipulatorInterface(){}

};
#endif MANIPULATORINTERFACE_H