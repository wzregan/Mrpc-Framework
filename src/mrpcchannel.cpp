#include "mrpcchannel.h"
#include <muduo/net/TcpClient.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <mrpcapplication.h>
#include <memory>
#include "rpcheader.pb.h"
#include "ZkClient.h"
#include <stdio.h>
using _unit8 = unsigned char;
void MrpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                                    google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                                    google::protobuf::Message* response, google::protobuf::Closure* done) 
{
    int rscode;
    // 获取序列化长度
    mrpc::RpcHeader rpcheader;
    rpcheader.set_method_name(method->name());
    rpcheader.set_service_name(method->service()->name());
    int request_size = request->ByteSizeLong();
    rpcheader.set_arg_size(request_size);

    int header_size = rpcheader.ByteSizeLong();

    // 获取序列化长度
    // 然后建立空间
    _unit8 * buffer = new _unit8[4+header_size+request_size];
    int * header_size_op =(int*) buffer;
    *header_size_op = header_size;

    rscode = rpcheader.SerializeToArray(buffer+4,header_size);
    if(request_size>0)
        rscode = request->SerializeToArray(buffer+4+header_size,request_size);

    if(rscode==0){
        controller->SetFailed("序列化失败...!");
        return;
    }
    string service_name = method->service()->name();
    int skfd = socket(PF_INET,SOCK_STREAM,0);

    MrpcConfig conf = MrpcApplication::Instance().config;
    sockaddr_in addr;
    // // 清空
    memset(&addr,0,sizeof(addr));

    ZkClient zc(conf.zookeeperip,conf.zookeeperport);
    zc.connect();
    string request_path = "/service/" + service_name;
    string hosts = zc.get(request_path.c_str());
    int idx = hosts.find(":");
    string  service_ip;
    int service_port;
    if(idx==std::string::npos){
        controller->SetFailed("服务未注册!");
        return;
    }else{
        service_ip = hosts. substr(0,idx);
        service_port = atoi(hosts.substr(idx+1,5).c_str());
    }
    // // 配置通信地址
    addr.sin_family = AF_INET;
    addr.sin_port = htons(service_port);
    addr.sin_addr.s_addr = inet_addr(service_ip.c_str());
    rscode = connect(skfd,(sockaddr*)&addr,sizeof(addr));

    if(rscode<0){
        controller->SetFailed("远程连接失败...");
        return;
    }
    rscode = send(skfd,buffer,header_size+request_size+4,0);
    if(rscode<0){
        controller->SetFailed("发起请求失败...");
        return;
    }
    _unit8 *res_data = new _unit8[1024];
    rscode = recv(skfd,res_data,1024,0);
    if(rscode<0){
        controller->SetFailed("接收返回值失败...");
        return;
    }
    response->ParseFromArray(res_data,rscode);
    if(done!=nullptr)
        done->Run();
}
