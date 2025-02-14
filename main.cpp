#include "include/NumericalMethods/Newton.hpp"
#include "include/NumericalMethods/Bisection.hpp"
#include "include/NumericalMethods/FixedPoint.hpp"
#include "include/NumericalMethods/Secant.hpp"
#include <bits/stdc++.h>
#include <ranges>

using lagrange::Function;
using namespace lagrange::numerical_methods;

// using namespace rg = std::ranges;

template<lagrange::util::TablePrintable T>
void Question1(T numericalMethod, const string formatHeader, const string formatTable) {
    auto table = lagrange::util::GenerateTable(numericalMethod, formatHeader, formatTable);

    std::print("{}", std::views::join_with(table, '\n') | std::ranges::to<std::string>());
}



int main(){
    Function func1(
        [](double x) {
            return x - cos(x);
        },
        [](double x) {
            return 1 + sin(x);
        },
        {
            [](double x) {
                return std::cos(x);
            },
        }
    );

    ConvergenceInfo info{ConvergenceInfo::MIST, {0.5, 1}, 0, 1e-11, 106};

    Newton newton(func1, info);
    Bisection bisection(func1, info);
    FixedPoint fixedPoint(func1, info, 0);
    Secant secant(func1, info);

    std::println("\n\nNewton");
    Question1(newton,
        "| {:^3} | {:^15} | {:^15} | {:^15} | {:^15} |",
        "| {:^3} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} |");
    std::println("\n\nBisection");
    Question1(bisection,
        "| {:^3} | {:^15} | {:^15} | {:^15} | {:^9} | {:^9} | {:^9} | {:^15} |",
        "| {:^3} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} | {:^9} | {:^9} | {:^9} | {:(^15.11f)(^15)} |");
    std::println("\n\nFixedPoint");
    Question1(fixedPoint,
        "| {:^3} | {:^15} | {:^15} |",
        "| {:^3} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} |");
    std::println("\n\nSecant");
    Question1(secant,
        "| {:^3} | {:^15} | {:^15} | {:^15} | {:^15} |",
        "| {:^3} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} | {:(^15.11f)(^15)} |");


    return 0;
}