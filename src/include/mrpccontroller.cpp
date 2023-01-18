#include "mrpccontroller.h"


void MrpcController::Reset()  {
    this->m_falid = true;
}

bool MrpcController::Failed() const  {
    return this->m_falid;
}

std::string MrpcController::ErrorText() const  {
    return m_faildmsg;
}

void MrpcController::SetFailed(const std::string& reason) {
    this->m_faildmsg = reason;
    m_falid = true;
}
