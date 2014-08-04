/*
*本地代码与Java层的接口类
*给出了系统自动生成的头文件com_example_vino_VinoNativeRender.h中接口实现
*创建年份：2013年6月
*作者：Jing
*/

#include "com_example_vino_VinoNativeRender.h"
#include <map>
#include "VinoRender.h"
#include "classUtility.h"
#include "GlobalInfo.h"

VinoRender gVinoRender;

//从Java层中获取全局参数，存储到本地代码
void Java_com_example_vino_VinoNativeRender_setGlobe
	(JNIEnv *env, jclass ths, jobjectArray ress, jobject perp, jobject pos, jint mode)
{
	int _mode=mode;
	gVinoRender.setInteractionMode(_mode);
	gVinoRender.createInteractionMode();
	int i;
	for(i = 0; i < 3; ++i)
	{
		jobject tmpRes = env->GetObjectArrayElement(ress, i);
		int tmpW, tmpH;
		jViewResolution::getInstance(env).get(env, tmpRes, tmpW, tmpH);
		switch(i)
		{
		case 0:
			//系统逻辑上的显示分辨率
			LOGI("show resolution is : %d*%d\n", tmpW, tmpH);
			GlobalInfo::getInstance().setShowRes(tmpW, tmpH);
			break;
		case 1:
			//参考帧的显示分辨率（刚开始想做个上采样，所以与深度图分辨率分开设置）
			LOGI("color resolution is : %d*%d\n", tmpW, tmpH);
			GlobalInfo::getInstance().setColorRes(tmpW, tmpH);
			break;
		case 2:
			//深度图的分辨率
			LOGI("depth resolution is : %d*%d\n", tmpW, tmpH);
			GlobalInfo::getInstance().setDepthRes(tmpW, tmpH);
			break;
		}
	}
	//视口信息
	float fov, aspect, near, far;
	jViewPerspective::getInstance(env).get(env, perp, fov, aspect, near, far);
	GlobalInfo::getInstance().setPerspective(fov, aspect, near, far);
	//视点初始信息
	osg::Vec3f eye, center, up;
	jstdCamPos::getInstance(env).get(env, pos, eye, center, up);
	GlobalInfo::getInstance().setStdCamPos(eye, center, up);
}

void Java_com_example_vino_VinoNativeRender_SetCamera
  (JNIEnv * env, jclass jc, jobject pos)
{
	if(gVinoRender._ic.mode==jInteraction::USER_DEFINE)
	{
		LOGI("setLookAt : \n");
		osg::Vec3f eye, center, up;
		jstdCamPos::getInstance(env).get(env, pos, eye, center, up);
		gVinoRender.setLookAt(eye,center,up);
	}
}


void Java_com_example_vino_VinoNativeRender_getCamera
  (JNIEnv * env, jclass js, jobject jo)
{
	if(gVinoRender._ic.mode==jInteraction::USER_DEFINE)
	{
		LOGI("getLookAt : \n");
		osg::Vec3f eye, center, up;
		gVinoRender.getCamera(eye,center, up);
		jstdCamPos::getInstance(env).set(env,jo,eye,center,up);
	}

}
/*jobject Java_com_example_vino_VinoNativeRender_getCamera
  (JNIEnv *, jclass pos)
{
		LOGI("getLookAt : \n");
		osg::Vec3f eye, center, up;
		gVinoRender.getCamera(eye,center, up);

}*/

//每当java层接收到参考帧数据时，发送到NDK层进行解压处理
void Java_com_example_vino_VinoNativeRender_setData
	(JNIEnv *env, jclass ths, jobject pack)
{
	jfloatArray matrix;
	jDataPacket::getInstance(env).getMatrixData(env, pack, matrix);

	jbyteArray color;
	jDataPacket::getInstance(env).getColorData(env, pack, color);

	jbyteArray depth;
	jint dstLen;
	jDataPacket::getInstance(env).getDepthData(env, pack, depth, dstLen);

	//深度重构在渲染线程中，所以需要设置锁
	pthread_mutex_t mutex = GlobalInfo::getInstance().getDepthImageBase().getMutex();

	pthread_mutex_lock(&mutex);
	//获取参考帧对应的视点矩阵
	jfloat* tmpMatrix = env->GetFloatArrayElements(matrix, 0);
	if(tmpMatrix != NULL)
	{
		GlobalInfo::getInstance().getDepthImageBase().setMatrix(tmpMatrix);

		env->ReleaseFloatArrayElements(matrix, tmpMatrix, 0);
	}

	//获取参考帧中颜色图像的数据

	//统计颜色图像解压时间
	double time1 = VinoRender::getCurrentTime();

	unsigned long srcColorSize = (unsigned long)env->GetArrayLength(color);
	jbyte * srcColorData =(jbyte *) env->GetByteArrayElements(color, NULL);
	if(srcColorData != NULL)
	{
		unsigned char* ucSrcColorData = (unsigned char*)srcColorData;
		GlobalInfo::getInstance().getDepthImageBase().setColorData(ucSrcColorData, srcColorSize);

		env->ReleaseByteArrayElements(color, srcColorData, 0);
	}
	double time2 = VinoRender::getCurrentTime();

	//LOGI("JPEG process time is %f", time2 - time1);

	//获取参考帧中深度图的数据（边采样点的形式）
	jbyte *srcDepthData = env->GetByteArrayElements(depth, NULL);
	if(srcDepthData != NULL)
	{
		char* cSrcDepthData = (char*)srcDepthData;

		GlobalInfo::getInstance().getDepthImageBase().setDepthData(cSrcDepthData, dstLen);

		env->ReleaseByteArrayElements(depth, srcDepthData, 0);
	}
	
	double time3 = VinoRender::getCurrentTime();
	//LOGI("DEPTH process time is %f", time3 - time2);


	pthread_mutex_unlock(&mutex);
}

