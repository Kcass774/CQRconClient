#ifndef ASIO_TCPSERVER_H_

#include <functional>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "TcpConnection.h"

namespace tcp {
	typedef tcp::TcpConnection connection_type;
	typedef std::shared_ptr<connection_type> connection_ptr;

	class TcpServer :public std::enable_shared_from_this<TcpServer>
	{
		typedef boost::asio::ip::tcp::acceptor acceptor_type;
		typedef boost::asio::ip::address address_type;
		typedef std::function<void(const boost::system::error_code&, connection_ptr)> NewConnectedCallBack;
	public:
		TcpServer(io_type* io, const endpoint_type& endpoint);
		~TcpServer();

		void Start_accept();
		inline void Close(ec_type* ec) {
			this->m_io->post([this]() {
				this->m_acceptor.close();
				});
		};
		inline bool isListen() { return this->m_acceptor.is_open(); };

		inline void setNewConnectedCallBack(NewConnectedCallBack callback) { this->m_NewConnectedCallBack_ = callback; };

	private:
		void accept_handler(const boost::system::error_code& ec, connection_ptr sock);

		std::vector<connection_ptr> m_vecConnection;

		NewConnectedCallBack m_NewConnectedCallBack_ = nullptr;
		io_type* m_io = nullptr;
		acceptor_type m_acceptor;
		char* m_buf = nullptr;
	};
}

#endif // !TCPSERVER_H_