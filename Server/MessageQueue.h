// 消息队列类声明
// author：胡山峰
// date：2013-8-29

#ifndef _MESSAGE_QUEUE_H
#define _MESSAGE_QUEUE_H

#include "Global.h"
#include "Message.h"

	// 线程间通信消息队列
	class MessageQueue {

	public:
		// 消息入队
		void push( Message *msg) {
			boost::mutex::scoped_lock lock(_mutex);
			_queue.push(msg);
			lock.unlock();
			_condition_variable.notify_one();
		}

		// 检查队列是否空
		bool empty() const {
			boost::mutex::scoped_lock lock(_mutex);
			return _queue.empty();
			
		}

		// 尝试出队一个消息，如果队列非空则返回true，否则false
		bool try_pop(Message *&msg) {
			boost::mutex::scoped_lock lock(_mutex);
			if(_queue.empty()) {
				return false;
			}

			msg=_queue.front();
			_queue.pop();
			return true;
		}

		// 阻塞式地出队一个消息，如果队列为空则一直等下去，直到队列非空
		void wait_and_pop(Message *&msg) {
			boost::mutex::scoped_lock lock(_mutex);
			while(_queue.empty()){
				_condition_variable.wait(lock);
			}

			msg=_queue.front();
			_queue.pop();
		}

	protected:
		std::queue<Message*> _queue;
		mutable boost::mutex _mutex;
		boost::condition_variable _condition_variable;
	};


#endif