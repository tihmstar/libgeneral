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
    while (_members) {
        _enterEvent.wait();
    }
}

#pragma mark public
void GuardAccess::addMember(){
    while (true){
        if (_members.fetch_add(1) >= GuardAccess::maxMembers){
            _members.fetch_sub(1);
            while (_members>=GuardAccess::maxMembers)
                _enterEvent.wait();
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
            while (_members>=GuardAccess::maxMembers)
                _enterEvent.wait();
        }else{
            while (_members > GuardAccess::maxMembers)
                _leaveEvent.wait(); //wait until all members are gone
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
    _notifyEvent.wait();
}
