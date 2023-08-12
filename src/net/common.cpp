//
// Created by Shawn Zhang on 8/11/23.
//

#include "naiveSTL/net/common.h"


namespace NaiveSTL::Net{


#ifdef __linux__
    #include <sys/eventfd.h>
    auto createEventfd() -> int
    {
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (evtfd < 0)
        {
            LOG_SYSERR << "Failed in eventfd";
            abort();
        }
        return evtfd;
    }

#else

    auto createEventfd() -> int {
        abort();
//        return 0;
    }

#endif

}