#include <iostream>
#include <string>
#include "user.pb.h"
using namespace std;
#include "mrpcapplication.h"
#include "rpcprovider.h"
#include "mrpclog.h"
class UserService:public fixbug::UserServiceRpc{ // 注意这里是RPC服务端
    bool login(string name,string pwd){
        cout<<"login name:"<<name<<endl;
        cout<<"login pwd:"<<pwd<<endl;
        cout<<"executing login method!.....";
        return true;
    }

    // 1.caller  ==> Login(LoginRequest); ==> calle
    // 2. calle ==> Login(LoginRequest) ==> 交到重写的这个Login方法上了
    virtual void Login(google::protobuf::RpcController* controller,
                    const ::fixbug::LoginRequest* request,
                    ::fixbug::LoginResponse* response,
                    ::google::protobuf::Closure* done)
    {
        // 框架给业务上报了请求参数的LOginRequest，并且获取对应的数据做本地业务
        string name = request->name();
        string pwd = request->pwd();
        // 做本地业务
        bool login_result = login(name,pwd);
        // 获取本地业务并且得到响应
        response->set_errorcode(11);
        response->set_msg("successful!");

        // 执行回调函数，也就是说把对象数据的序列化进行网络发送
        done->Run();
    }
};

int main(int argc,char **argv){

    for(int i=0;i<10;i++){
        MRPCLOG("你好啊，你有点吊阿");
        sleep(1);
    }
    //1.调用框架初始化 MrpcApplication::init();
    MrpcApplication::Init(argc,argv);
    std::cout<<MrpcApplication::Instance().config.rpcip;
    // 2. 创建服务提供者
    Rpcprovider provider; 
    // 3. 添加服务
    provider.NotifyService(new UserService());

    provider.Run();
}