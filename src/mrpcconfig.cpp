#include "mrpcconfig.h"
#include <fstream>
#include <nlohmann/json.hpp>

void MrpcConfig::Load(const char * filename) {
    std::fstream f = std::fstream(filename);
    nlohmann::json data = nlohmann::json::parse(f);
    rpcip = data["rpcserverip"].get<string>();
    rpcport = data["rpcserverport"].get<int>();
    zookeeperip = data["zookeeperip"].get<string>();
    zookeeperport = data["zookeeperport"].get<int>();
}
