/*
*本地代码读取Java层中的数据（类形式，通过虚拟机分配的内存）
*通过env环境变量提供的接口进行操作
*NDK可以直接在本地进行内存操作（与Java层独立），也可以通过env在Java层进行内存操作。
*详情请参考JNI技术
********************************
*本类针对Java层中不同的类对env提供的操作进行了封装，采用Singleton模式
*创建年份：2013年6月
*作者：Jing
*/
#include "classUtility.h"

//jVector3对应java中的Vector3类
//通过此类读取Java层传入的Vector3数据
//下同
jVector3::jVector3(JNIEnv* env)
{
	jclass tmpClass = env->FindClass("com/example/vino/Vector3");
	mX = env->GetFieldID(tmpClass, "x", "F");
	mY = env->GetFieldID(tmpClass, "y", "F");
	mZ = env->GetFieldID(tmpClass, "z", "F");
}
void jVector3::setVector3(JNIEnv* env, jobject& vec, vec3& vt)
{
	env->SetFloatField(vec, mX, vt._v[0]);
	env->SetFloatField(vec,mY,vt._v[1]);
	env->SetFloatField(vec,mZ,vt._v[2]);
}
void jVector3::getVector3(JNIEnv* env, jobject& vec, vec3& vt)
{
	vt._v[0] = env->GetFloatField(vec, mX);
	vt._v[1] = env->GetFloatField(vec, mY);
	vt._v[2] = env->GetFloatField(vec, mZ);
}


jDataPacket::jDataPacket(JNIEnv* env)
{
	jclass tmpClass = env->FindClass("com/example/vino/DataPacket");

	mMat = env->GetFieldID(tmpClass, "_mat", "[F");
	mCdata = env->GetFieldID(tmpClass, "_cData", "[B");
	mDdata = env->GetFieldID(tmpClass, "_dData", "[B");
	mDdstLen = env->GetFieldID(tmpClass, "_dSrcLen", "I");
}

void jDataPacket::getMatrixData(JNIEnv* env, jobject& pack, jfloatArray& data)
{
	data =(jfloatArray) env->GetObjectField(pack, mMat);
}

void jDataPacket::getColorData(JNIEnv* env, jobject& pack, jbyteArray& data)
{
	data = (jbyteArray) env->GetObjectField(pack, mCdata);
}

void jDataPacket::getDepthData(JNIEnv* env, jobject& pack, jbyteArray& data, jint& size)
{
	data = (jbyteArray)env->GetObjectField(pack, mDdata);
	size = env->GetIntField(pack, mDdstLen);
}


jViewResolution::jViewResolution(JNIEnv* env)
{
	jclass tmpClass = env->FindClass("com/example/vino/ViewResolution");
	mWidth = env->GetFieldID(tmpClass, "_width", "I");
	mHeight = env->GetFieldID(tmpClass, "_height", "I");
}

void jViewResolution::get(JNIEnv* env, jobject& res, int& width, int& height)
{
	width = env->GetIntField(res, mWidth);
	height = env->GetIntField(res, mHeight);
}



jMotionData::jMotionData(JNIEnv* env)
{
	jclass tmpClass = env->FindClass("com/example/vino/MotionData");
	mType = env->GetFieldID(tmpClass, "_type", "B");
	mX1 = env->GetFieldID(tmpClass, "_x1", "F");
	mY1 = env->GetFieldID(tmpClass, "_y1", "F");
	mX2 = env->GetFieldID(tmpClass, "_x2", "F");
	mY2 = env->GetFieldID(tmpClass, "_y2", "F");
}

unsigned char jMotionData::getType(JNIEnv* env, jobject& md)
{
	return((unsigned char)env->GetByteField(md, mType));
}

void jMotionData::getPoint1(JNIEnv* env, jobject& md, float& x1, float& y1)
{
	x1 = env->GetFloatField(md, mX1);
	y1 = env->GetFloatField(md, mY1);
}
void jMotionData::getPoint2(JNIEnv* env, jobject& md, float& x2, float& y2)
{
	x2 = env->GetFloatField(md, mX2);
	y2 = env->GetFloatField(md, mY2);
}




jViewPerspective::jViewPerspective(JNIEnv* env)
{
	jclass tmpClass = env->FindClass("com/example/vino/ViewPerspective");
	mFov = env->GetFieldID(tmpClass, "_fov", "F");
	mAspect = env->GetFieldID(tmpClass, "_aspect", "F");
	mNear = env->GetFieldID(tmpClass, "_near", "F");
	mFar = env->GetFieldID(tmpClass, "_far", "F");
}

void jViewPerspective::get(JNIEnv* env, jobject& pesp, float& fov, float& aspect, float& near, float& far)
{
	fov = env->GetFloatField(pesp, mFov);
	aspect = env->GetFloatField(pesp, mAspect);
	near = env->GetFloatField(pesp, mNear);
	far = env->GetFloatField(pesp, mFar);
}



jstdCamPos::jstdCamPos(JNIEnv* env)
{
	jclass tmpClass = env->FindClass("com/example/vino/stdCamPos");
	mEye= env->GetFieldID(tmpClass, "_eye", "Lcom/example/vino/Vector3;");
	mCenter = env->GetFieldID(tmpClass, "_center", "Lcom/example/vino/Vector3;");
	mUp = env->GetFieldID(tmpClass, "_up", "Lcom/example/vino/Vector3;");
}

void jstdCamPos::get(JNIEnv* env, jobject& pos, vec3& eye, vec3& center, vec3& up)
{
	jobject jeye = env->GetObjectField(pos, mEye);
	jobject jcenter = env->GetObjectField(pos, mCenter);
	jobject jup = env->GetObjectField(pos, mUp);
	jVector3::getInstance(env).getVector3(env, jeye, eye);
	jVector3::getInstance(env).getVector3(env, jcenter, center);
	jVector3::getInstance(env).getVector3(env, jup, up);
}
void jstdCamPos::set(JNIEnv* env, jobject& pos, vec3& eye, vec3& center, vec3& up)
{
		jobject jeye = env->GetObjectField(pos, mEye);
		jobject jcenter = env->GetObjectField(pos, mCenter);
		jobject jup = env->GetObjectField(pos, mUp);

		jVector3::getInstance(env).setVector3(env, jeye, eye);
		jVector3::getInstance(env).setVector3(env, jcenter, center);
		jVector3::getInstance(env).setVector3(env, jup, up);

}
/*void jstdCamPos::set(JNIEnv* env, vec3& eye, vec3& center, vec3& up)
{

}*/
