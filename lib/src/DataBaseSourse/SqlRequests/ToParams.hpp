#pragma once
#include <string>
#include <vector>
#include "Converters.hpp"

namespace lib::Converters {
    inline int toParams(int v)  { return v; }
    inline double toParams(double v)  { return v; }
    inline bool toParams(bool v)  { return v; }
    inline const std::string& toParams(const std::string& v)  { return v; }

    template<typename E> requires std::is_enum_v<E>
    std::string toParams(E e) {
        return toString(e);
    }

    inline std::string toParams(const std::vector<std::string>& v) {
        return toPgArray(v);
    }

}




