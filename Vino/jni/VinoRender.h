#ifndef  __VINO_RENDER__
#define __VINO_RENDER__

#include "Manipulator.h"
#include "CFirstPersonManipulator.h"
#include "GlobalInfo.h"
#include <string>
#include <map>


#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include "GlobalInfo.h"
#include "ManipulatorInterface.h"
#include "CameraOperator.h"



typedef unsigned char  vUchar;

struct prgSources
{
	char* vert;
	char* frag;
};
//2014_7_12
class jInteraction
{
public:
	jInteraction();
	static const int TRACKBALLMANIPULATOR = 0;
	static const int FIRSTPERSONMANIPULATOR =1;
	static const int USER_DEFINE=2;
	int mode;
};
class VinoRender
{
public:
	static const float squareVertices[];
	static const int offset1[];
	static const int offset2[];
	static const int offset3[];
	static const int gaussionKernel[];

	static const int  BUFFERSIZE = 2;

	 jInteraction _ic;//2014_7_12

	VinoRender();

	~VinoRender();
	static double getCurrentTime();

	static void printGLString(const char *name, GLenum s);

	static void checkGlError(const char* op);

	inline bool createProgramSize(int size)
	{
		mShdSrcSize = size;
		mShdSrcs = (prgSources*)malloc(sizeof(prgSources) * size);
		if(!mShdSrcs) return false;
		return true;
	}
	bool insertProgramSrc(int idx, const char * vert, const char *frag, int vLen, int fLen);

	void initRender();

	//2014_8_4
	void setLookAt(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up);
	//2014_8_4
	void getCamera(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up);

	void destroyRender();

	void preRecovery(const float* data, int size);

	void postRecovery();
	
	void setDepthData(const float* data, int size);

	void setColorData(const vUchar* data);

	void setMatrixData(const float* data);

	void testRender();

	void frame();

	void update(MotionData& md);
	void setInteractionMode(int _m);
	void createInteractionMode();

protected:

	GLuint loadShader(GLenum shaderType, const char* pSource);

	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

	void drawQuad(GLuint pos, GLuint texCoord = 0);

	void drawWarpingPoint(GLuint pos);

	void createTexture(GLuint& texture, int width, int height, GLenum format, const GLvoid *data, GLenum filter = GL_NEAREST);

	void destroyTexture(GLsizei n, const GLuint *texture);

	void setNextBuffer();



private:

	void setPushUniform();
	void setSmoothUniform();
	void setPullUniform();
	void setTestUniform();
	void setWarpTestUniform();


	typedef	std::map<std::string, GLuint> ShaderMap;
	typedef ShaderMap::iterator  ShdMapIter;
	
	//Manipulator mMplator;
	//CFirstPersonManipulator mMplator;

	ManipulatorInterface * mMplator;

	Resolution mShowRes;
	Resolution mColorRes;
	Resolution mDepthRes;
	Resolution mTestRes;

	prgSources *mShdSrcs;
	int mShdSrcSize;

	ShaderMap  mShdMap;

	GLuint mWarpVBO;
	GLuint mQuadVBO;

	GLuint mProcFBO;
	GLuint mWarpFB0;
	GLuint mDepthBuffer;

	GLuint mTestTex;
	GLuint mResultTex;

 	GLuint mPushTex[6];
 	GLuint mPullTex[6];
 	GLuint mSmthTex[6];

	GLuint mColorTexs[BUFFERSIZE];
	GLuint mDepthTexs[BUFFERSIZE];
	osg::Matrixf mMatrix[BUFFERSIZE];
	bool mUsed[BUFFERSIZE];
	int mBufferTop;
	int mNow;

	bool mFinishRecovery;
	bool mPreDone;
	bool mFirstData;


	float mNear, mFar;

	osg::Matrixf mProjMatrix;

	Resolution mLevelRes[6];
};

#endif
