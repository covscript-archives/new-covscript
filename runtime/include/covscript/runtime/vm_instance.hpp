#pragma once
#include <runtime/memory.hpp>

namespace cs {
    class vm_instance final {
        runtime::stack_type<runtime::stack_type<runtime::any>> m_stack;
    };
}