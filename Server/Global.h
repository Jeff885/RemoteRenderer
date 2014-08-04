// 全局头文件
// author：胡山峰
// date：2013-8-29

#ifndef _GLOBAL_H
#define _GLOBAL_H

// C++头文件

#include <string>
#include <vector>
#include <queue>
#include <exception>
#include <iostream>
#include <assert.h>



// BOOST头文件
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
typedef boost::asio::ip::tcp::socket  tcp_Socket;
typedef  boost::asio::io_service   io_service;

// OSG头文件
#include <Windows.h>
#include <osg/Vec3f>
#include <osg/Matrixf>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/ref_ptr>
#include <osg/Switch>
#include <osgDB/ReadFile>


/*
#include "Client.h"
#include "ClientTable.h"
#include "Message.h"
#include "MessageQueue.h"
#include "Session.h"
#include "Manipulator.h"
#include "Logger.h"
#include "ServerTask.h"
#include "ServerPipeline.h"
*/
enum  DataType
{
	VINO_CONFIG_SYN  = 0,
	VINO_CONFIG_VIEW_FRUSTUM = 0x08,
	VINO_CONFIG_VIEW_PERSPECTIVE = 0x09,
	VINO_CONFIG_VIEW_RESOLUTION = 0x0A,
	VINO_CONFIG_CAMERA_STANDARD = 0x10,
	VINO_CONFIG_QUALITY_UPSAMPLE = 0x18,
	VINO_MODEL_3DIMAGE = 0x40,
	VINO_MOTION_TOUCH_STANDARD = 0x80
};

using namespace std;

#define write_char(buf, at, cp) ((buf+at)[0]=((unsigned char*)cp)[0],\
	(buf+at)[1]=((unsigned char*)cp)[1])

#define write_uchar(buf, at, ucp) ((buf+at)[0]=((unsigned char*)ucp)[0])

#define write_short(buf, at, sp) ((buf+at)[0]=((unsigned char*)sp)[0],\
	(buf+at)[1]=((unsigned char*)sp)[1])

#define write_ushort(buf, at, usp) ((buf+at)[0]=((unsigned char*)usp)[0],\
	(buf+at)[1]=((unsigned char*)usp)[1])

#define write_int(buf, at, ip) ((buf+at)[0]=((unsigned char*)ip)[0],\
	(buf+at)[1]=((unsigned char*)ip)[1],\
	(buf+at)[2]=((unsigned char*)ip)[2],\
	(buf+at)[3]=((unsigned char*)ip)[3])

#define write_uint(buf, at, uip) ((buf+at)[0]=((unsigned char*)uip)[0],\
	(buf+at)[1]=((unsigned char*)uip)[1],\
	(buf+at)[2]=((unsigned char*)uip)[2],\
	(buf+at)[3]=((unsigned char*)uip)[3])

#define write_float(buf, at, fp) ((buf+at)[0]=((unsigned char*)fp)[0],\
	(buf+at)[1]=((unsigned char*)fp)[1],\
	(buf+at)[2]=((unsigned char*)fp)[2],\
	(buf+at)[3]=((unsigned char*)fp)[3])

#define write_double(buf, at, dp) (buf[0]=((unsigned char*)dp)[0],\
	buf[1]=((unsigned char*)dp)[1],\
	buf[2]=((unsigned char*)dp)[2],\
	buf[3]=((unsigned char*)dp)[3],\
	buf[4]=((unsigned char*)dp)[4],\
	buf[5]=((unsigned char*)dp)[5],\
	buf[6]=((unsigned char*)dp)[6],\
	buf[7]=((unsigned char*)dp)[7])

#define read_char(buf, at, cp) (*cp = *((char*)(buf+at)))

#define read_uchar(buf, at, ucp) (*ucp = *((unsigned char*)(buf+at)))

#define read_short(buf, at, sp) (*sp = *((short*)(buf+at)))

#define read_ushort(buf, at, usp) (*usp = *((unsigned short*)(buf+at)))

#define read_int(buf, at, ip) (*ip = *((int*)(buf+at)))

#define read_uint(buf, at, uip) (*uip = *((unsigned int*)(buf+at)))

#define read_float(buf, at, fp) (*fp = *((float*)(buf+at)))

#define read_double(buf, at, dp) (*dp = *((double*)(buf+at)))

#define INVALID_CLIENT_ID 0
//#define RFPM  //设置第一视角模式
enum Interaction
{
	TRACKBALLMANIPULATOR=0,
	FIRSTPERSONMANIPULATOR=1,
	USER_DEFINE=2
};
enum ApplicationType
{
	DEFAULTVALUE = 0,//缺省值为1
	ENGINESHOW=1//发动机结构展示
};


#endif