#ifndef TOUCH_EVENTBUFFER
#define TOUCH_EVENTBUFFER
#include "Message.h"
#include "Manipulator.h"
#include <boost/thread/mutex.hpp>
class Manipulator;

class TouchEventBuffer
{
	static const int MAX_BUFFER_SIZE = 1024;
public:
	TouchEventBuffer();

	~TouchEventBuffer();

	void putEventData(const TouchEvent& md);

	void readMotionData(Manipulator* manipulator,int width, int height);

	TouchEvent& getMotionData(const int& pos);

	int getStoredCount();

	void swapBuffer();

private:
	TouchEvent*		_read;
	TouchEvent*		_write;
	TouchEvent*		_buffer1;
	TouchEvent*		_buffer2;
	boost::mutex		_writeMutex;

	int		_cnt;
	int		_storedCnt;

};
#endif