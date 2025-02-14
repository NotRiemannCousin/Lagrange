#include "../../include/NumericalMethods/Newton.hpp"
#include "../../include/Util.hpp"

namespace lagrange::numerical_methods{
    Newton::Newton(const Function &func, const ConvergenceInfo &info)
    : NumericalMethod<Real, size_t, Real, Real, Real, Real>(func, info) {
        m_guesses.emplace_back(m_convergenceInfo.initialGuess);
    }


    Real Newton::Next(){
        if(m_func.GetDerivativeType() == Function::DerivativeType::NONE)
            return std::nullopt;
        if(!m_guesses.back().has_value())
            return std::nullopt;

        if(HasConverged())
            return m_guesses.back();
        
        auto x = *m_guesses.back();

        auto f = m_func(x);
        auto df = m_func.Derivative(x);

        if(!f.has_value() || df.value_or(0) == 0)
            return std::nullopt;

        m_guesses.emplace_back(x - *m_func(x) / *m_func.Derivative(x));
        return m_guesses.back();
    }

    tuple<size_t, Real, Real, Real, Real> Newton::CurrentState(){
        auto x   = m_guesses.back();
        auto fx  = x.has_value() ? m_func(*x) : std::nullopt;
        auto dfx = x.has_value() ? m_func.Derivative(*x) : std::nullopt;

        return {
            (int)m_guesses.size() - 1,
            x,
            fx,
            dfx,
            Error()
        };
    }    
};