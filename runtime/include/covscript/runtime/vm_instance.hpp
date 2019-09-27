#pragma once

#include <covscript/shared/memory.hpp>
#include <covscript/shared/any.hpp>
#include <cstdint>

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
        enum class frame_type : std::uint8_t {
            null, slot, scope, fcall, regeh, callback
        };
        struct frame_header {
            frame_type type;
            std::size_t size;
        };
        using byte_t = std::uint8_t;
        byte_t *stack_start = nullptr, *stack_limit = nullptr, *stack_pointer = nullptr;
        inline void push_header(frame_header header)
        {
            ::new (--stack_pointer) frame_header(header);
        }
    public:
        vm_instance() = delete;
        vm_instance(std::size_t stack_size)
        {
            if(!(stack_start = reinterpret_cast<byte_t*>(::malloc(stack_size))))
                throw_ex<cs::runtime_error>("VM Instance Allocation Failed.");
            stack_limit = stack_start + stack_size;
            stack_pointer = stack_limit;
            ::new (stack_pointer - 1) frame_header(frame_header::null);
        }
        void pop_stack()
        {
            switch(static_cast<frame_header>(*(stack_pointer - 1)))
            {
                case frame_header::null:
                    COVSDK_RTERR("Pop empty stack.");
            }
        }
    };
}
