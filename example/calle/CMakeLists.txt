add_executable(calle_test user.pb.cc userservice.cpp)
include_directories(${PROJECT_SOURCE_DIR}/example/callee)

target_link_libraries(calle_test mrpc protobuf)