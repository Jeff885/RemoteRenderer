#ifndef REMOTE_FIRSTPERSON_MANIPUALTOR
#define REMOTE_FIRSTPERSON_MANIPUALTOR



#include "Global.h"
#include <osgGA/FirstPersonManipulator>
#include <osg/Vec2>
#include <osg/Matrixf>
#include "Message.h"
enum
{
	MOVE_UP=1,
	TURN_RIGHT,
	MOVE_DOWN,
	TURN_LEFT,
};

class RemoteFirstPersonManipulator :public osgGA::FirstPersonManipulator
{
public:
	RemoteFirstPersonManipulator();
	void handle(const float x,const float y);
	int Direction(const float x,const float y);
	osg::Matrixd getInverseMatrix();
	bool isViewpointChanged();
	void setByMatrix( const osg::Matrixd& matrix);
	void setByLookAt(Camera c);
	void rotation();//Ðý×ª
	osg::Matrixd getPredInverseMatrix();//Ô¤²â
private:
	float lastcoordinate_x;
	float lastcoordinate_y;
	double m_distance;
	double v_distance;
	double m_rotation;
	double v_rotation;
	osg::Vec3f up;
	osg::Vec2 e[4];

	int pre_eye;
	int pre_rotation;

	static const float pred_var;
	static const float step;
	static const float step_rotation;
};
#endif