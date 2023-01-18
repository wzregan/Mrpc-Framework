#include "mrpclog.h"
#include <thread>
#include <iostream>
#include <fstream>
#define FG_RED "\033[1;31m"
#define FG_GREEN "\033[1;32m"
#define FG_YELLOW "\033[1;33m"
#define FG_WIGHT "\033[1;37m"
#define BG_RED "\033[1;41m"
#define BG_GREEN "\033[1;42m"
#define BG_YELLOW "\033[1;43m"
#define BG_WIGHT "\033[1;47m"
#define COLOR_NONE "\033[0;0m"
MrpcLog *MrpcLog::m_uni = nullptr;

void MrpcLog::Log(std::string  msg) {
    char time[64];
    std::time_t t = std::time(nullptr); // 时间
    tm* local_t = localtime(&t); // 获取本地时间
    sprintf(time,"[ %d月%d日 %d:%d:%d ]：",local_t->tm_mon+1,local_t->tm_mday,local_t->tm_hour,local_t->tm_min,local_t->tm_sec);

    if(m_level==INFO){
        // printf();
        std::cout<<FG_GREEN<<time<<msg<<COLOR_NONE <<"\n";
    }else if(m_level == DEBUG){
        std::cout<<FG_YELLOW<<time<<msg<<COLOR_NONE <<"\n";
    }else if(m_level == ERROR){
        std::cout<<FG_RED<<time<<msg<<COLOR_NONE <<"\n";
    }else if(m_level == LOGFILE){
        m_queue.push(msg);
    }
}

void MrpcLog::setLevel(LogLevel level) {
    m_level = level;
}
void MrpcLog::setLevel(LogLevel level,std::string file_dir){
    this->m_level = level;
}

MrpcLog& MrpcLog::Instance()
{   
    if(m_uni==nullptr){
        m_uni = new MrpcLog();
    }
    return *m_uni;
}

MrpcLog::MrpcLog()
{
    std::thread write_task([&](){
        char filename[128];
        
        std::time_t t = std::time(nullptr); // 时间
        tm* local_t = localtime(&t); // 获取本地时间
        sprintf(filename,"%d-%d-%d-start-log.txt",local_t->tm_year+1900,local_t->tm_mon+1,local_t->tm_mday);
        std::cout<<filename<<"\n";
        std::ofstream ofs;
        char time[32];
        while(true){
            // get time now
            std::time_t t = std::time(nullptr); // 时间
            tm* local_t = localtime(&t); // 获取本地时间
            sprintf(time,"[ %d月%d日 %d:%d:%d ]：",local_t->tm_mon+1,local_t->tm_mday,local_t->tm_hour,local_t->tm_min,local_t->tm_sec);
            std::string msg = this->m_queue.pop();
            ofs.open(filename,std::ios::app);
            ofs<<time<<msg;
            ofs.close();
        }
        
    });
    write_task.detach();
}
