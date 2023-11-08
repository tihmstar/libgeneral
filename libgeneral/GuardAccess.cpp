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
    uint64_t wevent = _enterEvent.getNextEvent();
    while (_members) {
        _enterEvent.waitForEvent(wevent);
        wevent = _enterEvent.getNextEvent();
    }
}

#pragma mark public
void GuardAccess::addMember(){
    while (true){
        if (_members.fetch_add(1) >= GuardAccess::maxMembers){
            _members.fetch_sub(1);
            uint64_t wevent = _enterEvent.getNextEvent();
            while (_members>=GuardAccess::maxMembers){
                _enterEvent.waitForEvent(wevent);
                wevent = _enterEvent.getNextEvent();
            }
        }else{
            break;
        }
    }
}

void GuardAccess::delMember(){
    _members.fetch_sub(1);
    _enterEvent.notifyAll();
    _leaveEvent.notifyAll();
    _notifyEvent.notifyAll();
}

void GuardAccess::lockMember(){
    while (true){
        if (_members.fetch_add(GuardAccess::maxMembers) >= GuardAccess::maxMembers){
            _members.fetch_sub(GuardAccess::maxMembers);
            uint64_t wevent = _enterEvent.getNextEvent();
            while (_members>=GuardAccess::maxMembers){
                _enterEvent.waitForEvent(wevent);
                wevent = _enterEvent.getNextEvent();
            }
        }else{
            uint64_t wevent = _leaveEvent.getNextEvent();
            while (_members > GuardAccess::maxMembers){
                _leaveEvent.waitForEvent(wevent); //wait until all members are gone
                wevent = _leaveEvent.getNextEvent();
            }
            break;
        }
    }
}

void GuardAccess::unlockMember(){
    _members.fetch_sub(GuardAccess::maxMembers);
    _enterEvent.notifyAll();
    _leaveEvent.notifyAll();
    _notifyEvent.notifyAll();
}

void GuardAccess::waitForMemberDrain(){
    uint64_t wevent = _notifyEvent.getNextEvent();
    _notifyEvent.waitForEvent(wevent);
}
