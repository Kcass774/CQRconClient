#ifndef ASIO_TCPSERVER_CPP_
#define ASIO_TCPSERVER_CPP_

#include "TcpServer.h"

tcp::TcpServer::TcpServer(io_type* io, const endpoint_type& endpoint)
	:m_acceptor(*io, endpoint)
{
	this->m_io = io;
}

tcp::TcpServer::~TcpServer() {

}

void tcp::TcpServer::Start_accept()
{
	connection_ptr conn(new connection_type(this->m_io));
	conn->initBuffer();
	auto sock = conn->getSockPtr();
	this->m_vecConnection.push_back(conn);
	m_acceptor.async_accept(*sock, boost::bind(&tcp::TcpServer::accept_handler, this, boost::placeholders::_1, conn));
}

void tcp::TcpServer::accept_handler(const boost::system::error_code& ec, connection_ptr conn)
{
	conn->Start_Accept();
	if (this->m_NewConnectedCallBack_)
		this->m_NewConnectedCallBack_(ec, conn);
	if (ec)
		return;
	this->Start_accept();
}

#endif // !TCPSERVER_CPP_