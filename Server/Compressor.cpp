#include <Windows.h>
#include "Compressor.h"
#include <iostream>
#include <boost/thread/thread.hpp>
// 创建compressor对象，分配压缩缓冲区
using namespace std;
Compressor::Compressor(int width, int height)
	:_depthSrcBuf(NULL),
	_depthSrcBufSize(0),
	_depthSrcSize(0),
	_depthDstBuf(NULL),
	_depthDstSize(0),
	_isDataAccess(false)
{
	_jpegBuf = tjAlloc(width * height * 3 + 100);

	_stateCmpress = (qlz_state_compress *) malloc(sizeof(qlz_state_compress));
}

// 析构compressor对象，释放压缩缓冲区
Compressor::~Compressor()
{
	tjFree(_jpegBuf);

	if(_depthSrcBuf)  delete _depthSrcBuf;//
	if(_depthDstBuf)	delete _depthDstBuf;

	free(_stateCmpress);
}

// 执行jpeg压缩，统计压缩数据量和压缩时间
void Compressor::startJpegCmp(Compressor* cmpor, osg::Image* cSrc)
{
	static int CNT = 0;
	CNT ++;
	static double  DATASIZE = 0.0;
	static double  PROTIME = 0.0;

	clock_t stt = clock();

	tjhandle handle = NULL;
	if((handle = tjInitCompress()) == NULL)
	{
		cout<< "*******  " << "JPEG handle initialization  failed~"
			<< "  *******\n";
		cmpor->_isJpegSucc = false;
		return;
	}
	//颜色图像压缩参数设置：TJSAMP_420,压缩质量：80（与客户端对应）
	int flag = tjCompress2(
		handle,
		cSrc->data(),
		cSrc->s(),
		0,
		cSrc->t(),
		TJPF_RGB,
		&cmpor->_jpegBuf,
		&cmpor->_jpegSize,
		TJSAMP_420,
		80,
		TJFLAG_BOTTOMUP
		);
	if(flag == -1)
	{
		cout<< "*******  " << "JPEG compression  failed~"
			<< "  *******\n";
		cmpor->_isJpegSucc = false;
		return;
	}
	cmpor->_isJpegSucc = true;
	clock_t fsh = clock();

	cout<< "JPEG compress succeeded~, time:" << fsh - stt <<"ms, " 
		<< "Size(S : D):" << cSrc->getTotalSizeInBytes() * 1.0 / 1024 << "Kb : "
		<< cmpor->_jpegSize * 1.0 / 1024 <<"Kb\n";
	DATASIZE += cmpor->_jpegSize * 1.0 / 1024;
	PROTIME += fsh - stt;
	cout<< "compress时间: "<< PROTIME / CNT << "ms" << "\n";
	cout<< "jpeg数据: "<< DATASIZE / CNT << "kb" << "\n";
}

// 执行深度压缩，统计压缩数据量和压缩时间
void Compressor::startDepthCmp(Compressor* cmpor, osg::Image* dSrc)
{
	static int CNT = 0;
	CNT ++;
	static double  DATASIZE = 0.0;

	clock_t stt = clock();
	const double  epsilon = 0.00000005;
	const  int  MAX = (1 << 16) -1;
	const  int bSize = 32;

	int i,j;
	int dataSize = dSrc->s() * dSrc->t(), dataCnt = 0;
	float* srcData = reinterpret_cast<float*>( dSrc->data());

	for(i = 0; i < dataSize; ++i)
		if(srcData[2*i + 1] > epsilon) ++dataCnt;

	cout <<"采样点个数: "<<dataCnt << endl;

	cout<< "Depth value count :" << dataCnt << "\n";
	if(dataCnt * 3.5 + 4 > cmpor->_depthSrcBufSize)
	{
		delete cmpor->_depthSrcBuf;
		delete cmpor->_depthDstBuf;
		cmpor->_depthSrcBufSize = dataCnt * 20;
		cmpor->_depthSrcBuf = new unsigned char[cmpor->_depthSrcBufSize];
		cmpor->_depthDstBuf = new char[cmpor->_depthSrcBufSize + 400];
		cout<< "*******  "  << "Depth buffers resized" << "  *******\n";
	}


	int blockX = (dSrc->s() - 1)/ bSize;
	int blockY = (dSrc->t() - 1)/ bSize;

	cmpor->_depthSrcSize = (dataCnt / 2) * 3;
	if(dataCnt & 1) cmpor->_depthSrcSize += 2;

	cmpor->_depthSrcSize += 12;

	putInt(cmpor->_depthSrcBuf, cmpor->_depthSrcSize);
	putInt(&cmpor->_depthSrcBuf[4], dSrc->s());
	putInt(&cmpor->_depthSrcBuf[8], dSrc->t());


	unsigned char* indexData = &cmpor->_depthSrcBuf[12];
	unsigned char* realData = &cmpor->_depthSrcBuf[cmpor->_depthSrcSize];

	int nextIndexPos=0;
	int nextRealPos = 0;
	int OneCnt = 0;

	bool flag = true;
	bool half = true;

	for(i = 0; i <= blockX; ++i)
	{
		int inX = dSrc->s() - i * bSize;
		if(inX > bSize) inX = bSize;

		for(j = 0; j <= blockY; ++j)
		{
			flag = (!flag);
			int inY = dSrc->t() - j * bSize;
			if(inY > bSize) inY = bSize;
			int ti, tj;
			for(ti = 0; ti < inX; ++ti)
				for(tj = 0; tj < inY; ++tj)
				{
					int posX = i *32 + ti;
					int posY = j * 32 + tj;
					int tPos = posY * dSrc->s() + posX;
					float data1 = srcData[2 * tPos];
					float data2 = srcData[2 * tPos + 1];
					if(data2 > epsilon)
					{
						half = (!half);
						bool isOne = false;
						if(1.0 - data1 < epsilon)
						{
							isOne = true;
							++OneCnt;
						}
						putDepthIndex(&indexData[nextIndexPos], half, ti*bSize + tj, flag, isOne);
						unsigned int val = unsigned int(data1 * MAX);
						if(!isOne) 
						{
							//VinoGlobal::vout<< "<" <<val << "," << data1 << ">";  
							putDepthData(&realData[nextRealPos], val);
							nextRealPos += 2;
						}
						++nextIndexPos;
						if(half) ++nextIndexPos;
					}
				}
		}
		//VinoGlobal::vout<<"\n";
	}
	cmpor->_depthSrcSize += 2 *(dataCnt - OneCnt);


	/*直接传输，移除QuickLZ  小模型效果还行，大模型效果不好
	cmpor->_depthDstSize = qlz_compress(
		cmpor->_depthSrcBuf,
		cmpor->_depthDstBuf,
		cmpor->_depthSrcSize,
		cmpor->_stateCmpress
		);*/

	cmpor->_depthDstSize = cmpor->_depthSrcSize;
	memcpy(cmpor->_depthDstBuf, cmpor->_depthSrcBuf, cmpor->_depthSrcSize);
	cmpor->_isDepthSucc = true;
	//cmpor->_depthDstBuf[4]
	clock_t fsh = clock();
	cout<< "Depth compress succeeded~, time:" << fsh - stt <<"ms, " 
		<< "Size(S : D):" << cmpor->_depthSrcSize * 1.0 / 1024 << "Kb : "
		<< cmpor->_depthDstSize * 1.0 / 1024 <<"Kb\n";
	DATASIZE += cmpor->_depthDstSize * 1.0 / 1024;
	cout<< "edge数据: "<< DATASIZE / CNT << "kb" << "\n";
}

