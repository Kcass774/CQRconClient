#include <set>

#include <cqcppsdk/cqcppsdk.h>

#include "rcon/client.h"

class cqrconclient : public rcon::client{
public:
    cqrconclient(tcp::io_type* io);

    void addTarget(const cq::Target& t);
    bool delTarget(const cq::Target& t);

    void sendMessage(const std::string& msg);

    inline void setMCMode(const bool& b) {
        this->m_MCMode = b;
    };

    inline void setConnectCallBack(tcp::ConnectCallBack) = delete;
    inline void setLoginSuccessCallback() = delete;
    inline void setErrorCallback(rcon::ERRORTYPE) = delete;
    inline void setsetResponseCallback(tcp::MessageCallBack) = delete;

private:
    bool m_MCMode = false;

    std::set<int64_t> m_setGroup;
    std::set<int64_t> m_setUser;
    std::set<int64_t> m_setDiscuss;
};