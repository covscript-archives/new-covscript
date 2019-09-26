#pragma once

#include <covscript/shared/memory.hpp>
#include <covscript/shared/any.hpp>

namespace cs {
    enum class instrument_types {
        // Expression
        expr,
        // Direct Jump
        jmp,
        // Conditional Jump
        jcond,
        // Push Stack Frame
        push,
        // Pop Stack Frame
        pop,
        // Call Function
        call,
        // Return Function
        ret,
        // Join Stackfull Coroutine
        joins,
        // Join Stackless Coroutine
        joinl,
        // Trigger Coroutine Dispatch
        yield,
        // Register Exception Handler
        regeh,
        // Throw Exception
        thex,
        // Variable Declaration
        vdecl,
        // Null
        null
    };
    class vm_instrument {

    };
    class vm_instance final {
        class stack_frame_base {

        };
        class stack_frame_range {

        };
        class stack_frame_func {

        };
        runtime::stack_type<runtime::stack_type<runtime::any>> m_stack;
    };
}