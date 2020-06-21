#ifndef ASIO_TCPCONNECTION_CPP_
#define ASIO_TCPCONNECTION_CPP_

#include "TcpConnection.h"

tcp::TcpConnection::TcpConnection(io_type* io)
	:m_sock(new sock_type(*io))
{
	this->m_io = io;
}

tcp::TcpConnection::~TcpConnection()
{
	if (this->m_buf) {
		free(this->m_buf);
	}
}

void tcp::TcpConnection::Start_Accept()
{
	this->m_IsOpen = true;
	this->m_sock->set_option(boost::asio::socket_base::keep_alive(true));
	this->m_sock->async_read_some(boost::asio::buffer(m_buf, 4096), boost::bind(&tcp::TcpConnection::read_handler, this, boost::placeholders::_1, boost::placeholders::_2));
}

void tcp::TcpConnection::initBuffer(const size_t& size)
{
	this->m_buf = (char*)malloc(sizeof(char) * size);
}

void tcp::TcpConnection::read_handler(const boost::system::error_code& ec, size_t length)
{
	if (ec.failed())
	{
		this->m_IsOpen = false;
		if (this->m_CloseCallBack_)
		{
			this->m_CloseCallBack_(ec);
		}
		return;
	}
	if (this->m_MessageCallBack_)
		this->m_MessageCallBack_(this->m_buf, length);
	this->m_sock->async_read_some(boost::asio::buffer(m_buf, 4096), boost::bind(&tcp::TcpConnection::read_handler, this, boost::placeholders::_1, boost::placeholders::_2));
}

#endif // !TCPCONNECTION_CPP_
