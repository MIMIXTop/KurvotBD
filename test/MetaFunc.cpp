

#include <boost/pfr/tuple_size.hpp>
#include <boost/pfr/detail/make_integer_sequence.hpp>
#include <gtest/gtest.h>

#include "Models.hpp"
#include "util/util.hpp"
TEST(MetaFunction, TestCountFieldOfStruct) {
    EXPECT_EQ(boost::pfr::tuple_size_v<lib::Models::Department>, 4);
}

TEST(MetaFunction, TestCountFieldOfStructInVariant) {
    lib::Models::Model m = lib::Models::Department{};
    EXPECT_EQ(std::visit(lib::util::FieldCounter{}, m), 4);
}

TEST(MetaFunction, TestFieldByIndexInStruct) {
    auto m = lib::Models::Department{
        .department_id = 4
    };

    auto result = lib::util::getFieldAsQVariant(m, 0);



    EXPECT_EQ(result.toInt(), m.department_id);
}