#include "Manipulator.h"
#include "VinoRender.h"
Manipulator::Manipulator()
	:_distance(1.0),
	_trackballSize(0.8),
	_minimunDistance(0.05)
{
}

osg::Matrixd Manipulator::getMatrix() const
{
	return osg::Matrixd::translate(0.0, 0.0, _distance) *
		osg::Matrixd::rotate(_rotation) *
		osg::Matrixd::translate(_center);
}

osg::Matrixd Manipulator::getInverseMatrix()
{
	return osg::Matrixd::translate(-_center)*
		osg::Matrixd::rotate(_rotation.inverse()) *
		osg::Matrixd::translate(0.0, 0.0, -_distance);
}

void Manipulator::setByMatrix(const osg::Matrixd& matrix)
{
	_center = osg::Vec3d(0., 0., -_distance) * matrix;
	_rotation = matrix.getRotate();
}

void Manipulator::setByLookAt(const osg::Vec3f& eye, const osg::Vec3f& center, const osg::Vec3f& up)
{
	osg::Matrixd  matrix;
	_distance = (eye - center).length();
	matrix.makeLookAt(eye, center, up);
	matrix = osg::Matrixd::inverse(matrix);
	setByMatrix(matrix);
}
void Manipulator::getCamera(osg::Vec3f& eye,  osg::Vec3f& center, osg::Vec3f& up)
{
	osg::Matrixf m= osg::Matrixf::translate(0.0, 0.0, _distance) *
			osg::Matrixf::rotate(_rotation) *
			osg::Matrixf::translate(_center);

	osg::Matrixf mat=osg::Matrixf::inverse(m);
	mat.getLookAt(eye,center,up);

}
void Manipulator::rotateTrackball(const float px0, const float py0, const float px1, const float py1)
{
	osg::Vec3d axis;
	float angle;
	trackball(axis, angle, px1, py1, px0, py0);

	osg::Quat new_rotate;
	new_rotate.makeRotate(angle, axis);

	_rotation = _rotation * new_rotate;
	_accRotation = _accRotation *new_rotate;
}

void Manipulator::trackball(osg::Vec3d& axis, float& angle, float p1x, float p1y,float p2x, float p2y)
{
	osg::Matrixd rotation_matrix(_rotation);

	osg::Vec3d uv = osg::Vec3d(0.0f,1.0f,0.0f)*rotation_matrix;
	osg::Vec3d sv = osg::Vec3d(1.0f,0.0f,0.0f)*rotation_matrix;
	osg::Vec3d lv = osg::Vec3d(0.0f,0.0f,-1.0f)*rotation_matrix;

	osg::Vec3d p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(_trackballSize, p1x, p1y);
	osg::Vec3d p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(_trackballSize, p2x, p2y);


	axis = p2^p1;
	axis.normalize();

	float t = (p2 - p1).length() / (2.0 * _trackballSize);


	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	angle = osg::inRadians(asin(t));

	angle = 0.1 * angle;
}

float Manipulator::tb_project_to_sphere(float r, float x, float y)
{
	float d, t, z;

	d = sqrt(x*x + y*y);
	if (d < r * 0.70710678118654752440)
	{
		z = sqrt(r*r - d*d);
	}                       
	else
	{
		t = r / 1.41421356237309504880;
		z = t*t / d;
	}
	return z;
}

void Manipulator::translate(const float dx, const float dy, const float dz)
{
	osg::Matrixd rotation_matrix;
	rotation_matrix.makeRotate(_rotation);
	osg::Vec3d dv(dx, dy, dz);

	//LOGI("length:%f", dv.length());
	//if(dv.length() > 8.0) return;
	if(dv.length() > 1000.0) return;
	dv = dv *rotation_matrix;
	_center +=dv;
	_accTranslation += dv;
}

void Manipulator::handle(const MotionData& md)
{
	if((md._type & (1 << 6)) && (md._type & (1<<7))) //action_move
	{
		if(_preTyte == 2) return;
		if((md._type & 1) && (md._type & 2))// two pointers
		{
			/*float midNow_x = (md._x1 + md._x2) / 2;
			float midNow_y = (md._y1 + md._y2) /2;

			float midLast_x = (_lastCood1._v[0] + _lastCood2._v[0]) /2;
			float midLast_y = (_lastCood1._v[1] + _lastCood2._v[1]) /2;

			float tran_dx =  midNow_x - midLast_x;
			float tran_dy = midNow_y - midLast_y;
			float scale = -0.2f * _distance;

			translate(tran_dx * scale, tran_dy * scale);*/
		}
		else if(md._type & 1)
			rotateTrackball(md._x1, md._y1,_lastCood1._v[0], _lastCood1._v[1]);
		else if(md._type & 2)
			rotateTrackball(md._x2, md._y2,_lastCood2._v[0], _lastCood2._v[1]);
	}
	updateLastPoint(md);
}

void Manipulator::updateLastPoint(const MotionData& md)
{
	_lastCood1._v[0] = md._x1;
	_lastCood1._v[1] = md._y1;
	_lastCood2._v[0] = md._x2;
	_lastCood2._v[1] = md._y2;
	if((md._type & (1 << 6)) && (md._type & (1 << 7)))
		_preTyte = 3;
	else if((md._type & (1 << 6)))
		_preTyte = 1;
	else
		_preTyte = 2;
}


void Manipulator::setDistance(const double& dis)
{
	_distance = dis;
}

void Manipulator::zoomModel(const float dy)
{
	float scale = 1.0f + dy * 0.5;

	float minDist = _minimunDistance;

	if(_distance * scale > minDist)
	{
		_distance *=scale;
	}
	else
	{
		_distance = minDist;
	}
}
