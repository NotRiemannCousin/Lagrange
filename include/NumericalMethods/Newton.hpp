#pragma once
#include "../NumericalMethod.hpp"
#include "../Constants.hpp"

using lagrange::constants::Real;

namespace lagrange::numerical_methods{
    class Newton final : public NumericalMethod<Real, size_t, Real, Real, Real, Real>{
        public:
        Newton(const Function &func, const ConvergenceInfo &info);

        Real Next() override;
        [[nodiscard]] constexpr array<string, 5> InfoHeader() const override {
            return {"n", "x", "f(x)", "f'(x)", "error"};
        }
        [[nodiscard]] tuple<size_t, Real, Real, Real, Real> CurrentState() override;
    };
}