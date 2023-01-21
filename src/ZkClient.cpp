//
// Created by regan on 2023/1/20.
//
#include "ZkClient.h"
#include "mrpcapplication.h"
#include <stdio.h>
#include <semaphore.h>
#include "mrpclog.h"
#include <zookeeper/zookeeper.h>
typedef struct SemMsg{
    sem_t sem;
    int rc;
    char data[512];
}SemMsg;

static void wathcer(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx){
    if(type == ZOO_SESSION_EVENT){
        // 如果回调函数state返回ZOO_CONNECTED_STATE，就说明连接成功
        if(state == ZOO_CONNECTED_STATE) {
            sem_t  *s= (sem_t*)zoo_get_context(zh);
            // 信号量+1
            sem_post(s);
        }
    }
}
static void completion_exists(int rc, const struct Stat *stat,const void *data){
    LOG_DEBUG("completion_exists 回调函数 %d %d",rc,ZNONODE);
    SemMsg *msg = (SemMsg *) data;
    msg->rc = rc;
    sem_post(&(msg->sem));
}
static void competion_create(int rc, const char *value, const void *data){
    LOG_DEBUG("competion_create 回调函数 %d %d",rc,ZNONODE);
    SemMsg *msg = (SemMsg *) data;
    msg->rc = rc;
    sem_post(&(msg->sem));
}
void competion_get(int rc, const char *value, int value_len,
                   const struct Stat *stat, const void *data){
    SemMsg *msg = (SemMsg *) data;

    strcpy(msg->data, value);

    msg->rc = rc;
    sem_post(&(msg->sem));

}

// 判断zookeeper的句柄是否为null
ZkClient::~ZkClient() {
    if(m_handle!= nullptr){
        zookeeper_close(m_handle);
    }
}
// 连接zook server
void ZkClient::connect() {
    string host = ip+":" + to_string(port);
    zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
    m_handle = zookeeper_init(host.c_str(),wathcer,30000,nullptr, nullptr,0);
    if(m_handle == nullptr){
        LOG_ERROR("Zookeeper初始化失败!");
        exit(1);
    }
    sem_t sem;
    sem_init(&sem,0,0);
    zoo_set_context(m_handle,&sem);
    //收到信号量
    sem_wait(&sem);
    LOG_INFO("Zookeeper connects successfully!");
}

std::string ZkClient::get(const char *path) {
    SemMsg msg;
    sem_init(&msg.sem,0,0);

    zoo_aget(this->m_handle,path,0,competion_get, &msg);
    sem_wait(&msg.sem);
    return string(msg.data);
}

void ZkClient::create(const char *path, const char *data, int datalen) {
    SemMsg msg;
    sem_init(&msg.sem,0,0);
    msg.rc = 100;
    int flag = zoo_aexists(this->m_handle,path,0, completion_exists, (void*)&msg);
    if(flag!=ZOK){
        LOG_ERROR("zookeeper API exists() faild!");
        exit(1);
    }
    sem_wait(&(msg.sem));
    if(msg.rc==ZOK){
        LOG_INFO("结点已经存在，无法再创建");
        return;
    }
    zoo_acreate(this->m_handle,path,data,datalen, &ZOO_OPEN_ACL_UNSAFE,ZOO_EPHEMERAL,competion_create,(void*)&msg);
    sem_wait(&(msg.sem));
    if(msg.rc==ZOK){
        LOG_INFO("结点创建成功!");
    }else{
        LOG_ERROR("结点创建失败，错误代码：%d",msg.rc);
    }
}

ZkClient::ZkClient(string ip, int port):m_handle(nullptr),ip(ip),port(port){

}


