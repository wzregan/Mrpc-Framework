#include "mrpcapplication.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include "mrpclog.h"
MrpcApplication MrpcApplication::app;
void MrpcApplication::Init(int argc,char **args) {
    char c;
    std::string file_name;
    while((c=getopt(argc,args,"f:"))!=-1){
        switch (c)
        {
        case 'f':
            file_name = optarg;
            break;
        default:
            LOG_ERROR("input invalid option!\n");
            break;
        }
    }
    app.config.Load(file_name.c_str());
}

MrpcApplication& MrpcApplication::Instance() {
    return app;
}

MrpcApplication::MrpcApplication() {
    
}
