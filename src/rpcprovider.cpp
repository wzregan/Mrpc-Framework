#include "rpcprovider.h"
#include "rpcheader.pb.h"
using namespace std;

Rpcprovider::Rpcprovider(/* args */) {
    
}

Rpcprovider::~Rpcprovider() {
    
}


void Rpcprovider::Run() {
    cout<<"runing.....\n";
    MrpcConfig config = MrpcApplication::Instance().config;
    string ip = config.rpcip;
    int port = config.rpcport;
    // 创建一个server对象
    muduo::net::TcpServer server(&m_loop,muduo::net::InetAddress(ip,port),"RPC_PROVIDER");
    // 设置连接回调函数
    server.setConnectionCallback(std::bind(&Rpcprovider::OnConnection,this,std::placeholders::_1));
    // 设置消息回调
    server.setMessageCallback(std::bind(&Rpcprovider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    // 设在muduo库的线程数量
    server.setThreadNum(4); 
    //启动服务
    server.start();
    //启动循环
    m_loop.loop();
}
void Rpcprovider::OnConnection(const TcpConnectionPtr& conn){
    if(!conn->connected()){
        conn->shutdown();
    }
}
void Rpcprovider::OnMessage(const TcpConnectionPtr& conn,Buffer* buffer,Timestamp ts){
    cout<<"有消息来了:\n";
    //首先把消息取出来
    std::string msg = buffer->retrieveAllAsString();
    // 从消息头中获取headersize
    uint32_t header_size;
    // 获取到header_size之后直把header取出来进行反序列化
    msg.copy((char *)&header_size,4,0);
    const char * header_data = msg.substr(4,header_size).c_str();
    mrpc::RpcHeader header;
    if(header.ParseFromArray(header_data,header_size)){
        std::string s_name = header.service_name();
        std::string m_name = header.method_name();
        int argsize = header.arg_size();
        std::string args_stream = msg.substr(4+header_size,argsize);

        std::cout<<header_size<<std::endl;
        std::cout<<s_name<<std::endl;
        std::cout<<m_name<<std::endl;
        std::cout<<argsize<<std::endl;
    }else{
        cout<<"解析请求失败...\n";
    }

}

void Rpcprovider::NotifyService(google::protobuf::Service *service){
    Rpcprovider::ServiceInfo info;

    const google::protobuf::ServiceDescriptor* descriptor = service->GetDescriptor();

    // 获取服务的名字
    string service_name = descriptor->name();
    info.m_services = service;
    // 获取service的数量
    int methods_count = descriptor->method_count();
    for(int i = 0;i < methods_count;i++){
        const google::protobuf::MethodDescriptor * method = descriptor->method(i);
        info.m_methods.insert({method->name(),method});
    }  
    m_servicveMap.insert({service_name,info});
}
