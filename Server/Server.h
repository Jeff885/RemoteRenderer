#ifndef SERVER
#define SERVER
#include "io_service_pool.h"
#include "Session.h"
#include "RemoteViewer.h"
class RemoteViewer;
class server
{
public:
	server(short port, int thread_cnt);
		

	void handle_accept(Session* new_Session, const boost::system::error_code& error);
	

	void run();
	

private:

	io_service_pool io_service_pool_;
	tcp::acceptor acceptor_;
	RemoteViewer* rv;
};
#endif
