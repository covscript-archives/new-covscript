//
// Created by kiva on 2019/9/24.
//

#include <covscript/shared/utility.hpp>
#include <covscript/shared/exception.hpp>

namespace cs {
    // exception.hpp
    namespace event {
        shared::event_type exception_raised;
    }

    namespace shared {
        // utility.hpp
        std::chrono::time_point<std::chrono::high_resolution_clock>
        timer::m_timer(std::chrono::high_resolution_clock::now());
    }
}
