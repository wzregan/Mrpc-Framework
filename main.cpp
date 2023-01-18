#include <muduo/base/Logging.h>
using namespace muduo;
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include "rpcheader.pb.h"
#include <memory>
#include <stdio.h>
#include <bitset>
typedef struct msg
{
  int header_size;
  char * data;
}Msg;

int main(){

  int sock_fd = socket(PF_INET,SOCK_STREAM,0);
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  int isok = connect(sock_fd,(sockaddr*)&addr,sizeof(addr));
  if(isok<0){
    std::cout<<"连接失败";
    return 0;
  }
  string s="okokokasdas";
  mrpc::RpcHeader h;
  h.set_service_name("regan");
  h.set_method_name("woaini");
  h.set_arg_size(100);

  string data = h.SerializeAsString();
  int header_size = 21;
  data = data.insert(0,(char*)&header_size,4);
  std::cout<<int(data.substr(0,4).c_str()[1]);

  send(sock_fd,(void*)&data,data.length()+2,0);

  return 0;
}

