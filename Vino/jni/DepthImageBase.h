#ifndef _customized_depth_image_base_h_
#define  _customized_depth_image_base_h_

#include <pthread.h>
struct HeadInfo
{
	int		index;
	bool		nextBlock;
	bool		isOne;
	HeadInfo()
		:index(0),
		nextBlock(false),
		isOne(false)
	{}
};

class DepthImageBase
{
	typedef unsigned char uchar;
public:

	static const int BSIZE = 32;
	static const int MAX = (1 << 16) - 1;

	DepthImageBase();

	~DepthImageBase();


	inline bool isWithData()
	{
		return(mMflag && mCflag && mDflag);
	}

	inline void withoutData()
	{
		mMflag = false;
		mCflag = false;
		mDflag = false;
	}

	inline void setMatrix(float *mat)
	{
		for(int i = 0; i < 16; ++i)
			mMatrix[i] = mat[i];
		mMflag = true;
	}

	inline float* getMatixData()
	{
		return mMatrix;
	}

	inline unsigned char *getColorData()
	{
		return mColor;
	}

	inline void getDepthData(float *&data, int &cnt)
	{
		data = mSample;
		cnt = mSampleCnt;
	}

	inline pthread_mutex_t& getMutex()
	{
		return mMutex;
	}

	void setColorData(uchar *srcData, int srcSize);

	void setDepthData(const char *srcData, int dstSize);


private:
	void reSizeColor(int cnt);

	void reSizeSampleCode(int cnt);

	void reSizeSample(int cnt);

	float toDepthValue(uchar *data);

	HeadInfo getHeadInfo(uchar *head, bool half);

	int getLocalInt(uchar* data);

	float mMatrix[16];

	unsigned char * mColor;
	int mColorSize;

	uchar *mSampleCode;
	int mSampleCodeSize;

	float *mSample;
	int mSampleSize;
	int mSampleCnt;

	bool		mCflag;
	bool		mDflag;
	bool		mMflag;

	pthread_mutex_t  mMutex;
};
#endif