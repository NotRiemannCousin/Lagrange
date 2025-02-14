#pragma once
#include "../NumericalMethod.hpp"
#include "../Constants.hpp"

using lagrange::constants::Real;

namespace lagrange::numerical_methods{
    class Bisection final : public NumericalMethod<pair<Real, Real>, size_t, Real, Real, Real, string, string, string, Real>{
        public:
        Bisection(const Function &func, const ConvergenceInfo &info);

        pair<Real, Real> Next() override;
        [[nodiscard]] constexpr array<string, 8> InfoHeader() const override {
            return {"n", "l", "x", "r", "sgn(f(l))", "sgn(f(x))", "sgn(f(r))", "error"};
        }
        [[nodiscard]] tuple<size_t, Real, Real, Real, string, string, string, Real> CurrentState() override;
    };
}

