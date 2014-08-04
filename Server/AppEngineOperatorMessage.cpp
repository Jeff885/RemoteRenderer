
#include "AppEngineOperatorMessage.h"
AppEngineOperatorMessage::AppEngineOperatorMessage()
{
	Message::_client_id=INVALID_CLIENT_ID;
	Message::_msg_type=APPLICATIONMSG;
}
// 返回消息大小，以字节为单位，用于序列化
size_t AppEngineOperatorMessage::getSize()
{
	return 29;
}

// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
size_t AppEngineOperatorMessage::serialize(unsigned char *write_buffer)
{
	// no implemention
	return 0;
}

// 从字节缓冲区中反序列化得到消息数据结构，返回读出的字节数
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