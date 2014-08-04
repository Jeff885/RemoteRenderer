#include "TouchEventBuffer.h"
#include <boost//thread/mutex.hpp>

TouchEventBuffer::TouchEventBuffer()
{
	_buffer1 = new TouchEvent[MAX_BUFFER_SIZE];
	_buffer2 = new TouchEvent[MAX_BUFFER_SIZE];
	_read = _buffer1;
	_write = _buffer2;

	_cnt = 0;
	_storedCnt = 0;
}

TouchEventBuffer::~TouchEventBuffer()
{
	if(_buffer1) delete[] _buffer1;
	if(_buffer2) delete[]	_buffer2;
}

void TouchEventBuffer::putEventData(const TouchEvent& md)
{
	
		boost::mutex::scoped_lock lock(_writeMutex);
		_write[_cnt++] = md;
}

void TouchEventBuffer::swapBuffer()
{
	boost::mutex::scoped_lock lock(_writeMutex);
	TouchEvent* tmp = _read;
	_read = _write;
	_write = tmp;
	_storedCnt = this->_cnt;
	this->_cnt = 0;
}

int TouchEventBuffer::getStoredCount()
{
	return _storedCnt;
}
TouchEvent& TouchEventBuffer::getMotionData(const int& pos)
{
	assert(pos < _storedCnt);
	return _read[pos];
}
