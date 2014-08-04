#include "Message.h"




size_t Message::getType() {

	return _msg_type;
}

// 设置消息类型
void Message::setType(size_t type) {
	_msg_type = type;
}

// 返回客户端ID
size_t Message::getClientID(){
	return _client_id;
}

// 设置客户端ID
void Message::setClientID(size_t client_id) {
	_client_id = client_id;
}
Message:: ~Message()
 {

 }

	StartMessage::StartMessage() {
		_msg_type = CONTROL_START_MSG;
		_client_id =INVALID_CLIENT_ID;
	}

	StartMessage::StartMessage(size_t client_id) {
		_msg_type = CONTROL_START_MSG;
		_client_id = client_id;
	}

	StartMessage::StartMessage(StartMessage &msg) {
		_msg_type = CONTROL_START_MSG;
		_client_id = msg._client_id;
	}

	size_t StartMessage:: getSize(){
		return 16;
	}
	int StartMessage::getAppType()
	{
		return apptype;
	}

	size_t StartMessage::serialize(unsigned char *write_buffer){
		size_t len = getSize();
		write_uint(write_buffer, 0, &len);
		write_uint(write_buffer, 4, &_msg_type);
		write_uint(write_buffer, 8, &_client_id);
		return len;
	}

	size_t StartMessage::deserialize(unsigned char *const read_buffer) {
		size_t len = getSize();
		read_int(read_buffer,8,&apptype);
		return len;
	}

	StartMessage::~StartMessage(){
		// do nothing
	}

