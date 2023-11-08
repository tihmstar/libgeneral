//
//  DeliveryEvent.hpp
//  libgeneral
//
//  Created by tihmstar on 02.11.20.
//

#ifndef DeliveryEvent_hpp
#define DeliveryEvent_hpp

#include <libgeneral/macros.h>
#include <libgeneral/Event.hpp>
#include <queue>

namespace tihmstar {
    template <typename T>
    class DeliveryEvent{
        bool _isDying;
        std::atomic<uint64_t> _members;
        Event _membersUpdateEvent;
        
        Event _dataWait;
        Event _dataIsFree;
        std::mutex _dataLock;
        std::queue<T> _dataQueue;

    public:
        DeliveryEvent();
        ~DeliveryEvent();
        
        T wait();
        void post(T data);
        
        /*
            Releases waiter and hints destruction of the object in near future
            Further calls to wait() or post() are not allowed after calling kill()
         */
        void kill();
    };

#pragma mark implementation

    template <class T>
    DeliveryEvent<T>::DeliveryEvent()
    : _isDying(false),_members(0)
    {
        //
    }

    template <class T>
    DeliveryEvent<T>::~DeliveryEvent(){
        kill();
    }

    template <class T>
    T DeliveryEvent<T>::wait(){
        ++_members;
        cleanup([&]{
            --_members;
            _membersUpdateEvent.notifyAll();
        });

        std::unique_lock<std::mutex> ul(_dataLock);
        while (!_dataQueue.size()) {
            retassure(!_isDying, "object died while waiting on it");
            uint64_t wevnet = _dataWait.getNextEvent();
            ul.unlock();
            _dataWait.waitForEvent(wevnet);

            ul.lock();
        }
        T mydata = _dataQueue.front(); _dataQueue.pop();
        _dataIsFree.notifyAll();
        return mydata;
    }

    template <class T>
    void DeliveryEvent<T>::post(T data){
        ++_members;
        cleanup([&]{
            --_members;
            _membersUpdateEvent.notifyAll();
        });
        
        _dataLock.lock();
        _dataQueue.push(data);
        _dataWait.notifyAll();
        _dataLock.unlock();
    }

    template <class T>
    void DeliveryEvent<T>::kill(){
        std::unique_lock<std::mutex> ul(_dataLock);
        _isDying = true;
        while ((uint64_t)_members > 0) {
            _dataWait.notifyAll(); //release waiter
            uint64_t wevent = _membersUpdateEvent.getNextEvent();
            ul.unlock();
            _membersUpdateEvent.waitForEvent(wevent);
            ul.lock();
        }
    }
};



#endif /* DeliveryEvent_hpp */
