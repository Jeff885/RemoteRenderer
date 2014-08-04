#include "Global.h"
#include "Server.h"
#include "RemoteViewer.h"
#include "SceneMgr.h"

osg::ref_ptr<RemoteViewer> _remoteviewer;
server::server(short port, int thread_cnt)
	: io_service_pool_(thread_cnt)
	, acceptor_(io_service_pool_.get_io_service(), tcp::endpoint(boost::asio::ip::address_v4::from_string("219.224.168.81"), port))
{
	Session* new_Session = new Session(io_service_pool_.get_io_service());
	acceptor_.async_accept(new_Session->socket(),
		boost::bind(&server::handle_accept, this, new_Session, boost::asio::placeholders::error));
}

void server::handle_accept(Session* new_Session, const boost::system::error_code& error)
{
	cout<<"handle_accept"<<endl;
	if (!error)
	{
		std::cout<<"new_Session start"<<std::endl;
		//�������ӣ�����һ��sessionά��������
		new_Session->start();
	}
	else
	{
		//����session
		cout<<" handle_accept error"<<endl;
		delete new_Session;
	}
	cout<<"�ȴ���һ���ͻ��˵ĵ���"<<endl;
	//����һ���첽�����ȴ���Session���ȴ���һ���Ự�ĵ���
	new_Session = new Session(io_service_pool_.get_io_service());
	acceptor_.async_accept(new_Session->socket(),
		boost::bind(&server::handle_accept, this, new_Session, boost::asio::placeholders::error));
}

void server::run()
{
	Scene_Mgr::instance();
	rv=new RemoteViewer();
	_remoteviewer=rv;
	boost::thread thd(boost::bind(RemoteViewer::start,rv));
	thd.detach();
	io_service_pool_.start();
	io_service_pool_.join();
}