CameraMessage::CameraMessage() {
			_msg_type = INITIAL_CAMERA_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void CameraMessage::getCameraArgs(Camera &camera) {
			camera = _camera;
		}

		void CameraMessage::setCameraArgs(const Camera &camera) {
			_camera = camera;
		}

		size_t CameraMessage::getSize() {
			return 12 + sizeof(Camera);
		}

		size_t CameraMessage::serialize(unsigned char *write_buffer) {
			/*
			  长度，类型，id,内容
			*/
			size_t len = getSize();
			write_uint(write_buffer, 0, &len);
			write_uint(write_buffer, 4, &_msg_type);
			write_uint(write_buffer, 8, &_client_id);

			write_float(write_buffer, 12, &_camera._eyex);
			write_float(write_buffer, 16, &_camera._eyey);
			write_float(write_buffer, 20, &_camera._eyez);

			write_float(write_buffer, 24, &_camera._centerx);
			write_float(write_buffer, 28, &_camera._centery);
			write_float(write_buffer, 32, &_camera._centerz);

			write_float(write_buffer, 36, &_camera._upx);
			write_float(write_buffer, 40, &_camera._upy);
			write_float(write_buffer, 44, &_camera._upz);

			return len;
		}

		size_t CameraMessage::deserialize(unsigned char *const read_buffer) {
			size_t len = getSize();
			read_float(read_buffer, 12, &_camera._eyex);
			read_float(read_buffer, 16, &_camera._eyey);
			read_float(read_buffer, 20, &_camera._eyez);

			read_float(read_buffer, 24, &_camera._centerx);
			read_float(read_buffer, 28, &_camera._centery);
			read_float(read_buffer, 32, &_camera._centerz);

			read_float(read_buffer, 36, &_camera._upx);
			read_float(read_buffer, 40, &_camera._upy);
			read_float(read_buffer, 44, &_camera._upz);
			return len;
		}

		CameraMessage::~CameraMessage(){
			// do nothing
		}


		PerspectiveMessage::PerspectiveMessage() {
			_msg_type = INITIAL_PERSPECTIVE_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void PerspectiveMessage::getPerspectiveArgs(Perspective &perspective) {
			perspective = _perspective;
		}

		void PerspectiveMessage::setPerspectiveArgs(const Perspective &perspective) {
			_perspective = perspective;
		}

		size_t PerspectiveMessage::getSize(){
			return 12+sizeof(Perspective);
		}

		size_t PerspectiveMessage::serialize(unsigned char *write_buffer) {
			size_t len = getSize();
			write_uint(write_buffer, 0, &len);
			write_uint(write_buffer, 4, &_msg_type);
			write_uint(write_buffer, 8, &_client_id);

			write_float(write_buffer, 12, &_perspective._fov);
			write_float(write_buffer, 16, &_perspective._aspect);
			write_float(write_buffer, 20, &_perspective._near);
			write_float(write_buffer, 24, &_perspective._far);
			return len;
		}

		size_t PerspectiveMessage::deserialize(unsigned char *const read_buffer) {
			size_t len = getSize();
			read_float(read_buffer, 12, &_perspective._fov);
			read_float(read_buffer, 16, &_perspective._aspect);
			read_float(read_buffer, 20, &_perspective._near);
			read_float(read_buffer, 24, &_perspective._far);
			return len;
		}

		PerspectiveMessage::~PerspectiveMessage(){
			// do nothing
		}


		TouchEventMessage::TouchEventMessage() {
			_msg_type = RUNTIME_TOUCH_EVENT_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void TouchEventMessage::getTouchEventArgs(TouchEvent &touch_event) {
			touch_event = _touch_event;
		}

		void TouchEventMessage::setResolutionArgs(const TouchEvent &touch_event) {
			_touch_event = touch_event;
		}

		size_t TouchEventMessage::getSize() {
			return 12+sizeof(TouchEvent);//29
		}

		size_t TouchEventMessage::serialize(unsigned char *write_buffer) {

			size_t len = getSize();
			write_uint(write_buffer, 0, &len);
			write_uint(write_buffer, 4, &_msg_type);
			write_uint(write_buffer, 8, &_client_id);

			write_uchar(write_buffer, 12, &_touch_event._event_type);
			write_float(write_buffer, 13, &_touch_event._x1);
			write_float(write_buffer, 17, &_touch_event._y1);
			write_float(write_buffer, 21, &_touch_event._x2);
			write_float(write_buffer, 25, &_touch_event._y2);
			return len;
		}

		size_t TouchEventMessage::deserialize(unsigned char *const read_buffer) {
			size_t len = getSize(); 
			read_uchar(read_buffer, 12, &_touch_event._event_type);
			read_float(read_buffer, 13, &_touch_event._x1);
			read_float(read_buffer, 17, &_touch_event._y1);
			read_float(read_buffer, 21, &_touch_event._x2);
			read_float(read_buffer, 25, &_touch_event._y2);
			return len;
		}

		TouchEventMessage::~TouchEventMessage(){
			// do nothing
		}
		InteractionMessage::InteractionMessage()
		{
			_msg_type=INITIAL_INTERACTION_MSG;
			_client_id=INVALID_CLIENT_ID;
		}
		size_t InteractionMessage::getSize()
		{
			return 12+sizeof(int);
		}
		size_t InteractionMessage::deserialize(unsigned char* const read_buffer)
		{
			//size_t len=getSize();
			read_int(read_buffer,12,&_it._type);
			return getSize();
		}
		void InteractionMessage::getInteractionType(InteractionType &interaction)
		{
			interaction=_it;
		}
		size_t InteractionMessage::serialize(unsigned char *write_buffer)
		{
			return 0;
		}

		StringMessage::StringMessage(size_t s):_strsize(s)
		{
			_client_id=INVALID_CLIENT_ID;
		}
		void  StringMessage::setSize(size_t s)
		{
			_strsize=s;
		}
		size_t StringMessage::getSize()
		{
			return _strsize;
		}
		size_t StringMessage::deserialize(unsigned char * const read_buffer)
		{
			char ch;
			int i=0;
			for(i=0;i<_strsize;i++)
			{
				read_char(read_buffer,12+i,&ch);
				if(ch)
					_str.push_back(ch);
				else
					break;
			}
			_strsize=i+1;
			return _strsize;
		}
		size_t StringMessage::serialize(unsigned char *write_buffer)
		{
			return 0;
		}
		string StringMessage::getString()
		{
			return _str;
		}
		StopMessage::StopMessage() {
			_msg_type = CONTROL_STOP_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		StopMessage::StopMessage(size_t client_id) {
			_msg_type = CONTROL_STOP_MSG;
			_client_id = client_id;
		}

		StopMessage::StopMessage(StopMessage &msg) {
			_msg_type = CONTROL_STOP_MSG;
			_client_id = msg._client_id;
		}

		size_t StopMessage::getSize() {
			return 12;
		}

		size_t StopMessage::serialize(unsigned char *write_buffer) {
			size_t len = getSize();
			write_uint(write_buffer, 0, &len);
			write_uint(write_buffer, 4, &_msg_type);
			write_uint(write_buffer, 8, &_client_id);
			return len;
		}

		size_t StopMessage::deserialize(unsigned char *const read_buffer) {
			size_t len = getSize();
			return len;
		}

		StopMessage::~StopMessage(){
			// do nothing
		}