#pragma once
#ifndef APPMSG
#define APPMSG
#include "Global.h"
#include "Message.h"
//Ӧ������Ϣ�壺����(4)������(4)���ͻ���ID(4)������id(4), ����ָ��(4)�����(9) =29���ֽ�
class AppEngineOperatorMessage : public Message 
{
public:
	AppEngineOperatorMessage();
	// ������Ϣ��С�����ֽ�Ϊ��λ���������л�
	virtual size_t getSize();

	// ����Ϣ���ݽṹ���л����ֽڻ������У�����д����ֽ���
	virtual size_t serialize(unsigned char *write_buffer);

	// ���ֽڻ������з����л��õ���Ϣ���ݽṹ�����ض������ֽ���
	virtual size_t deserialize(unsigned char *const read_buffer);

	virtual ~AppEngineOperatorMessage();
	int getSeqId();
	int getOp();
protected:
	int node_id;//������ID��
	int op;//ָ�����
};
#endif