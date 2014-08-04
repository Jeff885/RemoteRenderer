#pragma once
#ifndef APPMSG
#define APPMSG
#include "Global.h"
#include "Message.h"
//应用类消息体：长度(4)，类型(4)，客户端ID(4)，部件id(4), 操作指令(4)，填充(9) =29个字节
class AppEngineOperatorMessage : public Message 
{
public:
	AppEngineOperatorMessage();
	// 返回消息大小，以字节为单位，用于序列化
	virtual size_t getSize();

	// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
	virtual size_t serialize(unsigned char *write_buffer);

	// 从字节缓冲区中反序列化得到消息数据结构，返回读出的字节数
	virtual size_t deserialize(unsigned char *const read_buffer);

	virtual ~AppEngineOperatorMessage();
	int getSeqId();
	int getOp();
protected:
	int node_id;//部件的ID号
	int op;//指令操作
};
#endif