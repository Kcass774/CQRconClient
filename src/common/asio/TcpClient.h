#ifndef ASIO_TCPCLIENT_H_
#define ASIO_TCPCLIENT_H_

#include <functional>
#include <string>

#include <boost/asio.hpp>

#include <asio/TcpConnection.h>

namespace tcp {

	typedef std::function<void(const boost::system::error_code&)> ConnectCallBack;

	class TcpClient : public TcpConnection, std::enable_shared_from_this<TcpClient>
	{
	public:
		TcpClient(io_type* io);
		~TcpClient();

		void Start_Accept() = delete;
		inline void Connect(const boost::asio::ip::tcp::endpoint& endpoint) {
			this->getSockPtr()->async_connect(endpoint,
				std::bind(&tcp::TcpClient::conn_handler, this, std::placeholders::_1));
		};

		inline void setConnectCallBack(ConnectCallBack callback) { this->m_ConnectCallBack_ = callback; };
	private:
		void conn_handler(const boost::system::error_code& ec);

		ConnectCallBack m_ConnectCallBack_ = nullptr;
	};
}

#endif // !TCPCLIENT_H_