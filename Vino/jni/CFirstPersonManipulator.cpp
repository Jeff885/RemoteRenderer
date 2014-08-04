
#include "CFirstPersonManipulator.h"
#include <osg/Matrixf>

/*

客户端的代码
*/
/*
 0.1
 3200
 */
const float CFirstPersonManipulator::step=0.2;
const float CFirstPersonManipulator::rotation_step=(osg::PI/2800);

CFirstPersonManipulator::CFirstPersonManipulator():lastcoordinate_x(-1),
	lastcoordinate_y(1),
	m_distance(0)
{
	e[0]=osg::Vec2f(0,1);
	e[1]=osg::Vec2f(1,0);
	e[2]=osg::Vec2f(0,-1);
	e[3]=osg::Vec2f(-1,0);
}

osg::Matrixd CFirstPersonManipulator::getInverseMatrix()
{
	return osg::Matrixd::translate( -_eye ) * osg::Matrixd::rotate( _rotation.inverse() );
}
void CFirstPersonManipulator::setByLookAt(const osg::Vec3f& eye, const osg::Vec3f& center, const osg::Vec3f& up)
{
	this->up=up;
	osg::Matrixd m( osg::Matrixd::lookAt( eye, center, up ) );
	_eye = eye;
	_rotation = m.getRotate().inverse();
	//fixVerticalAxis( _rotation, true );
}
void CFirstPersonManipulator::getCamera(osg::Vec3f& eye,osg::Vec3f& center,osg::Vec3f& up)
{
	osg::Matrixf mat=osg::Matrixf::translate( -_eye ) * osg::Matrixf::rotate( _rotation.inverse() );

	//osg::Matrixf m=osg::Matrixf::inverse(mat);

	mat.getLookAt(eye,center,up);
}
void CFirstPersonManipulator::fixVerticalAxis(Quat& rotation, bool disallowFlipOver )
{
	osg::Matrixd coordinateFrame;
	//Vec3d localUp = getUpVector( coordinateFrame );
	Vec3d localUp =osg::Vec3d(coordinateFrame(2,0),coordinateFrame(2,1),coordinateFrame(2,2));

	fixVerticalAxis( rotation, localUp, disallowFlipOver );
}
void CFirstPersonManipulator::fixVerticalAxis( Quat& rotation, const Vec3d& localUp, bool disallowFlipOver )
{
	// camera direction vectors
	Vec3d cameraUp = rotation * Vec3d( 0.,1.,0. );
	Vec3d cameraRight = rotation * Vec3d( 1.,0.,0. );
	Vec3d cameraForward = rotation * Vec3d( 0.,0.,-1. );

	// computed directions
	Vec3d newCameraRight1 = cameraForward ^ localUp;
	Vec3d newCameraRight2 = cameraUp ^ localUp;
	Vec3d newCameraRight = (newCameraRight1.length2() > newCameraRight2.length2()) ?
newCameraRight1 : newCameraRight2;
	if( newCameraRight * cameraRight < 0. )
		newCameraRight = -newCameraRight;

	// vertical axis correction
	Quat rotationVerticalAxisCorrection;
	rotationVerticalAxisCorrection.makeRotate( cameraRight, newCameraRight );

	// rotate camera
	rotation *= rotationVerticalAxisCorrection;

	if( disallowFlipOver )
	{

		// make viewer's up vector to be always less than 90 degrees from "up" axis
		Vec3d newCameraUp = newCameraRight ^ cameraForward;
		if( newCameraUp * localUp < 0. )
			rotation = Quat( PI, Vec3d( 0.,0.,1. ) ) * rotation;

	}
}


int CFirstPersonManipulator::Direction(const float x,const float y)
{
	if(lastcoordinate_x == -1 && lastcoordinate_y ==1)
		return 0;
	osg::Vec2f direction=osg::Vec2f(x-lastcoordinate_x,y-lastcoordinate_y);
	double max=e[0]*direction;
	int index=1;
	for(int i=1;i<=3;i++)
	{
		double t=e[i]*direction;
		if(max<t)
		{
			max=t;
			index=i+1;
		}
	}
	return index;
}
void CFirstPersonManipulator::handle(const MotionData& md)
{
	handle(md._x1,md._y1);
}
void CFirstPersonManipulator::handle(const float x,const float y)
{
	switch(Direction(x,y))
	{
	case 0:
		break;
	case MOVE_UP:
		//m_distance+=step;
		_eye += _rotation * Vec3d( 0., 0., -step );
		break;
	case TURN_LEFT:

		{
			osg::Quat new_rotation;
			new_rotation.makeRotate(-rotation_step,up);
			_rotation*=new_rotation;
		}
		break;
	case TURN_RIGHT:
		{
			osg::Quat new_rotation;
			new_rotation.makeRotate(rotation_step,up);
			_rotation*=new_rotation;
		}
		break;
	case MOVE_DOWN:
		//m_distance-=step;
		_eye -= _rotation * Vec3d( 0., 0., -step);
		break;
	default:
		break;
	}

	 
	lastcoordinate_x=x;
	lastcoordinate_y=y;
}
