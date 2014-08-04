
#pragma  once
#ifndef CAMERAOPERATOR_H
#define CAMERAOPERATOR_H
#include "Global.h"
#include "Message.h"
#include "ManipulatorInterface.h"
class ManipulatorInterface;
class CameraOperator : public ManipulatorInterface
{
public:
	virtual void handleUpdate(TouchEvent &touch_event);
	CameraOperator(Camera& camera);

	virtual bool isViewpointChanged();

	virtual osg::Matrixd getInverseViewMatrix();

	virtual osg::Matrixd getPredInverseMatrix(double delta_time);

	virtual void resetPrediction();
	virtual void setByLookAt(Camera c);

	virtual ~CameraOperator(){}
private:
	bool isChange;
	osg::Matrixf mat;
};
#endif CAMERAOPERATOR_H