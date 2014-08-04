
#include "AppEngineOperatorMessage.h"
AppEngineOperatorMessage::AppEngineOperatorMessage()
{
	Message::_client_id=INVALID_CLIENT_ID;
	Message::_msg_type=APPLICATIONMSG;
}
// ������Ϣ��С�����ֽ�Ϊ��λ���������л�
size_t AppEngineOperatorMessage::getSize()
{
	return 29;
}

// ����Ϣ���ݽṹ���л����ֽڻ������У�����д����ֽ���
size_t AppEngineOperatorMessage::serialize(unsigned char *write_buffer)
{
	// no implemention
	return 0;
}

// ���ֽڻ������з����л��õ���Ϣ���ݽṹ�����ض������ֽ���
size_t AppEngineOperatorMessage::deserialize(unsigned char *const read_buffer)
{
	read_int(read_buffer,12,&node_id);
	read_int(read_buffer,16,&op);
	return 29;
}

AppEngineOperatorMessage::~AppEngineOperatorMessage()
{

}
int AppEngineOperatorMessage::getSeqId()
{
	return node_id;
}
int AppEngineOperatorMessage::getOp()
{
	return op;
}