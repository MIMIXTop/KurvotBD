#pragma once
#include <optional>
#include <pqxx/pqxx>

namespace lib::Sql {

    template <typename T>
    void appendParam(pqxx::params& ps, const std::optional<T>& opt) {
        if (opt.has_value())
            ps.append(opt.value());
        else
            ps.append();
    }

    template <typename T>
    void appendParam(pqxx::params& ps, const T& value) {
        ps.append(value);
    }

}
