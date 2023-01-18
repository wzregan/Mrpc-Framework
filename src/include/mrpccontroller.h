#include <google/protobuf/service.h>
#include <string>
class MrpcController:public google::protobuf::RpcController{
public:
    virtual void Reset();
    virtual bool Failed() const;
    virtual std::string ErrorText() const;
    virtual void SetFailed(const std::string& reason);
private:
    bool m_falid;
    std::string m_faildmsg;
};