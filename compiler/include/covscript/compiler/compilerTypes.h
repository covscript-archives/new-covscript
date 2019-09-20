//
// Created by kiva on 2019/9/19.
//
#pragma once

#include <covscript/shared/shared_types.hpp>

namespace cs {
    namespace compiler {
        using VMInt = cs::shared_types::vm_int;
        using VMFloat = cs::shared_types::vm_float;
        using VMBool = cs::shared_types::vm_bool;
        using VMChar = cs::shared_types::vm_char;
        using VMString = cs::shared_types::vm_string;

        template<typename T>
        using Ref = std::shared_ptr<T>;

        // Convenience functions to avoid lengthy dynamic_cast() != nullptr checks in many places.
        template<typename T1, typename T2>
        inline bool is(T2 *obj) { // For pointer types.
            return dynamic_cast<typename std::add_const<T1>::type>(obj) != nullptr;
        }

        template<typename T1, typename T2>
        inline bool is(Ref<T2> const &obj) { // For shared pointers.
            return dynamic_cast<T1 *>(obj.get()) != nullptr;
        }
    }

}

