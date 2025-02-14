#pragma once
#include "Constants.hpp"
#include <type_traits>
#include <functional>
#include <expected>
#include <optional>
#include <vector>

using std::function;
using std::optional;
using std::vector;
using std::span;

namespace lagrange{
    using lagrange::constants::Real;
    using lagrange::constants::RealFunction;


    class Function{
        public:
        enum DerivativeType{
            NONE = 0,
            INSECURE = 1,
            GENERATED = 2,
            USER_PROVIDED = 4,
            USER_PROVIDED_INSECURE = USER_PROVIDED | INSECURE
        };

        private:

        RealFunction m_func;
        RealFunction m_derivative;
        vector<RealFunction> m_phi;
        DerivativeType m_derivativeType;

        public:

        explicit Function(RealFunction func);
        template<typename T>
        requires std::same_as<T, bool>
        explicit Function(RealFunction func, T generateDerivative);
        template<typename T>
        requires std::same_as<T, bool>
        explicit Function(RealFunction func, RealFunction derivative, T ignoreMismatch = false);
        explicit Function(RealFunction func, RealFunction derivative, vector<RealFunction> const &phi, bool ignoreMismatch = false);

        [[nodiscard]] Real operator()(double x) const;

        [[nodiscard]] Real Derivative(double x) const;

        [[nodiscard]] Real NumericalDerivative(double x, double h = constants::EPS1_3) const;

        [[nodiscard]] DerivativeType GetDerivativeType() const;

        [[nodiscard]] span<const RealFunction> GetPhi() const;
    };
}