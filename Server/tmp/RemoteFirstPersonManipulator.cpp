#include "RemoteFirstPersonManipulator.h"
const float RemoteFirstPersonManipulator::step=0.2;
const float RemoteFirstPersonManipulator::step_rotation=(osg::PI/2800);//5¶È
const float RemoteFirstPersonManipulator::pred_var=5;
RemoteFirstPersonManipulator::RemoteFirstPersonManipulator():lastcoordinate_x(-1),
	lastcoordinate_y(1),
	m_distance(0),
	v_distance(0),
	m_rotation(0),
	v_rotation(0)
	//dx(0)
	//dy(0)
{
	e[0]=osg::Vec2(0,1);
	e[1]=osg::Vec2(1,0);
	e[2]=osg::Vec2(0,-1);
	e[3]=osg::Vec2(-1,0);


	pre_eye=0;
	pre_rotation=0;
}
void RemoteFirstPersonManipulator::handle(const float x,const float y)
{
	switch(Direction(x,y))
	{
	case 0:
		break;
	case MOVE_UP:
		//m_distance+=step;
		FirstPersonManipulator::moveForward(step);
		v_distance+=step;
		pre_eye++;
		break;
	case TURN_LEFT:
		{
			//dx+=x-lastcoordinate_x;
			//dy+=y-lastcoordinate_y;
			//m_rotation+=step_rotation;
			osg::Quat new_rotation;

			new_rotation.makeRotate(-step_rotation,up);
			//FirstPersonManipulator::performMouseDeltaMovement(dx,dy);
			//dx=0;
			//dy=0;
			FirstPersonManipulator::_rotation*=new_rotation;

			v_rotation+=step_rotation;
			pre_rotation--;
			
		}
		break;
	case TURN_RIGHT:
		{
			osg::Quat new_rotation;

			new_rotation.makeRotate(step_rotation,up);
			//FirstPersonManipulator::performMouseDeltaMovement(dx,dy);
			//dx=0;
			//dy=0;
			FirstPersonManipulator::_rotation*=new_rotation;
			//dx+=x-lastcoordinate_x;
			//dy+=y-lastcoordinate_y;
			//m_rotation-=step_rotation;
			v_rotation+=step_rotation;
			pre_rotation++;
		}
		break;
	case MOVE_DOWN:
		//m_distance-=step;
		FirstPersonManipulator::moveForward(-step);
		v_distance+=step;
		pre_eye--;
		break;
	default:
		break;
	}
	lastcoordinate_x=x;
	lastcoordinate_y=y;
}
int RemoteFirstPersonManipulator::Direction(const float x,const float y)
{
	if(lastcoordinate_x == -1 && lastcoordinate_y ==1)
		return 0;
	osg::Vec2 direction=osg::Vec2(x-lastcoordinate_x,y-lastcoordinate_y);
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
osg::Matrixd RemoteFirstPersonManipulator::getInverseMatrix()
{
	
	v_distance=0;
	v_rotation=0;
	return FirstPersonManipulator::getInverseMatrix();
}

osg::Matrixd RemoteFirstPersonManipulator::getPredInverseMatrix()//Ô¤²â
{
    

	 osg::Vec3 eye=_eye;
	 osg::Quat rotation=_rotation;
	 
	 if(pre_eye>0)
	 {
		eye += rotation * osg::Vec3d( 0., 0., -pred_var*step);//¸úrotationÏà¹Ø
	 }else
	 {
		 eye -= rotation * osg::Vec3d( 0., 0., -pred_var*step);
	 }
	  
	  osg::Quat new_rotation;
	  //new_rotation.makeRotate();
	  if(pre_rotation>0)
	  {
		  new_rotation.makeRotate(pred_var*step_rotation,up);
	  }else
	  {
		  new_rotation.makeRotate(-pred_var*step_rotation,up);
	  }

	  rotation*=new_rotation;

	  
	  pre_eye=0;
	  pre_rotation=0;
	  
	 return osg::Matrixd::translate(-eye) * osg::Matrixd::rotate(rotation.inverse() );
}
void RemoteFirstPersonManipulator::setByMatrix( const osg::Matrixd& matrix)
{
	FirstPersonManipulator::setByMatrix(matrix);
}
void RemoteFirstPersonManipulator::setByLookAt(Camera c)
{
	osg::Vec3f eye(c._eyex,c._eyey,c._eyez);
	osg::Vec3f center(c._centerx,c._centery,c._centerz);
	osg::Vec3f up(c._upx,c._upy,c._upz);

	this->up=up;
	FirstPersonManipulator::setTransformation(eye,center,up);
}
bool RemoteFirstPersonManipulator::isViewpointChanged()
{
	/*
	if(v_distance >=8)
		return true;

	return false;*/
	return true;
}