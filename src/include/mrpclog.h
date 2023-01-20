#pragma once
#include <queue>
#include <fstream>
#include <time.h>
#include "mrpcqueue.hpp"
#include <string>
#include <string.h>
#define DEFAULT_LOGLEVEL DEBUG

enum LogLevel{
    INFO,
    DEBUG,
    ERROR,
    LOGFILE
};
class MrpcLog{
public:
    void Log(std::string  msg);
    void setLevel(LogLevel level);

    static MrpcLog& Instance();
    MrpcLogQueue<std::string> m_queue;
private:
    static MrpcLog *m_uni;
    int m_level;
    std::string  filename;
    MrpcLog();
    MrpcLog(const MrpcLog&) = delete;
};
static void getFileName(char * buffer,const char * fn){
    const char * FN = fn;
    int idx = 0;
    for(int i=0;FN[i]!='\0';i++){
        if(FN[i]=='/'){
            idx = i;
        }
    }
    strcpy(buffer,FN+idx+1);
}


#define LOG_INFO(formatstr,...)\
        do\
        {\
            MrpcLog& loger = MrpcLog::Instance();\
            loger.setLevel(INFO); \
            char __buffer[512];\
            sprintf(__buffer,formatstr,##__VA_ARGS__);\
            loger.Log(__buffer);\
        }while (0);
#define LOG_DEBUG(formatstr,...)\
    do\
    {\
        MrpcLog& loger = MrpcLog::Instance();\
        loger.setLevel(DEBUG); \
        char __buffer[512];\
        char __newbuffer[512];\
        char __filename[64];\
        getFileName(__filename,__FILE__);\
        sprintf(__buffer,formatstr,##__VA_ARGS__);\
        strcat(__buffer,"\n%s:%d\n");\
        sprintf(__newbuffer,__buffer,__filename,__LINE__);\
        loger.Log(__newbuffer);\
    }while(0)
#define LOG_ERROR(formatstr,...)\
    do\
    {\
        MrpcLog& loger = MrpcLog::Instance();\
        loger.setLevel(ERROR); \
        char __buffer[512];\
        sprintf(__buffer,formatstr,##__VA_ARGS__);\
        loger.Log(__buffer);\
    }while(0)

#define LOG_FILE(formatstr,...)\
    do\
    {\
        MrpcLog& loger = MrpcLog::Instance();\
        loger.setLevel(LOGFILE);\
        char __buffer[512];\
        char __newbuffer[512];\
        char __filename[64];\
        getFileName(__filename,__FILE__);\
        sprintf(__buffer,formatstr,##__VA_ARGS__);\
        strcat(__buffer," <===%s:%d\n");\
        sprintf(__newbuffer,__buffer,__filename,__LINE__);\
        loger.Log(__newbuffer);\
    }while(0)

#define MRPCLOG(formatstr,...)\
    do\
    {\
        MrpcLog& loger = MrpcLog::Instance();\
        loger.setLevel(DEFAULT_LOGLEVEL); \
        char __buffer[512];\
        sprintf(__buffer,formatstr,##__VA_ARGS__);\
        loger.Log(__buffer);\
    }while(0)
