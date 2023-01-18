#pragma once
#include <google/protobuf/service.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <memory>
#include <iostream>
#include <google/protobuf/descriptor.h>
#include <functional>
#include <unordered_map>
#include <vector>
#include "mrpcapplication.h"
#include <nlohmann/json.hpp>
using muduo::net::TcpConnectionPtr;
using muduo::net::Buffer;
using muduo::Timestamp;
// RPC服务提供
class Rpcprovider
{
private:
    /* data */
public:
    Rpcprovider(/* args */);
    ~Rpcprovider();
    void NotifyService(google::protobuf::Service *service);
    void Run(); // 启动RPC服务节点，开始提供RPC远程网络调用服务
    void OnConnection(const TcpConnectionPtr&);
    void OnMessage(const TcpConnectionPtr&,Buffer*,Timestamp);
    struct ServiceInfo
    {
        google::protobuf::Service * m_services; // 保存服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> m_methods; // 保存服务方法
    };
    std::unordered_map<std::string,ServiceInfo> m_servicveMap;
    void ReponseCallBack(const TcpConnectionPtr& ptr,google::protobuf::Message *reponse);
private:
    std::unique_ptr<muduo::net::TcpServer> m_tcpserverPtr;
    muduo::net::EventLoop m_loop;
};