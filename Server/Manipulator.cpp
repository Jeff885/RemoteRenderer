
#include "Manipulator.h"

		Manipulator::Manipulator(Camera& camera)
			:_distance(1.0f),
			_trackball_size(0.8f),
			_min_distance(0.05),
			_acc_totalTime(150.0f) {
				/*osg::Vec3d eye(camera._eyex, camera._eyey, camera._eyez);
				osg::Vec3d center(camera._centerx, camera._centery, camera._centerz);
				osg::Vec3d up(camera._upx, camera._upy, camera._upz);

				_distance = (eye - center).length();

				osg::Matrixd  matrix;
				matrix.makeLookAt(eye, center, up);
				matrix = osg::Matrixd::inverse(matrix);
				_center = osg::Vec3d(0.0, 0.0, -_distance) * matrix;
				_rotation = matrix.getRotate();*/
				setByLookAt(camera);

		}
		void Manipulator::setByLookAt(Camera c)
		{
			Camera camera=c;
			osg::Vec3d eye(camera._eyex, camera._eyey, camera._eyez);
			osg::Vec3d center(camera._centerx, camera._centery, camera._centerz);
			osg::Vec3d up(camera._upx, camera._upy, camera._upz);

			_distance = (eye - center).length();

			osg::Matrixd  matrix;
			matrix.makeLookAt(eye, center, up);
			matrix = osg::Matrixd::inverse(matrix);
			_center = osg::Vec3d(0.0, 0.0, -_distance) * matrix;
			_rotation = matrix.getRotate();
		}
		osg::Matrixd Manipulator::getViewMatrix() {
			return osg::Matrixd::translate(0.0, 0.0, _distance) *
				osg::Matrixd::rotate(_rotation) *
				osg::Matrixd::translate(_center);
		}

		osg::Matrixd Manipulator::getInverseViewMatrix() {

			return  osg::Matrixd::translate(-_center)*
				osg::Matrixd::rotate(_rotation.inverse()) *
				osg::Matrixd::translate(0.0, 0.0, -_distance);

			//return osg::Matrixd::inverse(getViewMatrix());
		}

		osg::Matrixd Manipulator:: getPredInverseViewMatrix(double delta_time) {
			/*
			double acc_angle = 0.0;
			osg::Vec3d axis;
			_acc_rotation.getRotate(acc_angle, axis);

			// delta_t/t
			double ratio = delta_time / _acc_totalTime;
			//double ratio=0;
			

			// Ô¤²âÐý×ª
			osg::Quat delta_rotation(ratio * acc_angle , axis);
			osg::Quat pred_rotation = _rotation *delta_rotation;

			// Ô¤²âÆ½ÒÆ
			osg::Vec3d pred_center = _center +  _acc_translation * ratio;
			double  pred_distance = _distance + _acc_distance * ratio;
			pred_distance = pred_distance < _min_distance ? _min_distance : pred_distance;

			return osg::Matrixd::translate(-pred_center)*
				osg::Matrixd::rotate(pred_rotation.inverse()) *
				osg::Matrixd::translate(0.0, 0.0, -pred_distance);*/


			double epsilon = 0.00000001;
			double angle = 0.0;
			osg::Vec3d axis;
			_acc_rotation.getRotate(angle, axis);
			cout<<"½Ç¶È:"<<angle<<"   "<<endl;

			float scale = 0.0;
			if(_acc_totalTime > epsilon) scale = delta_time / _acc_totalTime;

			osg::Quat rotation(scale * angle , axis);
			rotation = _rotation *rotation;

			osg::Vec3d center = _center +  _acc_translation * scale;
			//2014/4/28
			double  distance = _distance;
			//double  distance = _distance+_acc_distance*scale;

			if(distance < _min_distance ) distance = _min_distance;

			return osg::Matrixd::translate(-center)*
				osg::Matrixd::rotate(rotation.inverse()) *
				osg::Matrixd::translate(0.0, 0.0, -distance);
		}

		osg::Matrixd Manipulator:: getPredViewMatrix(double delta_time) {

			return osg::Matrixd::inverse(getPredInverseViewMatrix(delta_time));
		}

		void Manipulator::update(TouchEvent &touch_event) {
			unsigned char  type = touch_event._event_type;
			if((type & (1 << 6)) && (type & (1<<7))) {
				if((type & 1) && (type & 2)){









					// do nothing
				}else if(type & 1){

					rotateTrackball(touch_event._x1, touch_event._y1,_last_cord1._v[0], _last_cord1._v[1]);
				}else if(type & 2){

					rotateTrackball(touch_event._x2, touch_event._y2,_last_cord2._v[0], _last_cord2._v[1]);
				}
			}
			updateLastPoint(touch_event);
		}

		/*bool timeForPrediction() {
			osg::Timer_t curr_time = osg::Timer::instance()->tick();
			double duration = osg::Timer::instance()->delta_m(_last_pred_time, curr_time);
			if(duration >= _pred_period){
				_last_pred_time = curr_time;
				if(isViewpointChanged()){
					return true;
				}
			}
			return false;
		}*/

		void Manipulator::resetPrediction() {
			_acc_translation.set(0.0, 0.0, 0.0);
			_acc_rotation.set(0.0, 0.0, 0.0, 1.0);
			_acc_distance = 1.0;
		}

		void Manipulator::setAccTotalTime(double time) {
			_acc_totalTime = time;
		}


		bool Manipulator::isViewpointChanged() {
			if(1 - abs(_acc_rotation.w()) > 0.00016  || _acc_translation.length() >0.005) { 
				return true;
			}
			return false;
			//return true;
		}

		void Manipulator::rotateTrackball(float px0, float py0, float px1, float py1) {
			osg::Vec3d axis;
			float angle;
			calcRotationArgs(axis, angle, px1, py1, px0, py0);
			osg::Quat new_rotate;
			new_rotate.makeRotate(angle, axis);

			_rotation = _rotation * new_rotate;
			_acc_rotation = _acc_rotation *new_rotate;
		}

		void Manipulator::calcRotationArgs(osg::Vec3d& axis, float& angle, 
			float p1x, float p1y,float p2x, float p2y) {
			osg::Matrixd rotation_matrix(_rotation);

			osg::Vec3d uv = osg::Vec3d(0.0f,1.0f,0.0f)*rotation_matrix;
			osg::Vec3d sv = osg::Vec3d(1.0f,0.0f,0.0f)*rotation_matrix;
			osg::Vec3d lv = osg::Vec3d(0.0f,0.0f,-1.0f)*rotation_matrix;

			osg::Vec3d p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(_trackball_size, p1x, p1y);
			osg::Vec3d p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(_trackball_size, p2x, p2y);

			axis = p2^p1;
			axis.normalize();

			float t = (p2 - p1).length() / (2.0 * _trackball_size);

			if (t > 1.0) t = 1.0;
			if (t < -1.0) t = -1.0;
			angle = osg::inRadians(asin(t));
			//2014/4/28
			angle = 0.1 * angle;
		}

		float Manipulator::tb_project_to_sphere(float r, float x, float y)
		{
			float d, t, z;

			d = sqrt(x*x + y*y);
			if (d < r * 0.70710678118654752440) {
				z = sqrt(r*r - d*d);
			}                       
			else {
				t = r / 1.41421356237309504880;
				z = t*t / d;
			}
			return z;
		}

		void Manipulator::translate(float dx, float dy, float dz) {
			osg::Matrixd rotation_matrix;
			rotation_matrix.makeRotate(_rotation);
			osg::Vec3d dv(dx, dy, dz);

			dv = dv *rotation_matrix;
			_center += dv;
			_acc_translation += dv;
		}

		void Manipulator::updateLastPoint(TouchEvent &touch_event) {

			_last_cord1._v[0] = touch_event._x1;
			_last_cord1._v[1] = touch_event._y1;
			_last_cord2._v[0] = touch_event._x2;
			_last_cord2._v[1] = touch_event._y2;
		}
		void Manipulator::handleUpdate(TouchEvent &touch_event)
		{
			update(touch_event);//·â×°Ò»²ã
		}
		osg::Matrixd Manipulator::getPredInverseMatrix(double delta_time)
		{
			 return getPredInverseViewMatrix(delta_time);
		}
		Manipulator::~Manipulator()
		{
			cout<<"Ö´ÐÐManipulatorÎö¹¹"<<endl;
		}
		
		

