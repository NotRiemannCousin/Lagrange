#pragma once
#include "Constants.hpp"
#include <filesystem>
#include <string>


namespace fs = std::filesystem;
using std::string;
using std::tuple;
using std::array;


namespace lagrange::util{

    enum WarningLevel {
        LOW = 0,
        MEDIUM = 1,
        HIGH = 2
    };

    void Log(string msg, WarningLevel level = WarningLevel::LOW, fs::path const &filename = "");

    template<size_t N, class Fun, class... Args>
    constexpr std::array<std::invoke_result_t<Fun, Args...>, N> make_array(Fun f, Args&&... args)
    {
        std::array<std::invoke_result_t<Fun, Args...>, N> arr;

        std::generate(arr.begin(), arr.end(), std::bind(f, std::forward<Args>(args)...));

        return arr;
    }


    template<class T>
    concept TablePrintable = requires(T t) {
        { t.CurrentState() };
        // ensure that t.CurrentState() is an tuple too
        { t.InfoHeader() } -> std::same_as<std::array<std::string, std::tuple_size_v<decltype(t.CurrentState())>>>;
        { t.HasConverged() } -> std::same_as<bool>;
        { t.Next() };
    };

    template<TablePrintable T>
    std::vector<string> GenerateTable(T numericalMethod, const string headerFormat, const string dataFormat) {
        std::vector<string> table;

        // Process header
        auto header = numericalMethod.InfoHeader();
        const string headerLine = std::apply([&](const auto&... args) {
            return std::vformat(headerFormat, std::make_format_args(args...));
        }, header);
        table.push_back(headerLine);

        // Process each state until convergence
        while (!numericalMethod.HasConverged()) {
            auto currentState = numericalMethod.CurrentState();
            string row = std::apply([&](const auto&... args) {
                return std::vformat(dataFormat, std::make_format_args(args...));
            }, currentState);
            table.push_back(row);
            numericalMethod.Next();
        }

        // Process the final state after convergence
        auto finalState = numericalMethod.CurrentState();
        string finalRow = std::apply([&](const auto&... args) {
            return std::vformat(dataFormat, std::make_format_args(args...));
        }, finalState);
        table.push_back(finalRow);

        return table;
    }


    namespace random{
        constexpr unsigned long long lcg(unsigned long long seed) { // Linear Congruential Generator
            return (6364136223846793005 * seed + 1) >> 30;
        }

        constexpr int generate_bounded_random(int min, int max) {
            static unsigned long long seed = 0;
            unsigned long long rand_val = lcg(seed++);
            return min + (rand_val % (max - min + 1)); // NOLINT(*-narrowing-conversions)
        }
    }
}
