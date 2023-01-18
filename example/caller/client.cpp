#include <iostream>
#include "mrpcapplication.h"
#include "user.pb.h"
#include "mrpcchannel.h"
using  std::cout;
using  std::endl;

int main(int argc,char** argv){
    cout<<"hello world!";
    MrpcApplication::Init(argc,argv);
    MrpcChannel *channel = new MrpcChannel();
    fixbug::UserServiceRpc_Stub uss(channel);
    fixbug::LoginRequest request;
    request.set_name("regan");
    request.set_pwd("woainima..");
    fixbug::LoginResponse response;
    
    cout<<"what:"<<response.ByteSizeLong()<<"\n";
    uss.Login(nullptr,&request,&response,nullptr);
    cout<<"response:errorcode="<<response.errorcode()<<",ms="<<response.msg();
    return 0;
}