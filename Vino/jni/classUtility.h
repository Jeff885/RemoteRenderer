#ifndef  _customized_class_utility_h_
#define  _customized_class_utility_h_


#include <jni.h>
#include <osg/Vec3f>
template<class type>
class jSingleton
{
public:
	static type& getInstance(JNIEnv *env)
	{
		static type singles(env);
		return singles;
	}
protected:
	jSingleton() {}
	~jSingleton() {}
};

class jVector3 : public jSingleton<jVector3>
{
	typedef osg::Vec3f vec3;
public:
	

	void getVector3(JNIEnv* env, jobject& vec, vec3& vt);
	void setVector3(JNIEnv* env, jobject& vec, vec3& vt);

private:
	jVector3(JNIEnv* env);

	jfieldID mX;
	jfieldID mY;
	jfieldID mZ;
	
	friend class jSingleton<jVector3>;
};

class jDataPacket : public jSingleton<jDataPacket>
{
public:
	void getMatrixData(JNIEnv* env, jobject& pack, jfloatArray& data);
	void getColorData(JNIEnv* env, jobject& pack, jbyteArray& data);
	void getDepthData(JNIEnv* env, jobject& pack, jbyteArray& data, jint& size);

private:
	jDataPacket(JNIEnv* env);

	jfieldID mMat;
	jfieldID mCdata;
	jfieldID mDdata;
	jfieldID mDdstLen;

	friend class jSingleton<jDataPacket>;
};

class jViewResolution: public jSingleton<jViewResolution>
{
public:
	void get(JNIEnv* env, jobject& res, int& width, int& height);

private:

	jViewResolution(JNIEnv* env);

	jfieldID mWidth;
	jfieldID mHeight;

	friend class jSingleton<jViewResolution>;
};


class jMotionData : public jSingleton<jMotionData>
{
public:
	unsigned char getType(JNIEnv* env, jobject& md);

	void getPoint1(JNIEnv* env, jobject& md, float& x1, float& y1);

	void getPoint2(JNIEnv* env, jobject& md, float& x2, float& y2);

private:
	jMotionData(JNIEnv* env);

	jfieldID mType;
	jfieldID mX1;
	jfieldID mY1;
	jfieldID mX2;
	jfieldID mY2;

	friend class jSingleton<jMotionData>;
};

class jViewPerspective : public jSingleton<jViewPerspective>
{
public:
	void get(JNIEnv* env, jobject& pesp, float& fov, float& aspect, float& near, float& far);

private:
	jViewPerspective(JNIEnv* env);

	jfieldID mFov;
	jfieldID mAspect;
	jfieldID mNear;
	jfieldID mFar;

	friend class jSingleton<jViewPerspective>;
};

class jstdCamPos: public jSingleton<jstdCamPos>
{
	typedef osg::Vec3f  vec3;
public:
	void get(JNIEnv* env, jobject& pos, vec3& eye, vec3& center, vec3& up);
	void set(JNIEnv* env, jobject& pos, vec3& eye, vec3& center, vec3& up);
	//void set(JNIEnv* env, jobject& pos, vec3& eye, vec3& center, vec3& up);

private:
	jstdCamPos(JNIEnv* env);

	jfieldID mEye;
	jfieldID mCenter;
	jfieldID mUp;

	friend class jSingleton<jstdCamPos>;
};

#endif
