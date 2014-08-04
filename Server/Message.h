// 消息类声明
// author：胡山峰
// date：2013-8-29

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "Global.h"
#include <vector>
#include "Logger.h"

using namespace std;
//#include "Configure.h"
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
	// gluLookAt参数
	struct Camera {
		float _eyex, _eyey, _eyez;
		float _centerx, _centery, _centerz;
		float _upx, _upy, _upz;
	};
	//2014_7_7
	struct InteractionType{
		unsigned char _type;
	};

	// gluPerspective参数
	struct Perspective {
		float _fov, _aspect, _near, _far;
	};

	// glViewport参数
	struct Resolution {
		int _width, _height;
	};

	// android touch event
	struct TouchEvent {
		unsigned char _event_type;
		float _x1, _y1;
		float _x2, _y2;
	};

	// reference frame
	struct RefFrame {
		size_t _color_buf_len, _depth_buf_src_len, _depth_buf_dst_len;
		float _matrix[16];
		unsigned char *_color_buf, *_depth_buf;
	};

	//raw reference frame
	struct RawRefFrame {
		osg::Matrixf _view_matrix;
		osg::ref_ptr<osg::Image> _color_image;
		osg::ref_ptr<osg::Texture2D> _depth_tex;
	};

	// 消息类型，主要有三类消息，分别是控制类消息、初始化消息和运行时消息
	// 控制类消息：用于控制系统的启动、暂停和停止，以及其他系统动作，以CONTROL_开头
	// 初始化消息：用于在系统启动时进行参数初始化，以INTIAL_开头
	// 运行时消息：用于在运行时在客户端与服务器之间或者服务器内部传递信息，以RUNTIME_开头
	enum MessageType {
		// 控制类消息
		CONTROL_START_MSG,
		CONTROL_STOP_MSG,
		CONTROL_SYN_MSG,

		// 初始化消息
		INITIAL_SCENE_MSG,
		INITIAL_CAMERA_MSG,
		INITIAL_PERSPECTIVE_MSG,
		INITIAL_RESOLUTION_MSG,
		INITIAL_INTERACTION_MSG,

		// 运行时消息
		RUNTIME_TOUCH_EVENT_MSG,
		RUNTIME_REF_FRAME_MSG,
		RUNTIME_VIEW_QUERY_MSG,
		RUNTIME_SCENE_RENDERING_MSG,
		RUNTIME_IMAGE_WARPING_MSG,
		RUNTIME_VIEW_UPDATE_MSG,
		RUNTIME_REF_ENCODING_MSG,
		RUNTIME_LOG_MSG,

		//应用类消息
		APPLICATIONMSG,//长度29个字节
		CAMERAPOSITION, //长度29个字节
		CAMERACENTER,   //长度29字节
		CAMERAUP,        //长度29字节
	};

	// 消息抽象类
	class Message {

	public:
		// 返回消息类型
		size_t getType();

		// 设置消息类型
		void setType(size_t type);
		
		// 返回客户端ID
		size_t getClientID();
		// 设置客户端ID
		void setClientID(size_t client_id);

		// 返回消息大小，以字节为单位，用于序列化
		virtual size_t getSize()  = 0;

		// 将消息数据结构序列化到字节缓冲区中，返回写入的字节数
		virtual size_t serialize(unsigned char *write_buffer)  = 0;

		// 从字节缓冲区中反序列化得到消息数据结构，返回读出的字节数
		virtual size_t deserialize(unsigned char *const read_buffer) = 0;

		// 析构函数
		virtual ~Message();
		

	protected:
		// 消息类型
		size_t _msg_type;

		// 客户端ID
		size_t _client_id;
	};

	// start消息，客户端->服务器
	// 格式：len,type,client_id
	// 作用：客户端连接上服务器后首先发送初始化消息，然后发送start消息用以通知服务器执行初始化工作
	class StartMessage : public Message {

	public:
		StartMessage();

		StartMessage(size_t client_id);
		

		StartMessage(StartMessage &msg);
		

		size_t getSize() ;
		int getAppType();

		size_t serialize(unsigned char *write_buffer) ;
			

		size_t deserialize(unsigned char *const read_buffer);
		
		virtual ~StartMessage();
		

	protected:
		int apptype;
		// nothing
	};

	// stop消息，客户端->服务器
	// 格式：len,type,client_id
	// 作用：客户端断开和服务器的连接前发送该消息，用于通知服务器执行析构工作
	class StopMessage : public Message {

	public:
		StopMessage() ;

		StopMessage(size_t client_id);
		StopMessage(StopMessage &msg);

		size_t getSize();

		size_t serialize(unsigned char *write_buffer);

		size_t deserialize(unsigned char *const read_buffer);

		virtual ~StopMessage();

	protected:
		// nothing
	};

	// camera消息，客户端->服务器
	// 格式：len,type,client_id,eyex,eyey,eyez,centerx,centery,centerz,upx,upy,upz
	// 作用：客户端连接上服务器后发送相机初始化消息，用于初始化相机参数
	class CameraMessage : public Message {

	public:
		CameraMessage();

		void getCameraArgs(Camera &camera);

		void setCameraArgs(const Camera &camera);

		size_t getSize();
		size_t serialize(unsigned char *write_buffer);

		size_t deserialize(unsigned char *const read_buffer);
		

		virtual ~CameraMessage();

	protected:
		// eye、center和up参数
		Camera _camera;
	};

	// perspective消息，客户端->服务器
	// 格式：len,type,client_id,fov,aspect,near,far
	// 作用：客户端连接上服务器后发送透视投影消息，用于初始化透视参数
	class PerspectiveMessage : public Message {

	public:
		PerspectiveMessage();

		void getPerspectiveArgs(Perspective &perspective);

		void setPerspectiveArgs(const Perspective &perspective);

		size_t getSize() ;

		size_t serialize(unsigned char *write_buffer) ;

		size_t deserialize(unsigned char *const read_buffer);

		virtual ~PerspectiveMessage();

	protected:
		// 透视投影参数
		Perspective _perspective;
	};

	// resolution消息，客户端->服务器
	// 格式：len,type,client_id,width,height
	// 作用：初始化视口大小，即分辨率
	class ResolutionMessage : public Message {

	public:
		ResolutionMessage() {
			_msg_type = INITIAL_RESOLUTION_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getResolutionArgs(Resolution &resolution) const{
			resolution = _resolution;
		}

		void setResolutionArgs(const Resolution &resolution) {
			_resolution = resolution;
		}

		size_t getSize() const{
			return 12+sizeof(Resolution);
		}

		size_t serialize(unsigned char *write_buffer) const{
			size_t len = getSize();
			write_uint(write_buffer, 0, &len);
			write_uint(write_buffer, 4, &_msg_type);
			write_uint(write_buffer, 8, &_client_id);

			write_int(write_buffer, 12, &_resolution._width);
			write_int(write_buffer, 16, &_resolution._height);
			return len;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			size_t len = getSize();

			read_int(read_buffer, 12, &_resolution._width);
			read_int(read_buffer, 16, &_resolution._height);
			return len;
		}

		virtual ~ResolutionMessage(){
			// do nothing
		}

	protected:
		// x和y方向分辨率
		Resolution _resolution;
	};

	// touch event消息，客户端->服务器
	// 格式：len,type,client_id,event_type,x1,y1,x2,y2
	class TouchEventMessage : public Message {

	public:
		TouchEventMessage();

		void getTouchEventArgs(TouchEvent &touch_event);

		void setResolutionArgs(const TouchEvent &touch_event);
		size_t getSize();
		
		size_t serialize(unsigned char *write_buffer);
		
		size_t deserialize(unsigned char *const read_buffer);

		virtual ~TouchEventMessage();

	protected:
		// android触摸屏事件参数
		TouchEvent _touch_event;
	};

	// ref_frame消息，服务器->客户端
	// 格式：len,type,client_id,matrix(4 by 4,float),color_buf_len,
	// depth_buf_src_len,depth_buf_dst_len,color_buf, depth_buf
	// 作用：服务器生成新的参考帧后，发送给客户端，用以更新客户端本地帧缓存
	class RefFrameMessage : public Message {

	public:
		RefFrameMessage() {
			_msg_type = RUNTIME_REF_FRAME_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getRefFrameArgs(RefFrame &frame) const {
			frame = _frame;
		}

		void setRefFrameArgs(const RefFrame &frame) {
			_frame = frame;
		}

		size_t getSize() const{
			return 12+sizeof(RefFrame);
		}

		size_t serialize(unsigned char *write_buffer) const{
			size_t len = getSize();
			write_uint(write_buffer, 0, &len);
			write_uint(write_buffer, 4, &_msg_type);
			write_uint(write_buffer, 8, &_client_id);

			// 写入矩阵元素
			strncpy((char*)(write_buffer+12), (char*)_frame._matrix, 16*sizeof(float));

			// 写入三个长度
			write_uint(write_buffer, 76, &_frame._color_buf_len);
			write_uint(write_buffer, 80, &_frame._depth_buf_src_len);
			write_uint(write_buffer, 84, &_frame._depth_buf_dst_len);

			// 写入颜色图像
			strncpy((char*)(write_buffer+88), (char*)_frame._color_buf, _frame._color_buf_len);

			// 写入深度图像
			strncpy((char*)(write_buffer+88+_frame._color_buf_len), 
				(char*)_frame._depth_buf, _frame._depth_buf_dst_len);

			return len;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			size_t len = getSize();
			// 读取矩阵元素
			strncpy((char*)_frame._matrix, (char*)(read_buffer+12), 16*sizeof(float));

			// 读出三个长度
			read_uint(read_buffer, 76, &_frame._color_buf_len);
			read_uint(read_buffer, 80, &_frame._depth_buf_src_len);
			read_uint(read_buffer, 84, &_frame._depth_buf_dst_len);

			// 读出颜色图像
			strncpy((char*)_frame._color_buf, (char*)(read_buffer+88), _frame._color_buf_len);

			// 写入深度图像
			strncpy((char*)_frame._depth_buf, 
				(char*)(read_buffer+88+_frame._color_buf_len), _frame._depth_buf_dst_len);

			return len;
		}

		~RefFrameMessage(){
			// do nothing
		}

	protected:
		RefFrame _frame;
	};

	// view_query消息，ViewpointMgr->Viewbuffer Query
	// 作用：由视点管理模块生成，用于通知视点缓存查询模块执行缓存查询
	class ViewQueryMessage : public Message {

	public:
		ViewQueryMessage() {
			_msg_type = RUNTIME_VIEW_QUERY_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getViewQueryArgs(osg::Matrixf &view_matrix) const {
			view_matrix = _view_matrix;
		}

		void setViewQueryArgs(const osg::Matrixf &view_matrix) {
			_view_matrix = view_matrix;
		}

		size_t getSize() const {
			assert(0);
			return 0;
		}

		size_t serialize(unsigned char *write_buffer) const {
			assert(0);
			return 0;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			assert(0);
			return 0;
		}

		~ViewQueryMessage(){
			// do nothing
		}

	protected:
		osg::Matrixf _view_matrix; // view matrix
	};

	// scene_rendering消息，Viewbuffer Query->Scene Renderer
	// 作用：由视点缓存查询模块生成，用于通知场景渲染模块执行渲染
	class SceneRenderingMessage : public Message {

	public:
		SceneRenderingMessage() {
			_msg_type = RUNTIME_SCENE_RENDERING_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getSceneRenderingArgs(osg::Matrixf &view_matrix) const{
			view_matrix = _view_matrix;
		}

		void setSceneRenderingArgs(const osg::Matrixf &view_matrix) {
			_view_matrix = view_matrix;
		}

		size_t getSize() const{
			assert(0);
			return 0;
		}

		size_t serialize(unsigned char *write_buffer) const{
			assert(0);
			return 0;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			assert(0);
			return 0;
		}

		~SceneRenderingMessage(){
			// do nothing
		}

	protected:
		osg::Matrixf _view_matrix;
	};

	// image warping消息，Viewbuffer Query->Image Warper
	// 作用：由视点缓存查询模块生成，用于通知图像合成模块执行合成
	class ImageWarpingMessage : public Message {

	public:
		ImageWarpingMessage() {
			_msg_type = RUNTIME_IMAGE_WARPING_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getImageWarpingArgs(vector<RawRefFrame> &raw_frame_vector) const {
			raw_frame_vector = _raw_frame_vector;	
		}

		void setImageWarpingArgs(vector<RawRefFrame> &raw_frame_vector) {
			_raw_frame_vector = raw_frame_vector;
		}

		size_t getSize() const{
			assert(0);
			return 0;
		}

		size_t serialize(unsigned char *write_buffer) const{
			assert(0);
			return 0;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			assert(0);
			return 0;
		}

		~ImageWarpingMessage(){
			// do nothing
		}

	protected:
		vector<RawRefFrame> _raw_frame_vector;
	};

	// view update消息，Scene Renderer->Viewbuffer Update
	// 作用：由场景渲染模块生成，用于通知视点缓存更新模块执行视点缓存更新
	class ViewUpdateMessage : public Message {

	public:
		ViewUpdateMessage() {
			_msg_type = RUNTIME_VIEW_UPDATE_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getViewbufferUpdateArgs(RawRefFrame &raw_frame) const {
			raw_frame = _raw_frame;
		}

		void setViewbufferUpdateArgs(RawRefFrame &raw_frame) {
			_raw_frame = raw_frame;
		}

		size_t getSize() const {
			assert(0);
			return 0;
		}

		size_t serialize(unsigned char *write_buffer) const {
			assert(0);
			return 0;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			assert(0);
			return 0;
		}

		~ViewUpdateMessage(){
			// do nothing
		}

	protected:
		RawRefFrame _raw_frame; // view matrix,color image,depth texture
	};

	// ref encoding消息，Scene Renderer|Image Warper->Image Encoder
	// 作用：由场景渲染模块或者图像合成模块生成，用于通知图像编码模块执行编码
	class RefEncodingMessage : public Message {

	public:
		RefEncodingMessage() {
			_msg_type = RUNTIME_REF_ENCODING_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getRefEncodingArgs() const {
				
		}

		void setRefEncodingArgs() {
				
		}

		size_t getSize() const {
			assert(0);
			return 0;
		}

		size_t serialize(unsigned char *write_buffer) const {
			assert(0);
			return 0;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			assert(0);
			return 0;
		}

		~RefEncodingMessage(){
			// do nothing
		}

	protected:
		// 
	};

	// log消息，xxx->Logger
	// 作用：由所有模块生成交给日志线程，用于显示日志
	class LogMessage : public Message {

	public:
		LogMessage() {
			_msg_type = RUNTIME_LOG_MSG;
			_client_id = INVALID_CLIENT_ID;
		}

		void getLogArgs(LogRecord &log) const {
			log = _log;
		}

		void setLogArgs(const LogRecord &log) {
			_log = log;
		}

		size_t getSize() const {
			assert(0);
			return 0;
		}

		size_t serialize(unsigned char *write_buffer) const {
			assert(0);
			return 0;
		}

		size_t deserialize(unsigned char *const read_buffer) {
			assert(0);
			return 0;
		}

		~LogMessage(){
			// do nothing
		}

	protected:
		LogRecord _log;
	};
	//2014_7_7
	class InteractionMessage: public Message
	{
	public:
		InteractionMessage();

		void getInteractionType(InteractionType &interaction);

		void setInteractionType(const InteractionType &interaction);

		size_t getSize();
		size_t serialize(unsigned char *write_buffer);

		size_t deserialize(unsigned char *const read_buffer);


		virtual ~InteractionMessage(){};

	protected:
		InteractionType _it;
	};
	class StringMessage: public Message
	{
	public:
		StringMessage(size_t s);
		void setSize(size_t s);
		string getString();
		size_t getSize();
		size_t deserialize(unsigned char * const read_buffer);
		size_t serialize(unsigned char * write_buffer);
		virtual ~StringMessage(){};
	protected:
		string _str;
		size_t _strsize;
	};

#endif