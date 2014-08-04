#include "CameraOperator.h"
#include "osg/Matrixf"

CameraOperator::CameraOperator()
{

}
void CameraOperator::setByLookAt(const osg::Vec3f& eye, const osg::Vec3f& center, const osg::Vec3f& up)
{
	LOGI("setCamera!!");
	mat.makeLookAt(eye,center,up);
}
osg::Matrixd CameraOperator::getInverseMatrix()
{
	return osg::Matrixf::inverse(mat);
}
void CameraOperator::handle(const MotionData& md)
{

}
void CameraOperator::getCamera(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up)
{
	LOGI("getCamera!!");
	mat.getLookAt(eye,center,up);
}
