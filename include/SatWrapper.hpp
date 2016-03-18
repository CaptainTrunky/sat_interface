#ifndef SAT_WRAPPER_HPP
#define SAT_WRAPPER_HPP

#include "Journal.hpp"

#include <memory>
#include <fstream>
#include <utility>
#include <vector>

#include <core/Solver.h>

class SatWrapper {
  public:
    using Var = Glucose::Var;

    using Clause = std::vector<Var>;
    using ClauseConst = const Clause;

    using Literal = Glucose::Lit;

    using Solver = Glucose::Solver;

    // Sums, overflow
    using PartialSum = std::pair<SatWrapper::Clause, SatWrapper::Var>;
    // sums, overflows
    using SequentialCounter = std::pair<SatWrapper::Clause, SatWrapper::Clause>;

    // Sum, carry
    using HalfAdder = std::pair<SatWrapper::Var, SatWrapper::Var>;
    using FullAdder = std::pair<SatWrapper::Var, SatWrapper::Var>;

    SatWrapper () {
      m_sat = new Solver();
      // FIXME: skip satidx == 0, hard to use it now
      get_new_var();

      // skip ids for always_true/false constants
      get_new_var ();
      get_new_var ();

      // setup constants
      add_literal (SatWrapper::always_true ());
      add_literal (negate(SatWrapper::always_false ()));
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

    Clause at_most_clause (ClauseConst& c, const size_t upper);

    Var get_new_var () {
      return get_solver()->newVar();
    }

    Var negate (const Var v) const {
      return -v;
    }

    Literal negate (const Literal& l) const {
      return ~l;
    }

    Literal get_literal (Var v, bool sign = false) const {
      const auto& l = Glucose::mkLit (std::abs(v), sign);
      return v > 0 ? l : negate (l);
    }

    Var get_var (const Literal& l) {
      const auto v = Glucose::var(l);

      return Glucose::sign(l) ? -v : v;
    }

    void add_literal (const Var v) {
      const auto& l = get_literal (v);
      add_literal (l);
    }

    void add_literal (const Literal& l) {
      const bool added = get_solver()->addClause(l);
      Clause dbg;
      dbg.push_back (get_var (l));
      _add_debug_clause (dbg);
      ASSERT (added, "Can't add literal");
    }

    void add_clause (const Clause& c) {
      Vec vec;

      for (const auto v: c) {
        const auto& l = get_literal (v);
        vec.push (l);
      }
      const bool added = get_solver()->addClause(vec);
      _add_debug_clause (c);
      ASSERT (added, "Can't add clause");
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

    bool solve (ClauseConst& c) {
      Vec asmp;
      for (const auto v: c) {
        asmp.push (get_literal (v));
      }
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
    using Clauses = std::vector<Clause>;

    Clauses _debug_clauses;

    static constexpr Var always_true () {
      return 1;
    };

    static constexpr Var always_false () {
      return 2;
    };

    void _add_debug_clause (ClauseConst& c) {
      _debug_clauses.push_back(c);
    }

    void _write_debug_clauses () {
      std::ofstream out ("./out.dbg");
      // FIXME: replace 0 vars with actual var num
      out << "p cnf " << 0 << " " << _debug_clauses.size() << std::endl;

      for (const auto& c: _debug_clauses) {
        for (const auto l: c) {
          out << l << " ";
        }
        out << "0" << std::endl;
      }
      out.flush();
    }

    Solver* m_sat;

    SequentialCounter _sequential_counter (
      ClauseConst& c,
      const size_t lower,
      const size_t upper
    );

    PartialSum _partialSum (
      const Var v,
      ClauseConst& sums
    );

    void _parallel_counter (
      ClauseConst& c,
      const size_t lower,
      const size_t upper
    );

    SatWrapper::HalfAdder _half_adder (
      const SatWrapper::Var a,
      const SatWrapper::Var b
    );

    SatWrapper::FullAdder _full_adder (
      const SatWrapper::Var a,
      const SatWrapper::Var b,
      const SatWrapper::Var c_in
    );
};
#endif // SAT_WRAPPER_HPP
