#include <algorithm>

#include "cqrconclient.h"

cqrconclient::cqrconclient(tcp::io_type* io) 
    :rcon::client(io)
{
    client::setConnectCallBack([this](const tcp::ec_type& ec) { 
        if (ec) {
            this->sendMessage("连接服务器错误:" + ec.message());
        } else {
            this->sendMessage("连接服务器成功");
        }
        });
    client::setLoginSuccessCallback([&]() {
        this->sendMessage("登录成功");
    });
    client::setErrorCallback([&](rcon::ERRORTYPE e) {
        switch(e)
        {
        case rcon::ERRORTYPE::PASSWORDWRONG:
                this->sendMessage("密码错误");
                break;
        case rcon::ERRORTYPE::WRONGSIZE:
            this->sendMessage("包大小错误");
            break;
        }
    });
    client::setResponseCallback([this](const char* buf, size_t bufsize) {
        if(bufsize) 
        {
            std::string str_ret(buf, bufsize);
            if (this->m_MCMode)
                for (int index = str_ret.find("§"); index != std::string::npos; index = str_ret.find("§", index))
                    str_ret.erase(index, 3);
            this->sendMessage(str_ret);
        }
    });
}

void cqrconclient::addTarget(const cq::Target& t) {
    if (t.is_group()) this->m_setGroup.insert(*t.group_id);
    if (t.is_discuss()) this->m_setDiscuss.insert(*t.discuss_id);
    if (t.is_private()) this->m_setUser.insert(*t.user_id);
}

bool cqrconclient::delTarget(const cq::Target& t) {
    if (t.is_group()) 
        return this->m_setGroup.erase(*t.group_id);
    if (t.is_discuss()) 
        return this->m_setDiscuss.erase(*t.discuss_id);
    if (t.is_private()) 
        return this->m_setUser.erase(*t.user_id);
}

void cqrconclient::sendMessage(const std::string& msg)
{
    for (auto& id : this->m_setUser) cq::send_private_message(id, msg);
    for (auto& id : this->m_setGroup) cq::send_group_message(id, msg);
    for (auto& id : this->m_setDiscuss) cq::send_discuss_message(id, msg);
}