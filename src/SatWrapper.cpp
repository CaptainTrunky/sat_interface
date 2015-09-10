#include "SatWrapper.hpp"

#include <algorithm>

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
  const auto gate = get_new_var ();

  const auto gate_literal = get_literal (gate);
  const auto neg_gate_literal = negate(gate_literal);

  Clause base;
  base.reserve(c.size() + 1);

  base.push_back(neg_gate_literal);
  for (const auto& l: c) {
    base.push_back(negate(l));
  }

  add_clause(base);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve(2);

    tail.push_back(gate_literal);
    tail.push_back(l);

    add_clause (tail);

    tail.clear();
  }
  return gate;

}

SatWrapper::Var
SatWrapper::nor_clause (ClauseConst& c) {
  const auto gate = get_new_var ();

  const auto gate_literal = get_literal (gate);
  const auto neg_gate_literal = negate(gate_literal);

  Clause base;
  base.reserve(c.size() + 1);

  base.push_back(gate_literal);
  for (const auto& l: c) {
    base.push_back(l);
  }

  add_clause(base);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve(2);

    tail.push_back(neg_gate_literal);
    tail.push_back(negate(l));

    add_clause (tail);

    tail.clear();
  }
  return gate;
}

SatWrapper::Var
SatWrapper::xor_clause (ClauseConst& c) {
  ASSERT(c.size() == 2, "Must contain 2 elements, use XOR gate literals to deal with set of variables");

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
  ASSERT(c.size() == 2, "Must contain 2 elements, use XNOR gate literals to deal with set of variables");

  const auto gate = get_new_var();

  const auto& gate_literal = get_literal(gate);
  const auto& not_gate_literal = negate(gate_literal);

  const auto& l1 = c.front();
  const auto& l2 = c.back();

  const auto& neg_l1 = negate(l1);
  const auto& neg_l2 = negate(l2);

  Clause tmp;
  tmp.reserve(3);

  tmp.push_back(gate_literal);
  tmp.push_back(l1);
  tmp.push_back(l2);

  add_clause(tmp);

  tmp.clear();

  tmp.push_back(gate_literal);
  tmp.push_back(neg_l1);
  tmp.push_back(neg_l2);

  add_clause(tmp);

  tmp.clear();

  tmp.push_back(not_gate_literal);
  tmp.push_back(l1);
  tmp.push_back(neg_l2);

  add_clause(tmp);

  tmp.clear();

  tmp.push_back(not_gate_literal);
  tmp.push_back(neg_l1);
  tmp.push_back(l2);

  add_clause(tmp);

  return gate;
}

SatWrapper::Var
SatWrapper::cardinality_clause (ClauseConst& c,
  const size_t lower,
  const size_t upper) {
  ASSERT(false, "Not implemented");
  return -1;
}

SatWrapper::Var
SatWrapper::_sequential_counter (ClauseConst& c,
  const size_t lower,
  const size_t upper) {
  ASSERT (false, "Not implemented");

  ASSERT (not c.empty(), "Can't be empty");
  ASSERT (c.size() == 1, "Nothing to count");
  ASSERT (c.size() < lower, "Number of variabels must be greater then lower bound, other way, it is always true");
  ASSERT (c.size() > upper, "Number of variables must be lesser then upper bound, other way, it is always true");

  SatWrapper::Clause current_outs;

  const auto tmp = get_new_var ();
  const auto always_neg = get_literal (tmp);

  std::fill (current_outs.begin(), current_outs.end(), always_neg);

  SatWrapper::Clause overflows;
  overflows.reserve (c.size());

  // Compute zero step partial sum
  const auto& zero_step = _partialSum (
    c.front(),
    current_outs
  );

  current_outs = zero_step.first;

  overflows.push_back (zero_step.second);

  for (size_t idx = 1; idx < c.size(); ++idx) {
    const auto& l = c[idx];

    const auto& tmp = _partialSum (
      l,
      current_outs
    );

    overflows.push_back (tmp.second);

    current_outs = tmp.first;
  }

  return -1;
}

SatWrapper::PartialSum
SatWrapper::_partialSum (
  const SatWrapper::Literal& l,
  const SatWrapper::Clause& sums) {
  SatWrapper::Clause outs;
  outs.resize (sums.size());

  // sum_1
  SatWrapper::Clause sum_1;

  sum_1.reserve (2);
  sum_1.push_back (sums.front());
  sum_1.push_back (l);

  const auto s_1 = or_clause (sum_1);
  outs[0] = get_literal (s_1);

  // All other sums

  for (size_t idx = 1; idx < outs.size(); ++idx) {
    const auto sum_prev = sums[idx-1];

    SatWrapper::Clause a;
    a.reserve (2);

    a.push_back (l);
    a.push_back (sum_prev);

    const auto s = and_clause (a);

    const auto s_lit = get_literal (s);

    const auto current_sum = sums[idx];

    SatWrapper::Clause o;
    o.reserve (2);
    o.push_back (s_lit);
    o.push_back (current_sum);

    const auto out = or_clause (o);

    outs[idx] = get_literal (out);
  }

  // Overflow
  SatWrapper::Clause overflow_clause;
  overflow_clause.reserve (2);

  overflow_clause.push_back (l);
  overflow_clause.push_back (sums.back());

  const auto overflow_var = and_clause (overflow_clause);
  const auto overflow = get_literal (overflow_var);

  return PartialSum (outs, overflow);
}

