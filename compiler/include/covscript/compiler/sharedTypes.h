//
// Created by kiva on 2019/9/20.
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
        using Ptr = std::shared_ptr<T>;

        template<typename T, typename ...Args>
        inline _LIBCPP_INLINE_VISIBILITY
        typename std::enable_if<!std::is_array<T>::value, std::shared_ptr<T>>::type
        makePtr(Args &&...__args) {
            return std::shared_ptr<T>::make_shared(std::forward<Args>(__args)...);
        }

        // Convenience functions to avoid lengthy dynamic_cast() != nullptr checks in many places.
        template<typename T1, typename T2>
        inline bool is(T2 *obj) { // For pointer types.
            return dynamic_cast<typename std::add_const<T1>::type>(obj) != nullptr;
        }

        template<typename T1, typename T2>
        inline bool is(Ptr<T2> const &obj) { // For shared pointers.
            return dynamic_cast<T1 *>(obj.get()) != nullptr;
        }
    }
}
