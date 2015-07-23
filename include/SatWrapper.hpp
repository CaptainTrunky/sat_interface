#ifndef SAT_WRAPPER_HPP
#define SAT_WRAPPER_HPP

#include "Journal.hpp"

#include <vector>
#include <memory>

#include <core/Solver.h>

class SatWrapper {
  public:
    using Var = Glucose::Var;
    using Literal = Glucose::Lit;
    using Clause = std::vector<Literal>;
    using ClauseConst = const Clause;
    using Solver = Glucose::Solver;

    SatWrapper () {
      m_sat = new Solver();
    }

    SatWrapper (Solver* s) {
      m_sat = s;
    }

    ~SatWrapper () {
      delete m_sat;
    }

    Var not_clause (ClauseConst& c);
    Var and_clause (ClauseConst& c);
    Var or_clause (ClauseConst& c);
    Var nand_clause (ClauseConst& c);
    Var nor_clause (ClauseConst& c);
    Var xor_clause (ClauseConst& c);
    Var xnor_clause (ClauseConst& c);

    Var cardinality_clause (ClauseConst& c,
      const size_t lower,
      const size_t upper
    );

    Var get_new_var () {
      return get_solver()->newVar();
    }

    Literal negate (const Literal& l) const {
      return ~l;
    }

    Literal get_literal (Var v, bool sign = false) const {
      return Glucose::mkLit(v, sign);
    }

    Var get_var (const Literal& l) {
      return Glucose::var(l);
    }

    void add_literal(const Literal& l) {
      get_solver()->addClause(l);
    }

    void add_clause (const Clause& c) {
      Vec v(c.size());
      _get_vec_from_clause(c, v);
      get_solver()->addClause(v);
    }

    void simplify () {
      m_sat->simplify();
    }

    bool solve () {
      return m_sat->solve();
    }

    bool solve (const Literal& asmp) {
      return m_sat->solve(asmp);
    }

    void get_model () const {

    }

    void get_var_value_from_model (const size_t idx) {

    }

    Solver* get_solver () {
      return m_sat;
    }

    const Solver* get_solver () const {
      return m_sat;
    }

    void write_dimacs (const std::string& filename) {
      get_solver()->toDimacs(filename.c_str());
    }

  private:
    using Vec = Glucose::vec<Literal>;

    Solver* m_sat;

    void _get_vec_from_clause (const Clause& c, Vec& v) const {
      ASSERT(v.size() == c.size(), "Must be equal");

      for (size_t idx = 0; idx < c.size(); ++idx) {
        v[idx] = c[idx];
      }
    }
};
#endif // SAT_WRAPPER_HPP
