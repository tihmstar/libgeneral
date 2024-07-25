//
//  GuardAccess.cpp
//  libgeneral
//
//  Created by erd on 27.10.23.
//  Copyright © 2023 tihmstar. All rights reserved.
//

#include "../include/libgeneral/GuardAccess.hpp"

using namespace tihmstar;

#pragma mark GuardAccess
GuardAccess::GuardAccess()
: _members(0)
{
    //
}

GuardAccess::~GuardAccess(){
    uint64_t wevent = _leaveEvent.getNextEvent();
    while (_members) {
        _leaveEvent.waitForEvent(wevent);
        wevent = _leaveEvent.getNextEvent();
    }
}

#pragma mark public
void GuardAccess::addMember(){
    while (true){
        if (_members.fetch_add(1) >= GuardAccess::maxMembers-1){
            _members.fetch_sub(1); _leaveEvent.notifyAll();
            
            uint64_t wevent = _leaveEvent.getNextEvent();
            while (_members>=GuardAccess::maxMembers-1){
                _leaveEvent.waitForEvent(wevent);
                wevent = _leaveEvent.getNextEvent();
            }
        }else{
            break;
        }
    }
}

void GuardAccess::delMember(){
    _members.fetch_sub(1);
    _leaveEvent.notifyAll();
}

void GuardAccess::lockMember(){
    while (true){
        if (_members.fetch_add(GuardAccess::maxMembers) >= GuardAccess::maxMembers){
            _members.fetch_sub(GuardAccess::maxMembers); _leaveEvent.notifyAll();
            
            uint64_t wevent = _leaveEvent.getNextEvent();
            while (_members>=GuardAccess::maxMembers){
                _leaveEvent.waitForEvent(wevent);
                wevent = _leaveEvent.getNextEvent();
            }
        }else{
            uint64_t wevent = _leaveEvent.getNextEvent();
            while (_members > GuardAccess::maxMembers){
                _leaveEvent.waitForEvent(wevent);
                wevent = _leaveEvent.getNextEvent();
            }
            break;
        }
    }
}

void GuardAccess::unlockMember(){
    _members.fetch_sub(GuardAccess::maxMembers);
    _leaveEvent.notifyAll();
}

bool GuardAccess::tryLockMember(){
    while (true){
        if (_members.fetch_add(GuardAccess::maxMembers) >= GuardAccess::maxMembers){
            _members.fetch_sub(GuardAccess::maxMembers); _leaveEvent.notifyAll();
            return false;
        }else{
            uint64_t wevent = _leaveEvent.getNextEvent();
            while (_members > GuardAccess::maxMembers){
                _leaveEvent.waitForEvent(wevent);
                wevent = _leaveEvent.getNextEvent();
            }
            return true;
        }
    }
}

void GuardAccess::waitForMemberDrain(){
    uint64_t wevent = _leaveEvent.getNextEvent();
    _leaveEvent.waitForEvent(wevent);
}
