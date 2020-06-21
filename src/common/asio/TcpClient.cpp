#ifndef ASIO_TCPCLIENT_CPP_
#define ASIO_TCPCLIENT_CPP_

#include "TcpClient.h"

tcp::TcpClient::TcpClient(io_type* io)
    :TcpConnection(io)
{
}

tcp::TcpClient::~TcpClient()
{
}

void tcp::TcpClient::conn_handler(const boost::system::error_code& ec)
{
    if (!ec)
        TcpConnection::Start_Accept();
    if (this->m_ConnectCallBack_)
        this->m_ConnectCallBack_(ec);
}

#endif // !TCPCLIENT_CPP_