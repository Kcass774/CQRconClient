#ifndef RCON_CLIENT_CPP_
#define RCON_CLIENT_CPP_
#include "client.h"

rcon::client::client(tcp::io_type* io) 
    : tcp::TcpClient(io) 
{
    TcpClient::setCloseCallBack([this](const tcp::ec_type& ec) {
        this->m_logined = false;
    });
    TcpClient::setMessageCallBack([this](const char* buf, const size_t& bufsize) {
        int size;
        memcpy(&size, buf, sizeof(size));
        if (!this->m_logined) {
            if (std::string(&buf[4], 4) == KEY) {
                this->m_logined = true;
                if (this->m_LoginSuccessCallback != nullptr) this->m_LoginSuccessCallback();
                return;
            } else {
                if (this->m_ErrorCallback) this->m_ErrorCallback(ERRORTYPE::PASSWORDWRONG);
            }
        }
        if (size == bufsize - 4) {
            if (m_ResponseCallback) this->m_ResponseCallback(&buf[12], bufsize - 14);
        } else {
            if (this->m_ErrorCallback) this->m_ErrorCallback(ERRORTYPE::WRONGSIZE);
        }
    });
}

void rcon::client::Login(const std::string& password) {
    std::string buf = KEY;
    int type = 3;
    buf.append((const char*)std::move(&type), sizeof(type));
    buf += password;
    buf += char(0);
    buf += char(0);
    int size = buf.size();
    std::string packet((const char*)std::move(&size), sizeof(size));
    packet += buf;
    tcp::ec_type ec;
    this->Write(packet.c_str(), packet.size(), &ec);
}

void rcon::client::SendCommand(const std::string& command) {
    std::string buf = KEY;
    int type = 2;
    buf.append((const char*)std::move(&type), sizeof(type));
    buf += command;
    buf += char(0);
    buf += char(0);
    int size = buf.size();
    std::string packet((const char*)std::move(&size), sizeof(size));
    packet += buf;
    tcp::ec_type ec;
    this->Write(packet.c_str(), packet.size(), &ec);
}

#endif