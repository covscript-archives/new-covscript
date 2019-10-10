//
// Created by kiva on 2019/9/20.
//
#pragma once

#include <covscript/shared/shared_types.hpp>
#include <covscript/shared/exception.hpp>

namespace cs {
    namespace compiler {
        using VMInt = cs::shared::vm_int;
        using VMFloat = cs::shared::vm_float;
        using VMBool = cs::shared::vm_bool;
        using VMChar = cs::shared::vm_char;
        using VMString = cs::shared::vm_string;

        using RuntimeException = cs::runtime_error;

        template<typename T, typename... ArgsT>
        void compilerThrow(ArgsT &&... args) {
            cs::throw_ex<T>(std::forward<ArgsT>(args)...);
        }

        template<typename T>
        using Ptr = std::shared_ptr<T>;

        template<typename T, typename ...Args>
        typename std::enable_if<!std::is_array<T>::value, std::shared_ptr<T>>::type
        makePtr(Args &&...__args) {
            return std::shared_ptr<T>::make_shared(std::forward<Args>(__args)...);
        }
    }
}
