#include "VinoRender.h"

#include "CameraOperator.h"
using std::string;
using std::pair;

jInteraction::jInteraction()
{
	mode=jInteraction::TRACKBALLMANIPULATOR;
}
void VinoRender::setInteractionMode(int _m)
{
	_ic.mode=_m;
}
void VinoRender::createInteractionMode()
{

	if(mMplator!= NULL)
	{
		delete mMplator;
	}
	if(_ic.mode == jInteraction::TRACKBALLMANIPULATOR)
	{
		mMplator=new Manipulator();
	}else if(_ic.mode == jInteraction::FIRSTPERSONMANIPULATOR)
	{
		mMplator=new CFirstPersonManipulator();
	}else if(_ic.mode == jInteraction::USER_DEFINE)
	{
		mMplator=new CameraOperator();
	}
}

const float VinoRender::squareVertices[] = {
	-1.0f, -1.0f,
	0.0f, 0.0f,
	1.0f, -1.0f,
	1.0f, 0.0f,
	-1.0f,  1.0f,
	0.0f, 1.0f,
	1.0f,  1.0f,
	1.0f, 1.0f
};
const int VinoRender::offset1[] = {
	-1,	  -1,
	1,	-1,
	-1,	  1,
	1,  1
};
const int VinoRender::offset2[] ={
	-2, -2, -2, -1, -2, 0, -2, 1,-2, 2,
	-1, 2, -1, -1, -1, 0, -1, 1,-1, 2,
	0, 2,0, -1, 0, 0, 0, 1, 0, 2,
	1, 2,1, -1, 1, 0, 1, 1,1, 2,
	2, -2, 2, -1, 2, 0, 2, 1, 2, 2
};
const int VinoRender::offset3[] ={
	0, -1,
	-1, 0,
	0, 1,
	1, 0
};
const int VinoRender::gaussionKernel[] ={
	1, 4, 7, 4, 1,
	4, 16, 26, 16, 4,
	7, 26, 41, 26, 7,
	4, 16, 26, 16, 4,
	1, 4, 7, 4, 1
};

VinoRender::VinoRender()
	:mShdSrcs(NULL),
	mShdSrcSize(0),
	mWarpVBO(0),
	mProcFBO(0),
	mBufferTop(0),
	mFinishRecovery(true),
	mPreDone(true),
	mFirstData(true),
	mNow(-1)
{
	createInteractionMode();//2014_7_12
}

