#ifndef SAT_WRAPPER_HPP
#define SAT_WRAPPER_HPP

#include <vector>
#include <memory>

#include <core/Solver.h>

class SatWrapper {
  public:
    using Literal = int;
    using Clause = std::vector<Literal>;
    using ClauseConst = const Clause;
    using SolverT = Glucose::Solver;

    SatWrapper ();

    SatWrapper (const SolverT& s) : m_sat(s) {
    }

    ~SatWrapper () {}

    void not_clause (ClauseConst& c);
    void and_clause (ClauseConst& c);
    void or_clause (ClauseConst& c);
    void nand_clause (ClauseConst& c);
    void nor_clause (ClauseConst& c);
    void xor_clause (ClauseConst& c);
    void xnor_clause (ClauseConst& c);

    void cardinality_clause (ClauseConst& c,
      const size_t lower,
      const size_t upper
      );

    void simplify();
    void solve();

    void get_model() const {

    }

    void get_lit (const size_t idx) {

    }

    const SolverT& get_solver() {
      return m_sat;
    }

  private:
    SolverT m_sat;
};
#endif // SAT_WRAPPER_HPP
