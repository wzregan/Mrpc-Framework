cmake_minimum_required(VERSION 3.0.0)
set(SRC_LIST mrpcapplication.cpp  rpcheader.pb.cc  rpcprovider.cpp mrpcconfig.cpp mrpcchannel.cpp mrpclog.cpp include/mrpcqueue.hpp)
#  add_library(mrpc SHARED ${SRC_LIST}) # 加上share的话就是动态库
add_library(mrpc  ${SRC_LIST})# 不加上share的话就是静态库

target_link_libraries(mrpc muduo_net muduo_base pthread protobuf)
