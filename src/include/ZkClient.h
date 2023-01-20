//
// Created by regan on 2023/1/20.
//

#ifndef MAIN_ZKCLIENT_H
#define MAIN_ZKCLIENT_H
#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient {
public:
    ZkClient();
    ~ZkClient();
public:
    void connect();
    std::string get(const char * path);
    void create(const char *path, const char *data, int datalen);

public:
    zhandle_t  *m_handle;

};


#endif //MAIN_ZKCLIENT_H
