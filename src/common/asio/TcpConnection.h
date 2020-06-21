#ifndef ASIO_TCPCONNECTION_H_
#define ASIO_TCPCONNECTION_H_

#include <functional>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace tcp {
	typedef boost::asio::io_context io_type;
	typedef boost::asio::ip::tcp::socket sock_type;
	typedef std::shared_ptr<sock_type> sock_ptr;
	typedef boost::asio::ip::tcp::endpoint endpoint_type;
	typedef boost::system::error_code ec_type;

	typedef std::function<void(const ec_type&)> CloseCallBack;
	typedef std::function<void(const ec_type&, const size_t& size)> WriteCallBack;
	typedef std::function<void(const char*, const size_t& size)> MessageCallBack;

	class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
	public:
		TcpConnection(io_type* io);
		~TcpConnection();

		inline void Close() { 
			this->m_io->post([this]() {
				this->m_sock->close();
				});
		};
		inline bool isOpen() { return this->m_IsOpen; };

		void Start_Accept();
		inline void Write(const char* data, const size_t size, ec_type* ec, WriteCallBack callback_ = nullptr) {
			this->m_sock->async_write_some(boost::asio::buffer(data, size),
				boost::bind(&tcp::TcpConnection::write_handler, this,
					boost::placeholders::_1, boost::placeholders::_2, callback_));
		};

		inline sock_ptr getSockPtr() { return this->m_sock; }
		inline io_type* getIo_context() { return this->m_io; };
		inline endpoint_type getLocalEndpoint() { return this->m_sock->local_endpoint(); }
		inline endpoint_type getRemoteEndpoint() { return this->m_sock->remote_endpoint(); };

		void initBuffer(const size_t& size = 4096);
		inline void setMessageCallBack(MessageCallBack callback) { this->m_MessageCallBack_ = callback; };
		inline void setCloseCallBack(CloseCallBack callback) { this->m_CloseCallBack_ = callback; };

	private:
		inline void write_handler(const boost::system::error_code& ec, size_t length, WriteCallBack callback_) { if (callback_) callback_(ec, length); };
		void read_handler(const boost::system::error_code& ec, size_t length);

		io_type* m_io;
		sock_ptr m_sock;

		bool m_IsOpen = false;

		CloseCallBack m_CloseCallBack_ = nullptr;
		MessageCallBack m_MessageCallBack_ = nullptr;

		char* m_buf = nullptr;
	};
}
#endif // !TCPCONNECTION_H_
