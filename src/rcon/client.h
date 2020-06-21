#ifndef RCON_CLIENT_H_
#define RCON_CLIENT_H_

#include <string>
#include <functional>
#include <thread>

#include <asio/TcpClient.h>

#define KEY "LOVE"

namespace rcon {
    enum class ERRORTYPE { 
        PASSWORDWRONG,
        WRONGSIZE
    };

    typedef std::function<void(ERRORTYPE)> ErrorCallback;
    typedef std::function<void()> LoginSuccessCallback;

    class client : public tcp::TcpClient {
    public:
        client(tcp::io_type* io);

        inline bool IsLogined() {
            return this->m_logined;
        };

        void Login(const std::string& password);
        void SendCommand(const std::string& command);

        inline void setMessageCallBack(tcp::MessageCallBack) = delete;
        inline void setCloseCallBack(tcp::CloseCallBack func) {
            this->m_ConnectCallBack = func;
        };
        inline void setLoginSuccessCallback(LoginSuccessCallback func) {
            this->m_LoginSuccessCallback = func;
        };
        inline void setResponseCallback(tcp::MessageCallBack func) {
            this->m_ResponseCallback = func;
        };
        inline void setErrorCallback(ErrorCallback func) {
            this->m_ErrorCallback = func;
        };

    private:
        bool m_logined = false;

        LoginSuccessCallback m_LoginSuccessCallback = nullptr;
        ErrorCallback m_ErrorCallback = nullptr;
        tcp::MessageCallBack m_ResponseCallback = nullptr;
        tcp::ConnectCallBack m_ConnectCallBack = nullptr;
    };
} // namespace rcon

#endif