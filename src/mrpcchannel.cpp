#include "mrpcchannel.h"
#include <muduo/net/TcpClient.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <mrpcapplication.h>
#include <memory>
#include "user.pb.h"
#include "rpcheader.pb.h"
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
    rscode = request->SerializeToArray(buffer+4+header_size,request_size);
    fixbug::LoginRequest *r = new fixbug::LoginRequest();

    r->ParseFromArray(buffer+4+header_size,request_size);
    cout<<r->pwd();
    if(rscode==0){
        controller->SetFailed("序列化失败...!");
        return;
    }
    int skfd = socket(PF_INET,SOCK_STREAM,0);
    MrpcConfig conf = MrpcApplication::Instance().config;
    sockaddr_in addr;
    // // 清空
    memset(&addr,0,sizeof(addr));
    
    
    // // 配置通信地址
    addr.sin_family = AF_INET;
    addr.sin_port = htons(conf.rpcport);
    addr.sin_addr.s_addr = inet_addr(conf.rpcip.c_str());
    
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
