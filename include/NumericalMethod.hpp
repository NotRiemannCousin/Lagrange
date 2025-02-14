#pragma once
#include "Constants.hpp"
#include "Function.hpp"
#include "util.hpp"
#include <variant>
#include <string>
#include <tuple>
#include <array>
#include <span>
#include <vector>
#include <cmath>
#include <ranges>

using std::function;
using std::optional;
using std::variant;
using std::string;
using std::array;
using std::tuple;
using std::span;
using std::pair;
using std::vector;

namespace lagrange::numerical_methods {
    struct ConvergenceInfo {
        enum ErrorType {
            ABSOLUTE,
            RELATIVE,
            MIST,
            MAX_ITERATIONS,
            ABSOLUTE_FUNCTION
        };

        const ErrorType type{ErrorType::MIST};
        const pair<double, double> range{-10, 10};
        const double initialGuess{5};
        const double error{1e-6};
        const size_t maxIterations{30};
    };


    template<class GuessType, class ...Types>
    class NumericalMethod {
    protected:
        Function m_func;
        vector<GuessType> m_guesses{};
        ConvergenceInfo m_convergenceInfo{};

        [[nodiscard]] pair<Real, Real> CurrAndLast() const {
            if constexpr (std::is_same_v<GuessType, Real>) {
                if (m_guesses.size() < 2)
                    return {std::nullopt, std::nullopt};

                if (!m_guesses[m_guesses.size() - 1].has_value() ||
                    !m_guesses[m_guesses.size() - 2].has_value())
                    return {std::nullopt, std::nullopt};

                auto curr = m_guesses[m_guesses.size() - 1].value();
                auto last = m_guesses[m_guesses.size() - 2].value();

                return {curr, last};

            }else {
                return m_guesses.size() == 0 ?
                    pair{std::nullopt, std::nullopt} :
                    m_guesses[m_guesses.size() - 1];
            }
        }

    public:
        explicit NumericalMethod(Function func, ConvergenceInfo info)
            : m_func(std::move(func)), m_convergenceInfo(std::move(info)) {
            // Initialize m_guesses with the initial guess

            // Check if the function evaluates properly at the bounds
            auto a = m_func(m_convergenceInfo.range.first);
            auto b = m_func(m_convergenceInfo.range.second);

            if (!a.has_value() || !b.has_value() || a.value() * b.value() > 0) {
                util::Log("No root in range!", util::WarningLevel::HIGH);
            }
        }

        virtual ~NumericalMethod() = default;

        virtual GuessType Next() = 0;
        virtual constexpr array<string, sizeof...(Types)> InfoHeader() const = 0;
        virtual tuple<Types...> CurrentState() = 0;

        [[nodiscard]] pair<double, double> Bounds() const {
            return m_convergenceInfo.range;
        }

        [[nodiscard]] span<const GuessType> Guesses() const {
            return m_guesses;
        }

        [[nodiscard]] Real Error() const {
            auto [curr, last] = CurrAndLast();

            if(!curr.has_value() || !last.has_value())
                return std::nullopt;

            switch (m_convergenceInfo.type) {
                case ConvergenceInfo::ABSOLUTE:
                    return std::abs(*curr - *last);
                case ConvergenceInfo::RELATIVE:
                    return std::abs((*curr - *last) / *curr);
                case ConvergenceInfo::MIST:
                    return std::abs((*curr - *last) / std::max(1.0, *curr));
                case ConvergenceInfo::ABSOLUTE_FUNCTION:
                    return std::abs(m_func(*curr).value_or(0.0));
                default:
                    return std::nullopt;
            }
        }

        [[nodiscard]] bool HasConverged() const {
            return m_guesses.size() > m_convergenceInfo.maxIterations ?
                true : // not really but anyways
                Error().value_or(m_convergenceInfo.error + 1) < m_convergenceInfo.error;
        }


        // void PrintTable() const {
        //     constexpr auto table_length = std::ranges::views::transform(
        //         [](const auto& info) {
        //             return max(4, info.size());
        //     }, InfoHeader());
        //
        //
        //     constexpr auto format = '| ' +
        //
        // }
    };
}