
#ifndef CAMERAOPERATOR_H
#define CAMERAOPERATOR_H

#include <osg/Matrixd>
#include <osg/Vec2f>
#include <osg/Quat>
#include "GlobalInfo.h"
#include "ManipulatorInterface.h"
#include "osg/Matrixf"


class CameraOperator: public ManipulatorInterface
{
public:
	CameraOperator();
	virtual void setByLookAt(const osg::Vec3f& eye, const osg::Vec3f& center, const osg::Vec3f& up);
	virtual osg::Matrixd getInverseMatrix();
	virtual void handle(const MotionData& md);
	virtual void getCamera(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up);
private:
	osg::Matrixf mat;
};


#endif	
