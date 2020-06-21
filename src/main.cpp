#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

#include <cqcppsdk/cqcppsdk.h>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <yaml-cpp/yaml.h>

#include "cqrconclient.h"

tcp::io_type io;
tcp::io_type::work work(io);

struct configServer {
    std::string name;
    std::string ip;
    int port;
    std::set<uint64_t> ops;
    bool mcmode;
};

struct configMain {
    std::string prefix;
    std::vector<configServer> servers;
};

struct client {
    configServer config;
    std::unique_ptr<cqrconclient> rconclient;
};
std::map<std::string, client> map_servers;

void operator<<(configMain& config, const YAML::Node& node) {
    config.prefix = node["prefix"].as<std::string>();
    auto servers = node["servers"];
    for (size_t i = 0; i < servers.size(); ++i) {
        std::set<uint64_t> setTmpops;
        auto arrayOps = servers[i]["ops"];
        for (size_t j = 0; j < arrayOps.size(); ++j) setTmpops.insert(arrayOps[j].as<uint64_t>());

        config.servers.push_back({
            servers[i]["name"].as<std::string>(),
            servers[i]["ip"].as<std::string>(),
            servers[i]["port"].as<int>(),
            setTmpops,
            servers[i]["mcmode"].as<bool>(),
        });
    }
}

configMain Config;

CQ_INIT {
    on_enable([] {
        new std::thread([] { io.run(); });
        std::string strPath = cq::dir::app() + "config.json";
        //std::string strPath = R"(J:\config.yaml)";
        try {
            YAML::Node docRoot = YAML::LoadFile(strPath);
            try {
                Config << docRoot;
                for (auto& x : Config.servers) {
                    auto ptrClient = std::unique_ptr<cqrconclient>(new cqrconclient(&io));
                    ptrClient->initBuffer();
                    ptrClient->setMCMode(x.mcmode);
                    map_servers[x.name] = {{x.name, x.ip, x.port, x.ops, x.mcmode}, std::move(ptrClient)};
                }
                logging::info("Rcon客户端", "插件已启用");
            } catch (const YAML::InvalidNode& e) {
                logging::error("Rcon客户端", "配置文件错误:(");
            }

        } catch (const YAML::ParserException& e) {
            std::string strMsg = "解析配置文件失败:";
            strMsg.append(e.what());
            logging::error("Rcon客户端", strMsg);
        } catch (const YAML::BadFile& e) {
            logging::error("Rcon客户端", "打开配置文件失败:(");
        }
    });

    on_disable([] {
        io.stop();
        io.reset();
        //断开所有连接
        for (auto iter = map_servers.begin(); iter != map_servers.end(); iter++) {
        }
        map_servers.clear();
    });

    on_message([](const MessageEvent& e) {
        if (e.message.find(Config.prefix) == 0) {
            int index1 = e.message.find(" ");
            if (index1 != std::string::npos && index1 != e.message.size() - 1) {
                auto index2 = map_servers.find(e.message.substr(1, index1 - 1));
                if (index2 != map_servers.end()) {
                    auto& ser = index2->second;
                    if (ser.config.ops.count(e.user_id) != 0) {
                        int index3 = e.message.find(" ", index1 + 1);
                        if (index3 == std::string::npos) index3 = e.message.size();
                        std::string cmd = e.message.substr(index1 + 1, index3 - index1 - 1);
                        if (cmd == "connect") {
                            if (ser.rconclient->isOpen()) {
                                cq::send_message(e.target, "已经连接过了");
                            } else {
                                cq::send_message(e.target, "发起连接中....");
                                boost::asio::ip::tcp::endpoint endpoint(
                                    boost::asio::ip::address::from_string(ser.config.ip), ser.config.port);
                                ser.rconclient->Connect(endpoint);
                            }
                        } else if (cmd == "close") {
                            if (ser.rconclient->isOpen()) {
                                ser.rconclient->Close();
                                cq::send_message(e.target, "断开成功....");
                            } else {
                                cq::send_message(e.target, "还没连接呢");
                            }
                        } else if (cmd == "login") {
                            if (ser.rconclient->isOpen()) {
                                if (ser.rconclient->IsLogined()) {
                                    cq::send_message(e.target, "已经登陆过了");
                                } else {
                                    cq::send_message(e.target, "登录中....");
                                    ser.rconclient->Login(e.message.substr(index3 + 1, e.message.size() - index3 - 1));
                                }
                            } else {
                                cq::send_message(e.target, "还没连接呢");
                            }
                        } else if (cmd == "command") {
                            if (ser.rconclient->isOpen()) {
                                if (ser.rconclient->IsLogined()) {
                                    cq::send_message(e.target, "执行指令中....");
                                    ser.rconclient->SendCommand(
                                        e.message.substr(index3 + 1, e.message.size() - index3 - 1));
                                } else {
                                    cq::send_message(e.target, "还没登录呢");
                                }
                            } else {
                                cq::send_message(e.target, "还没连接呢");
                            }
                        } else if (cmd == "add") {
                            ser.rconclient->addTarget(e.target);

                            cq::send_message(e.target, "添加目标成功");
                        } else if (cmd == "del") {
                            if (ser.rconclient->delTarget(e.target)) {
                                cq::send_message(e.target, "删除目标成功");
                            } else {
                                cq::send_message(e.target, "删除目标失败");
                            }
                        } else {
                            cq::send_message(e.target, "没有这条指令");
                        }
                    } else {
                        cq::send_message(e.target, "你没有该服务器的权限");
                    }
                    e.block();
                }
            }
        }
    });
}