#pragma once
#include <mutex>
#include <condition_variable> 
#include <pthread.h>
#include <iostream>
template<typename T>
class MrpcLogQueue{
public:
    MrpcLogQueue():m_mutex(m_mutexkernel,std::defer_lock){
        
    }
    void push(const T data){
        m_mutex.lock();
        this->m_queue.push(data);
        m_mutex.unlock();
        m_cond.notify_all();
    }
    T pop(){

        m_mutex.lock();
        if(m_queue.size()==0){
            m_mutex.unlock();
            std::unique_lock<std::mutex> lk(m_cv);
            m_cond.wait(lk);
            m_mutex.lock();
        }
        T res = m_queue.front();
        m_queue.pop(); 
        m_mutex.unlock();
        return res;
    }
private:
    std::queue<T> m_queue;
    std::unique_lock<std::mutex> m_mutex;
    std::mutex m_mutexkernel;
    std::mutex m_cv;
    std::condition_variable m_cond;
};