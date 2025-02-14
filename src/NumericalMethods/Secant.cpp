#include "../../include/NumericalMethods/Secant.hpp"
#include "../../include/Util.hpp"

namespace lagrange::numerical_methods{
    Secant::Secant(const Function &func, const ConvergenceInfo &info)
    : NumericalMethod<Real, size_t, Real, Real, Real, Real>(func, info) {
        m_guesses.push_back(m_convergenceInfo.range.first);
        m_guesses.push_back(m_convergenceInfo.range.second);
    }


    Real Secant::Next(){
        if(HasConverged())
            return m_guesses.back();
        
        auto x0 = m_guesses[m_guesses.size() - 2];
        auto x1 = m_guesses[m_guesses.size() - 1];

        if(!x1.has_value() || !x0.has_value())
          return std::nullopt;

        auto f0 = m_func(*x0);
        auto f1 = m_func(*x1);

        if(!f1.has_value() || !f0.has_value())
          return std::nullopt;

        m_guesses.push_back(
            ((*x0) * (*f1) - (*x1) * (*f0)) / (*f1 - *f0)
        );

        return m_guesses.back();
    }

    tuple<size_t, Real, Real, Real, Real> Secant::CurrentState(){
        Real a = m_guesses.size() > 2 ? m_guesses.back() : std::nullopt;
        Real b = *std::max(m_guesses.begin() + 1, m_guesses.end() - 2);
        Real c = *std::max(m_guesses.begin() + 0, m_guesses.end() - 3);

        return {
            (int)m_guesses.size(),
            a,
            b,
            c,
            Error()
        };
    }    
};