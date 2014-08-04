
#include "CameraOperator.h"
CameraOperator::CameraOperator(Camera& camera):isChange(false)
{
	mat.makeLookAt(osg::Vec3f(camera._eyex,camera._eyey,camera._eyez),
		osg::Vec3f(camera._centerx,camera._centery,camera._centerz),
		osg::Vec3f(camera._upx,camera._upy,camera._upz));
	isChange=false;
}
void CameraOperator::handleUpdate(TouchEvent &touch_event)
{
	
	
}

bool CameraOperator::isViewpointChanged()
{
	return isChange;
}

osg::Matrixd CameraOperator::getInverseViewMatrix()
{
	//return NULL;
	return osg::Matrixf::inverse(mat);
}
osg::Matrixd CameraOperator::getPredInverseMatrix(double delta_time)
{
	return osg::Matrixf::inverse(mat);
	//return NULL;
}
void CameraOperator::resetPrediction()
{

}
void CameraOperator::setByLookAt(Camera camera)
{
	mat.makeLookAt(osg::Vec3f(camera._eyex,camera._eyey,camera._eyez),
		osg::Vec3f(camera._centerx,camera._centery,camera._centerz),
		osg::Vec3f(camera._upx,camera._upy,camera._upz));
	isChange=true;
}