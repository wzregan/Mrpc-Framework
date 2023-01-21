//
// Created by regan on 2023/1/21.
//
#include "mrpcapplication.h"
#include "ZkClient.h"
#include "mrpclog.h"
#include <unistd.h>

int main(int argc,char **args){
    MrpcApplication::Init(argc,args);
    ZkClient zkc("127.0.0.1",2181);
    zkc.connect();
    zkc.create("/test3/test44","aaaaaaaa",sizeof("aaaaaaaa"));
    string data = zkc.get("/test3/test44");
//    zkc.create("/test1","hello,this is test1",sizeof("hello,this is test1"));
    LOG_DEBUG(data.c_str());
    sleep(3);
    return 0;
}