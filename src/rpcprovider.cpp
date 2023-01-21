#include "rpcprovider.h"
#include "rpcheader.pb.h"
#include "mrpclog.h"
#include "ZkClient.h"
#include "mrpclog.h"
using namespace std;
using _unite8 = unsigned char ;
Rpcprovider::Rpcprovider(/* args */) {
    
}

Rpcprovider::~Rpcprovider() {
    
}


void Rpcprovider::Run() {
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
    // 启动zookeeper服务
    ZkClient zc(config.zookeeperip,config.zookeeperport);
    zc.connect();
    string host = config.zookeeperip+":"+ to_string(config.rpcport);
    for(auto it = m_servicemap.begin(); it !=m_servicemap.end();it++){
        string serviceName = "/service/" + it->first;
        zc.create(serviceName.c_str(),host.c_str(),host.size());
    }
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
    LOG_DEBUG("有消息来了");
    //首先把消息取出来
    size_t readableSize = buffer->readableBytes();
    uint8_t * data = new uint8_t[readableSize];
    // 从消息头中获取headersize
    for(int i = 0;i< readableSize;i++){
        data[i] = buffer->readInt8();
    }
    int *header_size = (int*)data;

    uint8_t *header_data = data+4;

    
    cout<<"size:"<<*header_size<<"\n";
    mrpc::RpcHeader header;

    if(!header.ParseFromArray(header_data,*header_size)){
        cout<<"解析请求失败...\n";
        return;
    }
    std::string s_name = header.service_name();                                                                                                                                                                                                           
    std::string m_name = header.method_name();
    int argsize = header.arg_size();
    uint8_t *arg_data = header_data + (*header_size);
    std::cout<<header_size<<std::endl;
    std::cout<<s_name<<std::endl;
    std::cout<<m_name<<std::endl;
    std::cout<<argsize<<std::endl;
    // 然后在开始寻找服务和对应服务的方法
    auto it = m_servicemap.find(s_name);
    // if no service founded . . . . . .
    if(it == m_servicemap.end()){
        return;
    }
    google::protobuf::Service *service= it->second.m_services;
    // 生成request和reponse相应
    // 然后寻找对应的方法
    auto m_it = it->second.m_methods.find(m_name);
    if(m_it==it->second.m_methods.end()){
        return;
    }
    const google::protobuf::MethodDescriptor * method = m_it->second;
    //方法找到之后就可以开始序列化参数了
    google::protobuf::Message *m_request = service->GetRequestPrototype(method).New();
    m_request->ParseFromArray(arg_data,argsize);
    google::protobuf::Message *m_reponse = service->GetResponsePrototype(method).New();
    google::protobuf::Closure * closure = google::protobuf::NewCallback<Rpcprovider,const TcpConnectionPtr&,google::protobuf::Message*>(this,&Rpcprovider::ReponseCallBack,conn,m_reponse);
    
    service->GetResponsePrototype(method);
    service->CallMethod(method,nullptr,m_request,m_reponse,closure);
    
    
}
void Rpcprovider::ReponseCallBack(const TcpConnectionPtr& conn,google::protobuf::Message *response) {
    int bs = response->ByteSizeLong();
    cout<<"bs:"<<bs<<endl;
    uint8_t * data = new uint8_t[bs];
    bool isok = response->SerializeToArray(data,bs);
    if(isok){
        conn->send(data);
    }else{
        LOG_ERROR("序列化返回值失败");
    }
    conn->shutdown(); // 主动断开连接
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
    m_servicemap.insert({service_name, info});
}
