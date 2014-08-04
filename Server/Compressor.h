#ifndef		__COMPRESSOR_H__
#define		__COMPRESSOR_H__

#include <osg/Image>
#include "QuickLZ.h"
extern "C"
{
#include <turbojpeg.h>
};
class Compressor
{
public:
	Compressor(int width, int height);
	static  void  startJpegCmp(Compressor* cmpor, osg::Image*  cSrc);

	static  void  startDepthCmp(Compressor* cmpor, osg::Image*  dSrc);

	static  void  putDepthIndex(
		unsigned char* data, 
		bool half,
		unsigned int index,
		bool flag,
		bool isOne
		);
	static   void putDepthData(
		unsigned char* data,
		const unsigned int& val
		);
	static void putInt(unsigned char* data, const unsigned int& val);

	

	~Compressor();

	void doCompress(osg::Image* cSrc, osg::Image* dSrc);

	bool isDataAccess();

	void banDataAccess();

	unsigned  char* getJpegBuffer();

	unsigned long getJpegSize();

	char* getDepthDstBuffer();

	size_t getDepthDstSize();

	size_t getDepthSrcSize();


private:
	unsigned char*	_jpegBuf;
	unsigned long 	_jpegSize;

	unsigned char*	_depthSrcBuf;
	int _depthSrcBufSize;
	size_t  _depthSrcSize;

	char*		_depthDstBuf;
	size_t	_depthDstSize;

	qlz_state_compress* _stateCmpress;

	bool	 _isDataAccess;
	bool	 _isJpegSucc;
	bool  _isDepthSucc;

	//friend class Client;
};
#endif