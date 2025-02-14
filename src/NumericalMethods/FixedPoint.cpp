#include "../../include/NumericalMethods/FixedPoint.hpp"
#include "../../include/Util.hpp"

namespace lagrange::numerical_methods{
    FixedPoint::FixedPoint(const Function &func, const ConvergenceInfo &info, int targetPhi)
    : NumericalMethod<Real, size_t, Real, Real>(func, info) {
      m_phi = m_func.GetPhi()[targetPhi];
      m_guesses.emplace_back(m_convergenceInfo.initialGuess);
    }


    Real FixedPoint::Next(){
        if(!m_guesses.back().has_value())
            return std::nullopt;

        if(HasConverged())
            return m_guesses.back();
        
        auto x = *m_guesses.back();
        m_guesses.emplace_back(m_phi(x));

        return m_guesses.back();
    }

    tuple<size_t, Real, Real> FixedPoint::CurrentState(){
        return {
            (int)m_guesses.size() - 1,
            m_guesses.back(),
            Error()
        };
    }    
};