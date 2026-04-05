#pragma once
#include <string>
#include <vector>
#include "Converters.hpp"
#include "util/util.hpp"

namespace lib::Converters {

    template<typename T>
    std::enable_if_t<util::is_one_of<T>::value, T&&>
    toParams(T&& v) {
        return std::forward<T>(v);
    }

    inline const std::string& toParams(const std::string& v)  { return v; }

    template<typename E> requires std::is_enum_v<E>
    std::string toParams(E e) {
        return toString(e);
    }

    inline std::string toParams(const std::vector<std::string>& v) {
        return toPgArray(v);
    }

}