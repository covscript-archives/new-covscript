#pragma once

#include <covscript/shared/memory.hpp>
#include <covscript/shared/any.hpp>

namespace cs {
    class vm_instance final {
        shared::stack_type<shared::stack_type<shared::any>> m_stack;
    };
}
