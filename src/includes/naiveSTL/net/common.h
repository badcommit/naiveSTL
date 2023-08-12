//
// Created by Shawn Zhang on 8/11/23.
//

#ifndef NAIVESTL_COMMON_H
#define NAIVESTL_COMMON_H

#include <functional>

namespace NaiveSTL::Net {
    using timestamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

    auto createEventfd() -> int;

}

#endif //NAIVESTL_COMMON_H
