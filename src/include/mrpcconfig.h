#pragma once
#include <unordered_map>
#include <string>
using namespace std;
class MrpcConfig{
public:
    void Load(const char * filename);
public:
    string rpcip;
    int rpcport;
    string zookeeperip;
    int zookeeperport;
};