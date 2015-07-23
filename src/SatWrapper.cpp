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
  ASSERT(c.size() == 2, "Must contain 2 elements, use XOR ith gate literals to deal with set of variables");

  const auto gate = get_new_var();

  const auto& gate_literal = get_literal(gate);
  const auto& not_gate_literal = negate(gate_literal);

  const auto& l1 = c.front();
  const auto& l2 = c.back();

  const auto& neg_l1 = negate(l1);
  const auto& neg_l2 = negate(l2);

  Clause tmp;
  tmp.reserve(3);

  tmp.push_back(not_gate_literal);
  tmp.push_back(l1);
  tmp.push_back(l2);

  add_clause(tmp);

  tmp.clear();

  tmp.push_back(not_gate_literal);
  tmp.push_back(neg_l1);
  tmp.push_back(neg_l2);

  add_clause(tmp);

  tmp.clear();

  tmp.push_back(gate_literal);
  tmp.push_back(l1);
  tmp.push_back(neg_l2);

  add_clause(tmp);

  tmp.clear();

  tmp.push_back(gate_literal);
  tmp.push_back(neg_l1);
  tmp.push_back(l2);

  add_clause(tmp);

  return gate;
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
