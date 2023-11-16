//
//  Manager.cpp
//  libgeneral
//
//  Created by tihmstar on 17.11.20.
//  Copyright © 2020 tihmstar. All rights reserved.
//

#include "../include/libgeneral/macros.h"
#include "../include/libgeneral/exception.hpp"
#include "../include/libgeneral/Manager.hpp"
#include "../include/libgeneral/Event.hpp"
#include <unistd.h>


using namespace tihmstar;

Manager::Manager()
: _loopThread(nullptr),_loopState(LOOP_UNINITIALISED)
{
    //empty
}


Manager::~Manager(){
    debug("[Manager] destroying Manager(%p)",this);

    stopLoop();
#ifdef DEBUG
    assert(_loopState == LOOP_UNINITIALISED || _loopState == LOOP_STOPPED);
    assert(!_loopThread);
#endif
}

bool Manager::loopEvent(){
    reterror("[Manager] LoopEvent wasn't overwritten. Probably subclass construction failed!");
}

void Manager::startLoop(){
    retassure(_loopState == LOOP_UNINITIALISED, "[Manager] loop already initialized");

    loop_state expected = LOOP_UNINITIALISED;
    loop_state tobeplaced = LOOP_CONSTRUCTING;
    //if this fails, another thread is already creating a loop
    assure(_loopState.compare_exchange_strong(expected, tobeplaced));
    assure(!_loopThread);

    Event threadInitEvent;
    uint64_t wevent = threadInitEvent.getNextEvent();
    _loopThread = new std::thread([&]{
        _loopState = LOOP_RUNNING;
        threadInitEvent.notifyAll();
        try {
            beforeLoop();
        } catch (tihmstar::exception &e) {
            debug("[Manager] failed to execute beforeLoop action of exception error=%s code=%d",e.what(),e.code());
            _loopState = LOOP_STOPPING;
        }
        while (_loopState == LOOP_RUNNING) {
            try {
                if (!loopEvent()) break;
            } catch (tihmstar::exception &e) {
                debug("[Manager] breaking Manager-Loop because of exception error=%s code=%d",e.what(),e.code());
                break;
            }
        }
        afterLoop();
        _loopState = LOOP_STOPPED;
    });

    //hangs here iff _loopThread didn't spawn yet
    threadInitEvent.waitForEvent(wevent);
}

void Manager::stopLoop() noexcept{
    if (_loopState < LOOP_STOPPED) {
        if (_loopState == LOOP_UNINITIALISED) {
            try{
                startLoop();
            }catch (...){
                //we can't start the loop twice, but now we are sure it's been started at least once
            }
        }
        
        while (_loopState < LOOP_RUNNING) {
            //if we are constructing a thread, wait for it being constructed
            sched_yield();
        }
        
        while (!_loopThread) {
            //wait for _loopThread variable to be set
            sched_yield();
        }
        
        loop_state expected = LOOP_RUNNING;
        loop_state tobeplaced = LOOP_STOPPING;
        _loopState.compare_exchange_strong(expected, tobeplaced);
        /*
         _loopState is now either LOOP_STOPPING or LOOP_STOPPED
         */
        
        stopAction();
    }
    if (_loopThread) {
        try {
            _loopThread->join();
        } catch (...) {
            //pass
        }
        delete _loopThread; _loopThread = nullptr;
    }
}

void Manager::beforeLoop(){
    //do nothing by default
}

void Manager::afterLoop() noexcept{
    //do nothing by default
}

void Manager::stopAction() noexcept{
    //do nothing by default
}
