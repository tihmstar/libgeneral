//
//  Event.cpp
//  libgeneral
//
//  Created by tihmstar on 15.08.20.
//  Copyright © 2020 tihmstar. All rights reserved.
//

#include "../include/libgeneral/macros.h"
#include "../include/libgeneral/Event.hpp"

using namespace tihmstar;

Event::Event()
: _members(0), _curSendEvent(0), _curWaitEvent(0)
{
    
}

Event::~Event(){
    while (_members) {
        //drop all waiting members
        std::unique_lock<std::mutex> lk(_m);
        _cm.wait(lk, [&]{return !_members;});
    }
}

void Event::wait(){
    std::unique_lock<std::mutex> lk(_m);
    ++_members;

    uint64_t waitingForEvent = _curWaitEvent+1;
    if (waitingForEvent == 0) waitingForEvent++;

    _cv.wait(lk, [&]{return _curSendEvent>=waitingForEvent;});
    
    _curWaitEvent = _curSendEvent;
    
    --_members;
    _cm.notify_all();
}

void Event::notifyAll(){
    std::unique_lock<std::mutex> *lk = new std::unique_lock<std::mutex>(_m);
    bool doUnlockHere = true;
    cleanup([&]{
        if (doUnlockHere) {
            lk->unlock();
            delete lk;
        }
    });
    
    if(++_curSendEvent == 0) ++_curSendEvent;
    _cv.notify_all();
}

uint64_t Event::members() const{
    return _members;
}
