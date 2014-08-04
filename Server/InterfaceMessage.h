#pragma once
//2014_8_3 writen by gmj
#ifndef INTERFACEMSG
#define INTERFACEMSG
#include "Global.h"
#include "Message.h"
class Message;
class CameraPositionMsg : public Message 
{
public:
	CameraPositionMsg();
	// 返回消息大小，以字节为单位，用于序列化
	virtual size_t getSize();

	// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
	virtual size_t serialize(unsigned char *write_buffer);

	// 从字节缓冲区中反序列化得到消息数据结构，返回读出的字节数
	virtual size_t deserialize(unsigned char *const read_buffer);

	virtual ~CameraPositionMsg();
	float getX();
	float getY();
	float getZ();
protected:
	float x,y,z;
};
class CameraCenterMsg : public Message 
{
public:
	CameraCenterMsg();
	// 返回消息大小，以字节为单位，用于序列化
	virtual size_t getSize();

	// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
	virtual size_t serialize(unsigned char *write_buffer);

	// 从字节缓冲区中反序列化得到消息数据结构，返回读出的字节数
	virtual size_t deserialize(unsigned char *const read_buffer);

	virtual ~CameraCenterMsg();
	float getX();
	float getY();
	float getZ();
protected:
	float x,y,z;
};

class CameraUpMsg : public Message 
{
public:
	CameraUpMsg();
	// 返回消息大小，以字节为单位，用于序列化
	virtual size_t getSize();

	// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
	virtual size_t serialize(unsigned char *write_buffer);

	// 从字节缓冲区中反序列化得到消息数据结构，返回读出的字节数
	virtual size_t deserialize(unsigned char *const read_buffer);

	virtual ~CameraUpMsg();
	float getX();
	float getY();
	float getZ();
protected:
	float x,y,z;
};

#endif