// ��Ϣ����������
// author����ɽ��
// date��2013-8-29

#ifndef _MESSAGE_QUEUE_H
#define _MESSAGE_QUEUE_H

#include "Global.h"
#include "Message.h"

	// �̼߳�ͨ����Ϣ����
	class MessageQueue {

	public:
		// ��Ϣ���
		void push( Message *msg) {
			boost::mutex::scoped_lock lock(_mutex);
			_queue.push(msg);
			lock.unlock();
			_condition_variable.notify_one();
		}

		// �������Ƿ��
		bool empty() const {
			boost::mutex::scoped_lock lock(_mutex);
			return _queue.empty();
			
		}

		// ���Գ���һ����Ϣ��������зǿ��򷵻�true������false
		bool try_pop(Message *&msg) {
			boost::mutex::scoped_lock lock(_mutex);
			if(_queue.empty()) {
				return false;
			}

			msg=_queue.front();
			_queue.pop();
			return true;
		}

		// ����ʽ�س���һ����Ϣ���������Ϊ����һֱ����ȥ��ֱ�����зǿ�
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