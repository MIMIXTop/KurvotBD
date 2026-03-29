#pragma once
#include <string_view>

namespace lib::Constants {
    constexpr std::string_view getListTables = "SELECT table_name "
            "FROM information_schema.tables "
            "WHERE table_schema = 'public' "
            "AND table_type = 'BASE TABLE' "
            "ORDER BY table_name";
}