//在创建本地环境时，将shader文件读入
//文件成对出现，包括vertex shader 和  fragment shader
//shader第一行为注释为功能介绍，在后续文件读入中作为不同shader的标示，采用map映射
//如://smooth，说明此shader的功能是进行平滑处理
void Java_com_example_vino_VinoNativeRender_onCreate
	(JNIEnv *env, jclass ths, jobjectArray srcs)
{
	jMotionData::getInstance(env);

	int srcCnt = env->GetArrayLength(srcs) / 2;
	gVinoRender.createProgramSize(srcCnt);
	for(int i = 0; i < srcCnt; ++i)
	{
		//一组shader编号相邻，在写shader时需要正确指定
		jstring vert = (jstring) env->GetObjectArrayElement(srcs, 2*i);
		jstring frag = (jstring) env->GetObjectArrayElement(srcs, 2*i + 1);
		
		//vertex shader(顶点着色器)
		const int vLen = env->GetStringLength(vert);
		const char* vSrc = env->GetStringUTFChars(vert, 0);

		//fragment shader（片元着色器）
		const int fLen = env->GetStringLength(frag);
		const char* fSrc = env->GetStringUTFChars(frag, 0);
		gVinoRender.insertProgramSrc(i, vSrc, fSrc, vLen, fLen);

		env->ReleaseStringUTFChars(frag, fSrc);
		env->ReleaseStringUTFChars(vert, vSrc);
	}
}


//初始化本地Render（真正的OpenGL ES 2.0环境）
//调用全局变量gVinoRender的接口
void Java_com_example_vino_VinoNativeRender_onInit
	(JNIEnv *env, jclass ths)
{
	gVinoRender.initRender();
}

//onUpdate接口实时接收Java层中用户交互的原始数据
//MotionData为交互过程中参数的屏幕坐标
//一方面通过此接口传给本地代码处理，另一方面以副本的形式传输给服务器同步
void Java_com_example_vino_VinoNativeRender_onUpdate
	(JNIEnv *env, jclass ths, jobject md)
{
	MotionData tmp;
	tmp._type  = jMotionData::getInstance(env).getType(env, md);
	jMotionData::getInstance(env).getPoint1(env, md, tmp._x1, tmp._y1);
	jMotionData::getInstance(env).getPoint2(env, md, tmp._x2, tmp._y2);
	gVinoRender.update(tmp);
};


//本地显示绘制，及两帧参考的3D Image Warping合成结果
void Java_com_example_vino_VinoNativeRender_onFrame
	(JNIEnv *env, jclass ths)
{
	double stt = VinoRender::getCurrentTime();
	gVinoRender.frame();
	double fsh = VinoRender::getCurrentTime();
	LOGI("Frame time is %f", fsh - stt);
}

//OpenGL ES 2.0环境下提供深度重构的接口
//此接口供DepthImageBase调用
void Java_com_example_vino_VinoNativeRender_onProcess
	(JNIEnv * env, jclass ths)
{
	

	if(GlobalInfo::getInstance().getDepthImageBase().isWithData())
	{
		double time1 = VinoRender::getCurrentTime();

		pthread_mutex_t mutex = GlobalInfo::getInstance().getDepthImageBase().getMutex();
		pthread_mutex_lock(&mutex);

		//将颜色图像送入OpenGL ES 2.0环境，直接存为纹理
		LOGI("process1~");
		unsigned char* color = GlobalInfo::getInstance().getDepthImageBase().getColorData();
		gVinoRender.setColorData(color);
		//参考帧视点，之后会送入shader
		LOGI("process2~");
		float* matrix = GlobalInfo::getInstance().getDepthImageBase().getMatixData();
		gVinoRender.setMatrixData(matrix);
		//深度图数据
		LOGI("process3~");
		float* sample;
		int cnt;
		GlobalInfo::getInstance().getDepthImageBase().getDepthData(sample, cnt);
		gVinoRender.setDepthData(sample, cnt);

		//送入数据后，更新DepthImageBase状态：无数据
		LOGI("process4~");
		GlobalInfo::getInstance().getDepthImageBase().withoutData();

		pthread_mutex_unlock(&mutex);

		double time2 = VinoRender::getCurrentTime();

		LOGI("process time is %f", time2 - time1);
	}
	

	//Java_com_example_vino_VinoNativeRender_onFrame(env, ths);

	//double time3 = VinoRender::getCurrentTime();

	//LOGI("process duration is %f",time3 - time2);
}

//与onInit对应，销毁OpenGL ES 2.0环境
void Java_com_example_vino_VinoNativeRender_onDestory
	(JNIEnv *env, jclass ths)
{
	gVinoRender.destroyRender();
}
