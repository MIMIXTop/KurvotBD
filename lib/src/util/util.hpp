#pragma once

namespace lib::util {
    template <typename... Lambdas>
    struct match : Lambdas... {
        using Lambdas::operator()...;
    };

    template <typename... Lambdas>
    match(Lambdas... lambdas) -> match<Lambdas...>;

    template<typename T>
    struct is_one_of : std::disjunction<
        std::is_same<std::decay_t<T>,int>,
        std::is_same<std::decay_t<T>,std::string>,
        std::is_same<std::decay_t<T>,bool>,
        std::is_same<std::decay_t<T>,double>
        > {};
}

