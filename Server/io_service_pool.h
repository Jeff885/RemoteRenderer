#ifndef IO_SERVICE_POOL
#define IO_SERVICE_POOL
//author:������
//io_service_pool��  �̳߳أ�֧�ֿͻ��˶Է������˵Ĳ�������
#include "Global.h"
#include <boost/thread.hpp>
class io_service_pool : public boost::noncopyable
{
public:
	explicit io_service_pool(std::size_t pool_size):next_io_service(0)
	{
		for(std::size_t i=0;i<pool_size;i++)
		{
			io_service_sptr io_service(new boost::asio::io_service);
			work_sptr work(new boost::asio::io_service::work(*io_service));
			io_services.push_back(io_service);
			works.push_back(work);
		}

	}
	void start()
	{
		for(std::size_t i=0;i<io_services.size();i++)
		{
			//����io_service��run�������߳�
			boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run,io_services[i])));
			threads.push_back(thread);

		}
	}
	void join()
	{
		cout<<"�������������"<<endl;
		for(std::size_t i=0;i<threads.size();i++)
		{
			cout<<"join"<<i<<endl;
			cout<<"�ȴ��ͻ�������������...."<<endl;
			threads[i]->join();

		}
	}
	void stop()
	{
		for(std::size_t i=0;i<io_services.size();i++)
		{
			io_services[i]->stop();
		}
	}
	boost::asio::io_service& get_io_service()
	{
		boost::mutex::scoped_lock lock(mtx);
		boost::asio::io_service& io_serv=*io_services[next_io_service];
		++next_io_service;
		if(next_io_service == io_services.size())
		{
			next_io_service =0;
		}
		return io_serv;
	}
private:
	typedef boost::shared_ptr<boost::asio::io_service> io_service_sptr;
	typedef boost::shared_ptr<boost::asio::io_service::work> work_sptr;
	typedef boost::shared_ptr<boost::thread> thread_sptr;

	boost::mutex mtx;

	std::vector<io_service_sptr> io_services;
	std::vector<work_sptr> works;
	std::vector<thread_sptr> threads;
	std::size_t next_io_service;

};
#endif