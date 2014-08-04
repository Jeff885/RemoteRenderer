#ifndef  _customized_global_info_h_
#define  _customized_global_info_h_

#include "DepthImageBase.h"
#include <osg/Vec3f>
#include<android/log.h>


#define  LOG_TAG    "VINO_JNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

struct  Resolution
{
	int w;
	int h;
};

struct Perspective
{
	float fov;
	float aspect;
	float near;
	float far;
};

struct  stdCamPos
{
	osg::Vec3f		eye;
	osg::Vec3f		center;
	osg::Vec3f		up;
};
//2014_7_12
struct MotionData
{
	static const int SIZE = 17;
	unsigned char	_type;
	float	_x1;
	float	_y1;
	float	_x2;
	float	_y2;
};

class GlobalInfo
{
public:

	static GlobalInfo& getInstance()
	{
		static GlobalInfo singles;
		return singles;
	}

	inline void setShowRes(int width, int height)
	{
		mShowRes.w = width;
		mShowRes.h = height;
	}

	inline void setColorRes(int width, int height)
	{
		mColorRes.w = width;
		mColorRes.h = height;
	}

	inline void setDepthRes(int width, int height)
	{
		mDepthRes.w = width;
		mDepthRes.h = height;
	}
	inline void setPerspective(float& fov, float& aspect, float& near, float& far)
	{
		mPersp.fov = fov;
		mPersp.aspect = aspect;
		mPersp.near = near;
		mPersp.far = far;
	}

	inline void setStdCamPos(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up)
	{
		mCamPos.eye = eye;
		mCamPos.center = center;
		mCamPos.up = up;
	}
	inline DepthImageBase& getDepthImageBase()
	{
		return mBase;
	}

	inline const Resolution& getShowRes()
	{
		return mShowRes;
	}
	inline const Resolution& getColorRes()
	{
		return mColorRes;
	}
	inline const Resolution& getDepthRes()
	{
		return mDepthRes;
	}
	
	inline const Perspective& getPerspective()
	{
		return mPersp;
	}

	inline const stdCamPos& getStdCamPos()
	{
		return mCamPos;
	}
private:
	GlobalInfo() {}
	~GlobalInfo() {}

	Resolution mShowRes;

	Resolution mColorRes;

	Resolution mDepthRes;

	Perspective  mPersp;

	stdCamPos	  mCamPos;

	DepthImageBase mBase;
};


#endif
