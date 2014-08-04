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
	// ������Ϣ��С�����ֽ�Ϊ��λ���������л�
	virtual size_t getSize();

	// ����Ϣ���ݽṹ���л����ֽڻ������У�����д����ֽ���
	virtual size_t serialize(unsigned char *write_buffer);

	// ���ֽڻ������з����л��õ���Ϣ���ݽṹ�����ض������ֽ���
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
	// ������Ϣ��С�����ֽ�Ϊ��λ���������л�
	virtual size_t getSize();

	// ����Ϣ���ݽṹ���л����ֽڻ������У�����д����ֽ���
	virtual size_t serialize(unsigned char *write_buffer);

	// ���ֽڻ������з����л��õ���Ϣ���ݽṹ�����ض������ֽ���
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
	// ������Ϣ��С�����ֽ�Ϊ��λ���������л�
	virtual size_t getSize();

	// ����Ϣ���ݽṹ���л����ֽڻ������У�����д����ֽ���
	virtual size_t serialize(unsigned char *write_buffer);

	// ���ֽڻ������з����л��õ���Ϣ���ݽṹ�����ض������ֽ���
	virtual size_t deserialize(unsigned char *const read_buffer);

	virtual ~CameraUpMsg();
	float getX();
	float getY();
	float getZ();
protected:
	float x,y,z;
};

#endif