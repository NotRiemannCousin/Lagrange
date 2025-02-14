#pragma once
#include <functional>
#include <optional>
#include <numeric>
#include <numbers>
#include <format>
#include <string>
#include <cmath>


// Primary template for std::optional<T> with support for both char and wchar_t
template <typename T, typename CharT>
struct std::formatter<std::optional<T>, CharT> {
    std::formatter<T, CharT> value_formatter;
    std::formatter<std::basic_string<CharT>, CharT> nullopt_formatter;

    template<class ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        using char_type = typename ParseContext::char_type;
        auto it = ctx.begin();
        auto end = ctx.end();

        if (it == end || *it != CharT('(')) return it;
        ++it;

        auto value_start = it;
        while (it != end && *it != CharT(')')) ++it;
        if (it == end) return it; // Missing closing ')'

        std::size_t value_len = it - value_start;
        std::basic_string_view<char_type> value_spec(value_start, value_len);
        ParseContext value_ctx(value_spec, 0);
        value_formatter.parse(value_ctx);

        ++it; // Move past ')'

        if (it == end || *it != CharT('(')) return it; // Expect '(' for nullopt
        ++it;

        auto nullopt_start = it;
        while (it != end && *it != CharT(')')) ++it;
        if (it == end) return it; // Missing closing ')'

        std::size_t nullopt_len = it - nullopt_start;
        std::basic_string_view<char_type> nullopt_spec(nullopt_start, nullopt_len);
        ParseContext nullopt_ctx(nullopt_spec, 0);
        nullopt_formatter.parse(nullopt_ctx);

        ++it; // Move past ')'

        return it;
    }

    template<class FormatContext>
    constexpr auto format(const std::optional<T>& opt, FormatContext& ctx) const {
        return opt.has_value() ?
            value_formatter.format(*opt, ctx) :
            nullopt_formatter.format(std::basic_string<CharT>{CharT('-')}, ctx);
    }
};


namespace lagrange::constants{
    using Real = std::optional<double>;

    // R->R function. Returns double if defined, std::nullopt otherwise.
    using RealFunction = std::function<Real(double)>;

    const double INF = std::numeric_limits<double>::infinity();

    const double PI = std::numbers::pi;

    const double EPS = std::numeric_limits<double>::epsilon();
    const double EPS1_3 = std::pow(EPS, 1/3.0);

}