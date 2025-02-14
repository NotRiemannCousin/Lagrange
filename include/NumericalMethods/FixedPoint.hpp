#pragma once
#include "../NumericalMethod.hpp"
#include "../Constants.hpp"

using lagrange::constants::Real;

namespace lagrange::numerical_methods{
    class FixedPoint final : public NumericalMethod<Real, size_t, Real, Real>{
        RealFunction m_phi;
        public:
        FixedPoint(const Function &func, const ConvergenceInfo &info, int targetPhi);

        Real Next() override;
        [[nodiscard]] constexpr array<string, 3> InfoHeader() const override {
            return {"n", "ϕ(x)", "error"};
        }
        [[nodiscard]] tuple<size_t, Real, Real> CurrentState() override;
    };
}