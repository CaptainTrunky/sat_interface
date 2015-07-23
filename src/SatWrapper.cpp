#include "SatWrapper.hpp"

SatWrapper::Var
SatWrapper::not_clause (ClauseConst& c) {
  ASSERT (c.size() == 1, "Must contain one literal");

  Clause neg;
  neg.push_back(~c.front());

  add_clause(neg);

  return get_var(c.front());
}

SatWrapper::Var
SatWrapper::and_clause (ClauseConst& c) {
  const auto gate = get_new_var ();

  const auto gate_literal = get_literal (gate);

  Clause base;
  base.reserve(c.size() + 1);

  base.push_back(gate_literal);
  for (const auto& l: c) {
    base.push_back(negate(l));
  }

  add_clause(base);

  const auto neg_gate_literal = negate(gate_literal);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve(2);

    tail.push_back(neg_gate_literal);
    tail.push_back(l);

    add_clause (tail);

    tail.clear();
  }
  return gate;
}

SatWrapper::Var
SatWrapper::or_clause (ClauseConst& c) {
  const auto gate = get_new_var ();

  const auto gate_literal = get_literal (gate);
  const auto neg_gate_literal = negate(gate_literal);

  Clause base;
  base.reserve(c.size() + 1);

  base.push_back(neg_gate_literal);
  for (const auto& l: c) {
    base.push_back(l);
  }

  add_clause(base);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve(2);

    tail.push_back(gate_literal);
    tail.push_back(negate(l));

    add_clause (tail);

    tail.clear();
  }
  return gate;
}

SatWrapper::Var
SatWrapper::nand_clause (ClauseConst& c) {
  ASSERT(false, "Not implemented");
  return -1;
}

SatWrapper::Var
SatWrapper::nor_clause (ClauseConst& c) {
  ASSERT(false, "Not implemented");
  return -1;
}

SatWrapper::Var
SatWrapper::xor_clause (ClauseConst& c) {
  ASSERT(false, "Not implemented");
  return -1;
}

SatWrapper::Var
SatWrapper::xnor_clause (ClauseConst& c) {
  ASSERT(false, "Not implemented");
  return -1;
}

SatWrapper::Var
SatWrapper::cardinality_clause (ClauseConst& c,
  const size_t lower,
  const size_t upper) {
  ASSERT(false, "Not implemented");
  return -1;
}
