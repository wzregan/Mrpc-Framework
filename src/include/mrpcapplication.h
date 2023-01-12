#pragma once
#include "mrpcconfig.h"
// RPC服务器初始化类
class MrpcApplication{
public:
    static void Init(int argc,char **args);
    static MrpcApplication& Instance();
    MrpcConfig config;

private:
    static MrpcApplication app;
    MrpcApplication();
    MrpcApplication(const MrpcApplication&) = delete;
    MrpcApplication(MrpcApplication&&) = delete;
};