VinoRender::~VinoRender()
{
	destroyRender();
	delete mMplator;
}
//2014_8_4
void VinoRender::setLookAt(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up)
{

	if(mMplator!=NULL)
	{
		mMplator->setByLookAt(eye,center,up);
	}
}
void VinoRender::getCamera(osg::Vec3f& eye, osg::Vec3f& center, osg::Vec3f& up)
{
		if(mMplator!=NULL)
		{
			mMplator->getCamera(eye,center,up);
		}
}
double VinoRender::getCurrentTime()
{
	struct timespec res;
	clock_gettime(CLOCK_REALTIME, &res);
	return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

void VinoRender::printGLString(const char *name, GLenum s)
{
	const char *v = (const char *) glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}

void VinoRender::checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error
		= glGetError()) {
			LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

bool VinoRender::insertProgramSrc(int idx, const char * vert, const char *frag, int vLen, int fLen)
{
	mShdSrcs[idx].vert = (char*)malloc(sizeof(char) * (vLen + 1));
	if(!mShdSrcs[idx].vert) return false;
	mShdSrcs[idx].frag = (char*)malloc(sizeof(char) * (fLen + 1));
	if(!mShdSrcs[idx].frag)
	{
		free(mShdSrcs[idx].vert);
		mShdSrcs[idx].vert = NULL;
		return false;
	}
	LOGI("create shader %d  sucessfully", idx);
	int i;
	for(i = 0; i < vLen; ++i) mShdSrcs[idx].vert[i] = vert[i];
	mShdSrcs[idx].vert[i] = '\0';
	for(i = 0; i < fLen; ++i) mShdSrcs[idx].frag[i] = frag[i];
	mShdSrcs[idx].frag[i] ='\0';
}

void VinoRender::initRender()
{

	//int texture_units;
	//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	
// 	int maxSize;
// 	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
// 	LOGI("max texture size  is %d\n", maxSize);
// 	int maxRenderbufferSize;
// 	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
// 	LOGI("max render buffer size is %d",maxRenderbufferSize );
	//LOGI("maximum texture is %d\n", texture_units);
	//GLint mOldFBO;
	//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFBO);
	//LOGI("old FBO number is %d\n", mOldFBO);
// 	int rang[2], precision;
// 	glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT,rang, &precision);
// 	LOGI("rang is %d, %d;pricison is %d\n", rang[0],rang[1], precision);

	stdCamPos pos = GlobalInfo::getInstance().getStdCamPos();
	//mMplator.setByLookAt(pos.eye, pos.center, pos.up);
	mMplator->setByLookAt(pos.eye, pos.center, pos.up);
	Perspective psp = GlobalInfo::getInstance().getPerspective();
	mProjMatrix.makePerspective(psp.fov, psp.aspect, psp.near, psp.far * 1.5);
	mNear = psp.near;
	mFar = psp.far;

	mShowRes = GlobalInfo::getInstance().getShowRes();
	mColorRes = GlobalInfo::getInstance().getColorRes();
	mDepthRes = GlobalInfo::getInstance().getDepthRes();

	//VBO warping position data
	int dataSize = sizeof(float) * mDepthRes.w * mDepthRes.h * 2;
	float* pointIdx = (float*)malloc(dataSize);
	if(pointIdx)
	{
		int s, t;
		for(t = 0; t < mDepthRes.h; ++t)
			for(s = 0; s < mDepthRes.w; ++s)
			{
				int pos = t * mDepthRes.w + s;
				pointIdx[2*pos] = s + 0.5;
				pointIdx[2*pos + 1] = t + 0.5;
			}
		glGenBuffers(1, &mWarpVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mWarpVBO);
		glBufferData(GL_ARRAY_BUFFER, dataSize, pointIdx, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);/////?????
		LOGI("warp VBO number is %d\n", mWarpVBO);
		free(pointIdx);
	}

	//VBO quad data
	glGenBuffers(1, &mQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, squareVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	LOGI("quad VBO number is %d\n", mQuadVBO);

	//shader sources
	int i;
	for(i = 0; i < mShdSrcSize; i++)
	{
		string sName;
		const char *p = mShdSrcs[i].vert;
		int pos = 2;
		while(p[pos] != '\n')
			sName.insert(sName.end(), p[pos++]);
		int tt = createProgram(mShdSrcs[i].vert, mShdSrcs[i].frag);
		LOGI("shader name is %s\n", sName.c_str());
		LOGI("shader num is %d\n", tt);
		mShdMap[sName] = tt;
	}
	setPushUniform();
	setSmoothUniform();
	setPullUniform();
	setWarpTestUniform();
	//FBO
	glGenFramebuffers(1, &mProcFBO);
	LOGI("FBO number is %d\n", mProcFBO);

	mLevelRes[0] = mDepthRes;
	for(i = 1; i < 6; ++i)
	{
		mLevelRes[i].w = (mLevelRes[i - 1].w + 1) / 2;
		mLevelRes[i].h = (mLevelRes[i - 1].h + 1)  / 2;
		LOGI("%d:%d", mLevelRes[i].w, mLevelRes[i].h);
	}

	for(i = 5; i > 0; --i)
		createTexture(
		mPullTex[i], mLevelRes[i].w,mLevelRes[i].h,
		GL_RGBA, NULL
		);
	for(i = 0; i < 5; ++i)
		createTexture(
		mPushTex[i], mLevelRes[i].w, mLevelRes[i].h,
		GL_RGBA, NULL
		);

	mPushTex[5] = mPullTex[5];

	for(i = 5; i > 0; --i)
		createTexture(
		mSmthTex[i],mLevelRes[i].w, mLevelRes[i].h,
		GL_RGBA, NULL
		);
	for(i = 0; i < BUFFERSIZE; ++i)
	{
		createTexture(
		mDepthTexs[i], mDepthRes.w, mDepthRes.h,
		GL_RGBA, NULL
		);		
		createTexture(
			mColorTexs[i], mColorRes.w, mColorRes.h,
			GL_RGB, NULL, GL_LINEAR
			);
		mUsed[i] = false;
	}
	mBufferTop = 0;
	createTexture(mResultTex, mShowRes.w, mShowRes.h, GL_RGB, NULL, GL_LINEAR);
//  	mTestRes = mLevelRes[0];
//  	glGenTextures(1, &mTestTex);
// 	glBindTexture(GL_TEXTURE_2D, mTestTex);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 	glTexImage2D( GL_TEXTURE_2D,
// 		0,
// 		GL_RGBA,
// 		mTestRes.w,
// 		mTestRes.h,
// 		0,
// 		GL_RGBA,
// 		GL_UNSIGNED_BYTE,
// 		NULL);
// 	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &mWarpFB0);
	glBindFramebuffer(GL_FRAMEBUFFER, mWarpFB0);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		mResultTex,
		0
		);
	glGenRenderbuffers(1, &mDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mShowRes.w, mShowRes.h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VinoRender::setPushUniform()
{
	int tmpShd = mShdMap["push"];
	glUseProgram(tmpShd);
	GLuint osName = glGetUniformLocation(tmpShd, "offset[0]");
	glUniform2iv(osName, 4,offset1);
	glUseProgram(0);
}

void VinoRender::setSmoothUniform()
{
	int tmpShd = mShdMap["smooth"];
	glUseProgram(tmpShd);
	GLuint osName = glGetUniformLocation(tmpShd, "offset");
	glUniform2iv(osName, 25, offset2);
	GLuint klName = glGetUniformLocation(tmpShd, "kernel");
	glUniform1iv(klName, 25, gaussionKernel);
	glUseProgram(0);
}

void VinoRender::setPullUniform()
{
	int tmpShd = mShdMap["pull"];
	glUseProgram(tmpShd);
	GLuint os1Name = glGetUniformLocation(tmpShd, "offset1[0]");
	glUniform2iv(os1Name, 4, offset1);
	GLuint os2Name = glGetUniformLocation(tmpShd, "offset2[0]");
	glUniform2iv(os2Name, 4, offset3);
	glUseProgram(0);
}
void VinoRender::setTestUniform()
{
	glUseProgram(mShdMap["test"]);
	glUseProgram(0);
}

void VinoRender::setWarpTestUniform()
{
	int tmpShd = mShdMap["warpTest"];
	glUseProgram(tmpShd);
	GLuint size = glGetUniformLocation(tmpShd, "depthSize");
	glUniform2f(size, float(mDepthRes.w), float(mDepthRes.h));
	glUseProgram(0);
}



void VinoRender::destroyRender()
{

	glDeleteTextures(BUFFERSIZE, mDepthTexs);
	glDeleteTextures(5, &mPullTex[1]);
	glDeleteTextures(5, &mSmthTex[1]);
	glDeleteTextures(6, mPushTex);


	glDeleteRenderbuffers(1, &mDepthBuffer);
	glDeleteFramebuffers(1, &mWarpFB0);
	glDeleteFramebuffers(1, &mProcFBO);
	mWarpVBO = 0;

	ShdMapIter iter;
	for(iter = mShdMap.begin(); iter != mShdMap.end(); ++iter)
		glDeleteProgram(iter->second);
	int i;
	for(i = 0; i < mShdSrcSize; ++i)
	{
		free(mShdSrcs[i].vert);
		mShdSrcs[i].vert = NULL;
		free(mShdSrcs[i].frag);
		mShdSrcs[i].frag = NULL;
	}
	free(mShdSrcs);
	mShdSrcs = NULL;
	mShdSrcSize = 0;

	mShdMap.clear();
}

void VinoRender::createTexture(GLuint& texture, int width, int height, GLenum format, const GLvoid *data, GLenum filter)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D( GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void VinoRender::destroyTexture(GLsizei n, const GLuint *texture)
{
	glDeleteTextures(n, texture);
}

GLuint VinoRender::loadShader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n",
						shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}



GLuint VinoRender::createProgram(const char* pVertexSource, const char* pFragmentSource)
{
	LOGI("Load shader~");
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
	return program;
}


void VinoRender::drawQuad(GLuint pos, GLuint texCoord)
{
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 16, 0);
	glEnableVertexAttribArray(pos);
	if(texCoord)
	{
		glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 16, (GLvoid*)8);
		glEnableVertexAttribArray(texCoord);
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(pos);
	if(texCoord)
	{
		glDisableVertexAttribArray(texCoord);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VinoRender::drawWarpingPoint(GLuint pos)
{
	glBindBuffer(GL_ARRAY_BUFFER, mWarpVBO);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(pos);
	glDrawArrays(GL_POINTS, 0, mDepthRes.w * mDepthRes.h);
	glDisableVertexAttribArray(pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VinoRender::testRender()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, mProcFBO);
// 	glFramebufferTexture2D(
// 		GL_FRAMEBUFFER, 
// 		GL_COLOR_ATTACHMENT0,
// 		GL_TEXTURE_2D,
// 		mTestTex,
// 		0
// 		);
//  		glViewport(0, 0, mTestRes.w, mTestRes.h);
//  		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//  		glClear( GL_COLOR_BUFFER_BIT);
// 	GLuint shdNum = mShdMap["test"];
// 	glUseProgram(shdNum);
// 	GLuint edgePosition = glGetAttribLocation(shdNum, "aPosition");
// 	drawQuad(edgePosition);
// 	glUseProgram(0);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 
// 
// 	glViewport(0, 0, mTestRes.w , mTestRes.h);
// 	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
// 	glClear( GL_COLOR_BUFFER_BIT);
// 	shdNum = mShdMap["show"];
// 	glUseProgram(shdNum);
// 	GLuint showTex = glGetUniformLocation(shdNum, "showTex");
// 	GLuint showSize = glGetUniformLocation(shdNum, "showSize");
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, mTestTex);
// 	glUniform1i(showTex, 0);
// 	glUniform2f(showSize, float(mTestRes.w), float(mTestRes.h));
// 	GLuint quadPos = glGetAttribLocation(shdNum, "aPosition");
// 	drawQuad(quadPos);
// 	glUseProgram(0);
}


void VinoRender::setDepthData(const float* data, int size)
{
	LOGI("test1~");
	if(mFirstData)
	{
		preRecovery(data, size);
		postRecovery();
		setNextBuffer();
		mFirstData = false;
		return;
	}
	LOGI("test2~");
	if(!mFinishRecovery)
	{
		postRecovery();
		setNextBuffer();
	}
	preRecovery(data, size);
	mFinishRecovery = false;
	mPreDone = true;
	LOGI("test3~");
}

void VinoRender::setColorData(const vUchar* data)
{
	LOGI("yesyes1~");
	glBindTexture(GL_TEXTURE_2D, mColorTexs[mBufferTop]);
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		mColorRes.w,
		mColorRes.h,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data
		);
	glBindTexture(GL_TEXTURE_2D, 0);
	LOGI("yesyes2~");
}

void VinoRender::setMatrixData(const float* data)
{
	mMatrix[mBufferTop].set(data);
}

void VinoRender::frame()
{
	if(!mFinishRecovery && !mPreDone)
	{
		postRecovery();
		setNextBuffer();
		mFinishRecovery = true;
	}
	mPreDone = false;

// 	GLuint shdNum = mShdMap["warpTest"];
// 	glEnable(GL_DEPTH_TEST);
// 	glUseProgram(shdNum);
// 
// 	glViewport(0, 0, mShowRes.w, mShowRes.h);
// 	glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
// 	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 	GLuint wpMat  = glGetUniformLocation(shdNum, "wpMat");
// 	glUniformMatrix4fv(wpMat, 1, GL_FALSE, mMatrix[mNow].ptr());
// 	
// 	GLuint MVP = glGetUniformLocation(shdNum, "MVP");
// 	osg::Matrixf tmvp = mMplator.getInverseMatrix();
// 	tmvp *= mProjMatrix;
// 	glUniformMatrix4fv(MVP, 1, GL_FALSE, tmvp.ptr());
// 	
// 	GLuint depthTex = glGetUniformLocation(shdNum, "depthTex");
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, mDepthTexs[mNow]);
// 	glUniform1i(depthTex, 0);
// 	
// 	GLuint colorTex = glGetUniformLocation(shdNum, "colorTex");
// 	glActiveTexture(GL_TEXTURE1);
// 	glBindTexture(GL_TEXTURE_2D, mColorTexs[mNow]);
// 	glUniform1i(colorTex, 1);
// 
// 	GLuint warpingPos = glGetAttribLocation(shdNum, "aPosition");
// 	drawWarpingPoint(warpingPos);
// 	glUseProgram(0);
// 	glDisable(GL_DEPTH_TEST);
		glClearColor(0.153, 0.659, 0.843, 1.0);
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, mWarpFB0);
		glViewport(0, 0, mShowRes.w, mShowRes.h);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLuint shdNum = mShdMap["warpTest"];
		glUseProgram(shdNum);

		GLuint MVP = glGetUniformLocation(shdNum, "MVP");
		//osg::Matrixf tmvp = mMplator.getInverseMatrix();
		osg::Matrixf tmvp = mMplator->getInverseMatrix();
		tmvp *= mProjMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, tmvp.ptr());

		GLuint warpingPos = glGetAttribLocation(shdNum, "aPosition");
		for(int i = 1; i <= BUFFERSIZE; i++)
		{
				int nowBuffer = (mNow + i) % BUFFERSIZE;
				if(mUsed[nowBuffer])
				{
					GLuint wpMat  = glGetUniformLocation(shdNum, "wpMat");
					glUniformMatrix4fv(wpMat, 1, GL_FALSE, mMatrix[nowBuffer].ptr());


					GLuint depthTex = glGetUniformLocation(shdNum, "depthTex");
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mDepthTexs[nowBuffer]);
					glUniform1i(depthTex, 0);

					GLuint colorTex = glGetUniformLocation(shdNum, "colorTex");
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, mColorTexs[nowBuffer]);
					glUniform1i(colorTex, 1);

					drawWarpingPoint(warpingPos);
				}
		}
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		int maxWidth = 768, maxHeight = 1038;
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glViewport(0, 0, maxWidth, maxHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		shdNum = mShdMap["show"];
		glUseProgram(shdNum);
		GLuint showTex = glGetUniformLocation(shdNum, "showTex");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mResultTex);
		glUniform1i(showTex, 0);

		GLuint showPos = glGetAttribLocation(shdNum, "aPosition");
		GLuint texCoord = glGetAttribLocation(shdNum, "texCoord");
		drawQuad(showPos, texCoord);
		glUseProgram(0);
		//glFinish();
}

