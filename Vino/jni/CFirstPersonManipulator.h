#ifndef CFIRSTPERSONMANIPULATOR_H
#define CFIRSTPERSONMANIPULATOR_H
#include <osg/Matrixd>
#include <osg/Vec2f>
#include <osg/Quat>
#include "GlobalInfo.h"
#include "ManipulatorInterface.h"

using namespace osg;

enum
{
	MOVE_UP=1,
	TURN_RIGHT,
	MOVE_DOWN,
	TURN_LEFT,
};

class CFirstPersonManipulator : public ManipulatorInterface
{
public:
	CFirstPersonManipulator();
	void handle(const float x, const float y);
	int Direction(const float x,const float y);
	virtual osg::Matrixd getInverseMatrix();
	virtual void setByLookAt(const osg::Vec3f& eye, const osg::Vec3f& center, const osg::Vec3f& up);
	//virtual void getCamera(osg::Vec3f& eye,osg::Vec3f& center,osg::Vec3f& up);
	void fixVerticalAxis(Quat& rotation, bool disallowFlipOver);
	void fixVerticalAxis( Quat& rotation, const Vec3d& localUp, bool disallowFlipOver );

	virtual void getCamera(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up);
	//bool isViewpointChanged();
	//2014_7_12
	virtual void handle(const MotionData& md);
private:
	float lastcoordinate_x;
	float lastcoordinate_y;


	osg::Vec2f e[4];
	static const float step;
	static const float rotation_step;
	//
	//osg::Vec3d _center;
	//osg::Quat _rotation;
	//double _distance;
	double m_distance;
	osg::Vec3d _eye;
	osg::Vec3f up;
	osg::Quat  _rotation;

};

#endif