// 分别创建颜色图像和深度图像压缩线程，执行压缩，标识压缩结果
void Compressor::doCompress(osg::Image* cSrc, osg::Image* dSrc)
{
	cout<<"doCompress"<<endl;
	_isDataAccess = false;
	_isDepthSucc = false;
	_isJpegSucc = false;
	clock_t stt = clock();
	boost::thread jpegThd(boost::bind(&startJpegCmp, this, cSrc));
	boost::thread depthThd(boost::bind(&startDepthCmp, this, dSrc));
	jpegThd.join();
	depthThd.join();
	if(_isDepthSucc&& _isJpegSucc) _isDataAccess = true;
	if(_isDataAccess)
	{
		clock_t fsh = clock();
		cout<< ">>>>>>>  " << "Compression is successful~, time: " 
			<<fsh - stt << "ms.  <<<<<<<\n";
	}
	
}


void Compressor::putDepthIndex( 
	unsigned char* data, 
	bool half, 
	unsigned int index, 
	bool flag, 
	bool isOne )
{
	int tmp;
	if(!half)
	{
		data[0] = 0;
		data[1] = 0;
		if(flag) data[0] |= 0x80;
		if(isOne) data[0] |= 0x40;
		tmp = (index >> 4) & 0x3f;
		data[0] |= tmp;
		tmp = (index << 4) & 0xf0;
		data[1] |= tmp;
	}
	else
	{
		data[1] = 0;
		if(flag) data[0] |= 0x8;
		if(isOne) data[0] |= 0x4;
		tmp = (index >> 8) & 0x3;
		data[0] |= tmp;
		tmp = index & 0xff;
		data[1] |= tmp;
	}
}

// 将整形值的低2个字节放入缓冲区中
void Compressor::putDepthData( unsigned char* data, const unsigned int& val )
{
	data[0] = 0;
	data[1] = 0;
	int tmp;
	tmp = (val >> 8) & 0xff;
	data[0] |= tmp;
	tmp = val & 0xff;
	data[1] |= tmp;
}

void Compressor::putInt(unsigned char* data, const unsigned int& val)
{
	unsigned int tmp;
	int i;
	for(i = 3; i >=0; --i)
	{
		tmp = (val >> (i*8)) & 0xff;
		data[3 -i] = 0;
		data[3 - i] |= tmp;
	}
}

unsigned char* Compressor::getJpegBuffer()
{
	return _jpegBuf;
}

unsigned long Compressor::getJpegSize()
{
	return _jpegSize;
}

char* Compressor::getDepthDstBuffer()
{
	return _depthDstBuf;
}

size_t Compressor::getDepthDstSize()
{
	return _depthDstSize;
}
size_t Compressor::getDepthSrcSize()
{
	return _depthSrcSize;
}
bool Compressor::isDataAccess()
{
	return _isDataAccess;
}
void Compressor::banDataAccess()
{
	_isDataAccess = false;
}