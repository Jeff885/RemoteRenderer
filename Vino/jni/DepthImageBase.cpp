#include "DepthImageBase.h"
#include <iostream>
#include "libjpeg/turbojpeg.h"
#include "quicklz/quicklz.h"
#include <string.h>
#include "GlobalInfo.h"


DepthImageBase::DepthImageBase()
	:mColor(NULL),
	mColorSize(-1),
	mSampleCode(NULL),
	mSampleCodeSize(-1),
	mSample(NULL),
	mSampleSize(-1),
	mSampleCnt(0),
	mCflag(false),
	mDflag(false),
	mMflag(false)
{

}

DepthImageBase::~DepthImageBase()
{
	delete[] mColor;
	mColor = NULL;

	delete[] mSampleCode;
	mSampleCode = NULL;

	delete[] mSample;
	mSample = NULL;
}

void DepthImageBase::setColorData(uchar *srcData, int srcSize)
{
	tjhandle dhandle = tjInitDecompress();
	if(dhandle == NULL) return;

	int width = 0, height = 0;
	int jpegSubsamp;
	int flag = tjDecompressHeader2(
		dhandle,
		srcData,
		srcSize,
		&width,
		&height,
		&jpegSubsamp
		);
	if(flag == -1)
	{
		tjDestroy(dhandle);
		return;
	}

	if(mColorSize != width * height * 3) reSizeColor(width * height * 3);

	flag = tjDecompress2(
		dhandle,
		srcData,
		srcSize,
		mColor,
		width,
		tjPixelSize[TJPF_RGB]*width,
		height,
		TJPF_RGB,
		TJFLAG_BOTTOMUP
		);
	tjDestroy(dhandle);
	if(flag == -1) return;

	LOGI("JPEG %d*%d", width, height);
	mCflag = true;
}


void DepthImageBase::setDepthData(const char *srcData, int dstSize)
{
	if(dstSize > mSampleCodeSize) reSizeSampleCode(dstSize);

	/******直接传输，移除QuickLZ*******/

// 	qlz_state_decompress *state_decompress = (qlz_state_decompress*)malloc(sizeof(qlz_state_decompress));
// 
// 	int r = qlz_decompress(srcData, mSampleCode, state_decompress);
// 
// 	free(state_decompress);
// 
// 	if(r != dstSize) 
// 	{
// 		LOGI("the decompressed size of depth does not match");
// 		return;
// 	}

	memcpy(mSampleCode, srcData, dstSize);
	int readW = getLocalInt(mSampleCode + 4);
	int readH = getLocalInt(mSampleCode + 8);

	Resolution tmpRes = GlobalInfo::getInstance().getDepthRes();
	if(readW != tmpRes.w || readH != tmpRes.h)
	{
		LOGI("the size of depth image does not match!");
		return;
	}
	
	int blockY = (readH - 1) / BSIZE;

	int headPos = 12;

	int dataPos = getLocalInt(mSampleCode);

	bool half = false;
	bool nowBlock = false;
	int nowX = 0, nowY = 0;

	int dataCnt = ((dataPos - headPos) / 3) * 2;
	if((dataPos - headPos) % 3 != 0) dataCnt += 1;

	if(dataCnt * 3 > mSampleSize) reSizeSample(dataCnt * 3);
	mSampleCnt = dataCnt;

	for(int i = 0; i < dataCnt; ++i)
	{
		HeadInfo  hInfo = getHeadInfo(mSampleCode + headPos, half);
		if(hInfo.nextBlock != nowBlock)
		{
			if(nowY == blockY)
			{
				nowX += 1;
				nowY = 0;
			}
			else nowY += 1;
			nowBlock = hInfo.nextBlock;
		}
		int tmpW = nowX * BSIZE + hInfo.index / BSIZE;
		int tmpH = nowY * BSIZE + hInfo.index % BSIZE;

		mSample[3*i] = (2*tmpW + 1 - readW) / ((float)(readW));
		mSample[3*i + 1] = (2 * tmpH + 1  - readH) /((float)(readH));
		mSample[3*i + 2] = 1.0f;
		if(!hInfo.isOne)
		{
			mSample[3*i + 2] = toDepthValue(mSampleCode + dataPos);
			dataPos += 2;
			//确定深度重构是否正确
			//LOGI("No_one_value: %f", mSample[3*i + 2]);
		}
		headPos += (half ? 2 : 1);
		half = (!half);
	}
	mDflag = true;
}


void DepthImageBase::reSizeColor(int cnt)
{
	delete[] mColor;
	mColor = NULL;
	mColor = new uchar[cnt];

	if(mColor != NULL)
	{
		LOGI("resize color buffer successfully~");
	}
	mColorSize = cnt;
}

void DepthImageBase::reSizeSampleCode(int cnt)
{
	delete[] mSampleCode;
	mSampleCode = NULL;
	mSampleCode  = new uchar[2 * cnt];
	if(mSampleCode != NULL)
	{
		LOGI("resize sample code buffer successfully~");
	}
	mSampleCodeSize = 2 * cnt;
}

void DepthImageBase::reSizeSample(int cnt)
{
	delete[] mSample;
	mSample = NULL;
	mSample = new float[2 * cnt];
	if(mSample != NULL)
	{
		LOGI("resize sample  buffer successfully~");
	}
	mSampleSize = 2 * cnt;
}

int DepthImageBase::getLocalInt(uchar* data)
{
	int tmp = 0;
	tmp |= ((data[0] << 24 ) & 0xff000000);
	tmp |= ((data[1] << 16) & 0xff0000);
	tmp |= ((data[2] << 8) & 0xff00);
	tmp |= (data[3] & 0xff);
	return tmp;
}

HeadInfo DepthImageBase::getHeadInfo(uchar *head, bool half)
{
	HeadInfo rs;
	if(!half)
	{
		if((head[0] & 0x80) != 0) rs.nextBlock = true;
		if((head[0] & 0x40) != 0) rs.isOne = true;
		rs.index |= (head[0] << 4) & 0x3f0;
		rs.index |= (head[1] >> 4) & 0xf;
	}
	else
	{
		if((head[0] & 0x8) != 0) rs.nextBlock = true;
		if((head[0] & 0x4) != 0) rs.isOne = true;
		rs.index |= (head[0] << 8) & 0x300;
		rs.index |= head[1] & 0xff;
	}
	return rs;
}

float DepthImageBase::toDepthValue(uchar *data)
{
	int tmp = 0;
	tmp |= ((data[0] << 8) & 0xff00);
	tmp |= (data[1] & 0xff);

	float rs = (float)(tmp * 1.0 / MAX);
	return rs;
}
