#include "../include/Function.hpp"
#include "../include/Util.hpp"
#include <ranges>
#include <vector>
#include <cmath>

using std::vector;
using std::string;
using std::print;
using std::pow;
using std::abs;



namespace lagrange{
    Function::Function(RealFunction func) : Function(std::move(func), true) {}


    template<typename T>
    requires std::same_as<T, bool>
    Function::Function(RealFunction func, T generateDerivative) : m_func{std::move(func)},
    // Function::Function(RealFunction func) : m_func{std::move(func)}, m_derivativeType{DerivativeType::GENERATED}
        m_derivativeType{generateDerivative ? DerivativeType::GENERATED : DerivativeType::NONE}
    {
        // if(generateDerivative)
            m_derivative = [this](double x){ return NumericalDerivative(x); };
    }


    template<typename T>
    requires std::same_as<T, bool>
    Function::Function(RealFunction func, RealFunction derivative, T ignoreMismatch)
        : m_func{std::move(func)}, m_derivative{std::move(derivative)}, m_derivativeType{DerivativeType::USER_PROVIDED}
    {
        if (ignoreMismatch) return;

        static const auto points = util::make_array<256>(util::random::generate_bounded_random, -1000, 1000);
        bool isMismatching = false;

        for (const auto p : points) {
            const auto d1 = m_derivative(p);
            const auto d2 = NumericalDerivative(p);

            if ((d1.has_value() != d2.has_value()) ||
                (d1 && d2 && abs(d1.value() - d2.value()) > constants::EPS1_3)) 
            {
                isMismatching = true;
                break;
            }
        }

        if (isMismatching) {
            m_derivativeType = DerivativeType::USER_PROVIDED_INSECURE;
            util::Log("Potentially mismatching derivative", util::WarningLevel::LOW);
        }
    }



    Function::Function(RealFunction func, RealFunction derivative, vector<RealFunction> const &phi, bool ignoreMismatch)
        : Function(move(func), move(derivative), ignoreMismatch) {
            m_phi = phi;
        }



    Real Function::operator()(double x) const{
        return m_func(x);
    }



    Real Function::Derivative(double x) const{
        if(m_derivativeType == DerivativeType::NONE)
            return std::nullopt;
        return m_derivative(x);
    }



    // Real Function::NumericalDerivative(double x, double h = EPS1_3){
    Real Function::NumericalDerivative(double x, double h) const{
        if(m_derivativeType == DerivativeType::NONE)
            return std::nullopt;

        // check if is continuous
        auto xl = m_func(x - constants::EPS);
        auto xc = m_func(x);
        auto xr = m_func(x + constants::EPS);

        if(!xl.has_value() || !xc.has_value() || !xr.has_value() ||
            std::fabs(*xl - *xc) > constants::EPS ||
            std::fabs(*xc - *xr) > constants::EPS)
            return std::nullopt;
        
        auto a = m_func(x - h);
        auto b = m_func(x + h);

        if(!a.has_value() || !b.has_value())
            return std::nullopt;

        return (b.value() - a.value()) / (2 * h);
    }



    Function::DerivativeType Function::GetDerivativeType() const {
        return m_derivativeType;
    }

    span<const RealFunction> Function::GetPhi() const {
        return m_phi;
    }
}