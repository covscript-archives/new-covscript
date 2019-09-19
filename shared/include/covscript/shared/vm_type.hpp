#pragma once

#include <string>
#include <cstdint>

namespace cs {
    namespace builtin_types {
        using vm_int = long;
        using vm_float = double;
        using vm_bool = bool;
        using vm_char = signed char;
        using vm_string = std::string;
    }
}