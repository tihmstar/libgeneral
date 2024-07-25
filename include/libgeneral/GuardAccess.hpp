//
//  GuardAccess.hpp
//  libgeneral
//
//  Created by erd on 27.10.23.
//  Copyright © 2023 tihmstar. All rights reserved.
//

#ifndef GuardAccess_hpp
#define GuardAccess_hpp

#include <libgeneral/Event.hpp>
#include <libgeneral/macros.h>
#include <atomic>

namespace tihmstar {
    class GuardAccess{
        static constexpr const uint32_t maxMembers = 0x10000;
        std::atomic<uint32_t> _members;
        Event _leaveEvent;

    public:
        GuardAccess();
        ~GuardAccess();

        //readonly access
        void addMember();
        void delMember();
        
        //write/modify access
        void lockMember();
        void unlockMember();

        /*
            Attempts to lock Member if it is not already locked.
            Returns true if lock was acquired
            Returns false if already locked
         */
        bool tryLockMember();

        //block until someone removed members
        void waitForMemberDrain();
    };
};

#define guardWrite(var) guard _cleanupGuard([&]{var.unlockMember();});var.lockMember()
#define guardRead(var)  guard _cleanupGuard([&]{var.delMember();});var.addMember()
#define guardReadConditional(var,doGuard) guard _cleanupGuard([&]{if (doGuard) var.delMember();});do{if (doGuard) var.addMember();} while(0)

#endif /* GuardAccess_hpp */
