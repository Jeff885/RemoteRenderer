#ifndef  __REMOTE_MANIPULATOR_H__
#define  __REMOTE_MANIPULATOR_H__

#include <osg/Vec3f>
#include <osg/Quat>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include "GlobalInfo.h"
#include "ManipulatorInterface.h"

//2014_7_12
//struct MotionData
//{
	//static const int SIZE = 17;
	//unsigned char	_type;
	//float	_x1;
	//float	_y1;
	//float	_x2;
	//float	_y2;
//};

class Manipulator : public ManipulatorInterface
{
public:

	Manipulator();

	virtual ~Manipulator() {}

	void setByMatrix(const osg::Matrixd& matrix);

	virtual void setByLookAt(const osg::Vec3f& eye, const osg::Vec3f& center, const osg::Vec3f& up);

	osg::Matrixd getMatrix() const;

	virtual osg::Matrixd getInverseMatrix();

	virtual void handle(const MotionData& md);
	virtual void getCamera(osg::Vec3f& eye,osg::Vec3f& center,osg::Vec3f& up);

	//2014_7_12
	void setDistance(const double& dis);

protected:
	void  rotateTrackball(const float px0, const float py0,
		const float px1, const float py1);

	void trackball(osg::Vec3d& axis, float& angle, float p1x, float p1y,float p2x, float p2y);

	float tb_project_to_sphere(float r, float x, float y);

	void translate(const float dx, const float dy, const float dz = 0.0f);

	void updateLastPoint(const MotionData& md);

	void zoomModel(const float dy);

private:
	osg::Vec3d _center;
	osg::Quat _rotation;
	double _distance;

	//a  accumulated change in the prediction period;
	osg::Vec3d _accTranslation;
	osg::Quat _accRotation;
	double _accDistance;

	double _trackballSize;
	//double _ZoomFactor;

	double _minimunDistance;
	double _accTotalTime;

	osg::Vec2f _lastCood1;
	osg::Vec2f _lastCood2;

	int _preTyte;
};
#endif