void VinoRender::update(MotionData& md)
{
	int width = 768;
	int height = 1024;
	md._x1 = (2 * md._x1 - width) / width;
	md._y1 = (height - 2 * md._y1) / height;
	md._x2 = (2 * md._x2 - width) / width;
	md._y2 = (height - 2 * md._y2) / height;
	//mMplator.handle(md._x1,md._y1);
	//mMplator.handle(md);
	mMplator->handle(md);
}

void VinoRender::setNextBuffer()
{
	mNow = mBufferTop;
	mUsed[mBufferTop] = true;
	mBufferTop = (mBufferTop + 1) % BUFFERSIZE;
}

void VinoRender::preRecovery(const float* data, int size)
{
	int i;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, mProcFBO);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		mPushTex[0],
		0
		);
	glViewport(0, 0, mLevelRes[0].w, mLevelRes[0].h);
	glClear( GL_COLOR_BUFFER_BIT);

	GLuint shdNum = mShdMap["edge"];
	glUseProgram(shdNum);
	GLuint edgePosition = glGetAttribLocation(shdNum, "aPosition");

	glVertexAttribPointer(edgePosition, 3, GL_FLOAT, GL_FALSE, 0, data);
	glEnableVertexAttribArray(edgePosition);
	glDrawArrays(GL_POINTS, 0, size);
	glDisableVertexAttribArray(edgePosition);
	glUseProgram(0);

	shdNum = mShdMap["push"];
	glUseProgram(shdNum);
	for(i = 1; i < 6; ++i)
	{
		glViewport(0, 0, mLevelRes[i].w, mLevelRes[i].h);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			mPushTex[i],
			0
			);
		glClear( GL_COLOR_BUFFER_BIT);

		GLuint preSize  = glGetUniformLocation(shdNum, "preSize");
		glUniform2f(preSize, float(mLevelRes[i-1].w), float(mLevelRes[i-1].h));
		GLuint preTex = glGetUniformLocation(shdNum, "preTex");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mPushTex[i -1]);
		glUniform1i(preTex, 0);
		GLuint pushPosition = glGetAttribLocation(shdNum, "aPosition");
		drawQuad(pushPosition);
	}
	glUseProgram(0);

	shdNum = mShdMap["pull"];
	glUseProgram(shdNum);
	for(i = 4; i >= 1; --i)
	{
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			mPullTex[i],
			0
			);
		glViewport(0, 0, mLevelRes[i].w, mLevelRes[i].h);
		glClear(GL_COLOR_BUFFER_BIT);
		GLuint curSize = glGetUniformLocation(shdNum, "curSize");
		glUniform2f(curSize, float(mLevelRes[i].w), float(mLevelRes[i].h));
		GLuint curTex = glGetUniformLocation(shdNum, "curTex");
		GLuint preTex = glGetUniformLocation(shdNum, "preTex");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mPushTex[i]);
		glUniform1i(curTex, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mPullTex[i + 1]);
		glUniform1i(preTex, 1);
		GLuint plPosition = glGetAttribLocation(shdNum, "aPosition");
		drawQuad(plPosition);
	}
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VinoRender::postRecovery()
{
	mPullTex[0] = mDepthTexs[mBufferTop];
	glBindFramebuffer(GL_FRAMEBUFFER, mProcFBO);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		mPullTex[0],
		0
		);
	GLuint shdNum = mShdMap["pull"];
	glUseProgram(shdNum);
	glViewport(0, 0, mLevelRes[0].w, mLevelRes[0].h);
	glClear(GL_COLOR_BUFFER_BIT);
	GLuint curSize = glGetUniformLocation(shdNum, "curSize");
	glUniform2f(curSize, float(mLevelRes[0].w), float(mLevelRes[0].h));
	GLuint curTex = glGetUniformLocation(shdNum, "curTex");
	GLuint preTex = glGetUniformLocation(shdNum, "preTex");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mPushTex[0]);
	glUniform1i(curTex, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mPullTex[1]);
	glUniform1i(preTex, 1);
	GLuint plPosition = glGetAttribLocation(shdNum, "aPosition");
	drawQuad(plPosition);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

