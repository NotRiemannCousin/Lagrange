#include "../../include/NumericalMethods/Bisection.hpp"
#include "../../include/Util.hpp"

namespace lagrange::numerical_methods{
    Bisection::Bisection(const Function &func, const ConvergenceInfo &info)
    : NumericalMethod<pair<Real, Real>, size_t, Real, Real, Real, string, string, string, Real>(func, info) {
        m_guesses.clear();

        m_guesses.push_back({m_convergenceInfo.range.first, m_convergenceInfo.range.second});
    }


    pair<Real, Real> Bisection::Next(){
        if(m_guesses.size() == 0)
            return {std::nullopt, std::nullopt};

        if(HasConverged())
            return m_guesses.back();
        
        auto [_l, _r] = m_guesses[m_guesses.size() - 1];

        if(!_l.has_value() || !_r.has_value())
          return {std::nullopt, std::nullopt};

        auto mid = std::midpoint(*_l, *_r);

        auto l = m_func(*_l);
        auto c = m_func(mid);
        auto r = m_func(*_r);

        if(!l.has_value() || !c.has_value() || !r.has_value())
              return {std::nullopt, std::nullopt};

        if((*l) * (*c) < 0)
            m_guesses.emplace_back(_l, mid);
        else
            m_guesses.emplace_back(mid, _r);


        return m_guesses.back();
    }

    tuple<size_t, Real, Real, Real, string, string, string, Real> Bisection::CurrentState(){
        auto [l, r] = m_guesses.back();
        auto mid = l.has_value() && r.has_value() ?
                  Real{std::midpoint(*l, *r)} :
                  std::nullopt;

        static auto parseSgn = [this](Real n) -> string{
          if(!n.has_value())
            return  " ";
          n = this->m_func(*n);
          if(!n.has_value())
            return  " ";
          return *n > 0 ? "+" : "-";
        };

        return {
            (int)m_guesses.size() - 1,
            l,
            mid,
            r,
            parseSgn(l),
            parseSgn(mid),
            parseSgn(r),
            Error()
        };
    }    
};