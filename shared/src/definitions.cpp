//
// Created by kiva on 2019/9/24.
//

#include <covscript/shared/utility.hpp>

namespace cs {
    namespace shared {
        // exception.hpp
        namespace event {
            event_type exception_raised;
        }

        // utility.hpp
        std::chrono::time_point<std::chrono::high_resolution_clock>
        timer::m_timer(std::chrono::high_resolution_clock::now());
    }
}
