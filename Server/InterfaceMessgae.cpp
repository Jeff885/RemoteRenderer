#include "InterfaceMessage.h"
CameraPositionMsg::CameraPositionMsg()
{
	Message::_client_id=INVALID_CLIENT_ID;
	Message::_msg_type=CAMERAPOSITION;
}
size_t CameraPositionMsg::getSize()
{
	return 29;
}
// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
size_t CameraPositionMsg::serialize(unsigned char *write_buffer)
{
	return 29;
}
size_t CameraPositionMsg::deserialize(unsigned char *const read_buffer)
{
	read_float(read_buffer,12,&x);
	read_float(read_buffer,16,&y);
	read_float(read_buffer,20,&z);
	return 29;
}
float CameraPositionMsg::getX()
{
	return x;
}
float CameraPositionMsg::getY()
{
	return y;
}
float CameraPositionMsg::getZ()
{
	return z;
}
CameraPositionMsg::~CameraPositionMsg()
{

}

//////////////////////////////////////////////////////////////////////////
CameraCenterMsg::CameraCenterMsg()
{
	Message::_client_id=INVALID_CLIENT_ID;
	Message::_msg_type=CAMERACENTER;
}
size_t CameraCenterMsg::getSize()
{
	return 29;
}
// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
size_t CameraCenterMsg::serialize(unsigned char *write_buffer)
{
	return 29;
}
size_t CameraCenterMsg::deserialize(unsigned char *const read_buffer)
{
	read_float(read_buffer,12,&x);
	read_float(read_buffer,16,&y);
	read_float(read_buffer,20,&z);
	return 29;
}
float CameraCenterMsg::getX()
{
	return x;
}
float CameraCenterMsg::getY()
{
	return y;
}
float CameraCenterMsg::getZ()
{
	return z;
}
CameraCenterMsg::~CameraCenterMsg()
{

}

//////////////////////////////////////////////////////////////////////////


CameraUpMsg::CameraUpMsg()
{
	Message::_client_id=INVALID_CLIENT_ID;
	Message::_msg_type=CAMERAUP;
}
size_t CameraUpMsg::getSize()
{
	return 29;
}
// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
size_t CameraUpMsg::serialize(unsigned char *write_buffer)
{
	return 29;
}
size_t CameraUpMsg::deserialize(unsigned char *const read_buffer)
{
	read_float(read_buffer,12,&x);
	read_float(read_buffer,16,&y);
	read_float(read_buffer,20,&z);
	return 29;
}
float CameraUpMsg::getX()
{
	return x;
}
float CameraUpMsg::getY()
{
	return y;
}
float CameraUpMsg::getZ()
{
	return z;
}
CameraUpMsg::~CameraUpMsg()
{

}
