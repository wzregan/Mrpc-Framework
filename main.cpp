#include <muduo/base/Logging.h>
using namespace muduo;
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <rpcheader.pb.h>
#include <memory>
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
  char d[100];
  int len = 0;
  int *header_size =(int*)d;
  len+=4;
  char * servicename = d+len;
  strcpy(servicename,"UserService");
  len=1+len+sizeof("UserService");
  char * methodname = d+len;
  strcpy(servicename,"Login");

  len=1+len+sizeof("Login");
  std::cout<<len<<","<<d+4<<d+10;  
  *header_size = len;

  send(sock_fd,(void*)&d,len+4,0);
  return 0;
}

