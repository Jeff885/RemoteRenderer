
#ifndef MANIPULATORINTERFACE
#define MANIPULATORINTERFACE

#include <osg/Matrixd>
#include <osg/Vec2f>
#include <osg/Quat>
#include "GlobalInfo.h"
class ManipulatorInterface
{
   public:
	virtual void setByLookAt(const osg::Vec3f& eye, const osg::Vec3f& center, const osg::Vec3f& up)=0;
	virtual osg::Matrixd getInverseMatrix()=0;
	virtual void handle(const MotionData& md)=0;
	virtual void getCamera(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up)=0;
};
#endif

