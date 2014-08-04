// 控制器类头文件
// author：胡山峰
// date：2013-8-30

#ifndef _MANIPULATOR_H
#define _MANIPULATOR_H
#include "Message.h"
#include "Global.h"
#include "Client.h"

#include "TouchEventBuffer.h"
#include "ManipulatorInterface.h"
// 控制器类

class Manipulator : public ManipulatorInterface{
	public:
	virtual void handleUpdate(TouchEvent &touch_event);
	virtual bool isViewpointChanged();
	virtual osg::Matrixd getInverseViewMatrix();
	virtual osg::Matrixd getPredInverseMatrix(double delta_time); 
    virtual void resetPrediction();
	virtual ~Manipulator();
	public:
		Manipulator(Camera& camera);
			
		virtual void setByLookAt(Camera c);
		osg::Matrixd getViewMatrix(); 

		//osg::Matrixd getInverseViewMatrix();
		
		osg::Matrixd getPredInverseViewMatrix(double delta_time);

		osg::Matrixd getPredViewMatrix(double delta_time);

		void update(TouchEvent &touch_event);

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

		//void resetPrediction();
		void setAccTotalTime(double time) ;

		//bool isViewpointChanged() ;

	private:
		void rotateTrackball(float px0, float py0, float px1, float py1);

		void calcRotationArgs(osg::Vec3d& axis, float& angle, 
			float p1x, float p1y,float p2x, float p2y);

		float tb_project_to_sphere(float r, float x, float y);
		
		void translate(float dx, float dy, float dz=0);

		void updateLastPoint(TouchEvent &touch_event) ;
	private:
		// 记录当前视点变换
		osg::Vec3d _center;
		osg::Quat _rotation;
		double _distance;

		// 记录自上次预测时刻到当前的视点变换
		osg::Vec3d _acc_translation;
		osg::Quat _acc_rotation;
		double _acc_distance;

		double _trackball_size;

		double _min_distance;
		double _acc_totalTime;

		osg::Vec2f _last_cord1;
		osg::Vec2f _last_cord2;

		// 上次预测时刻
		

		//// 预测周期
		//double _pred_period;
	};


#endif