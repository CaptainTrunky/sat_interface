#include "SatWrapper.hpp"

#include <algorithm>
#include <math.h>

SatWrapper::Var
SatWrapper::not_clause (ClauseConst& c) {
  ASSERT (c.size() == 1, "Must contain one literal");

  const auto& neg = negate (get_literal (c.front ()));

  add_literal (neg);

  return c.front ();
}

SatWrapper::Var
SatWrapper::and_clause (ClauseConst& c) {
  const auto gate = get_new_var ();

  Clause base;
  base.reserve (c.size () + 1);

  base.push_back(gate);
  for (const auto& l: c) {
    base.push_back (negate (l));
  }

  add_clause(base);

  const auto neg_gate = negate (gate);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve (2);

    tail.push_back (neg_gate);
    tail.push_back (l);

    add_clause (tail);

    tail.clear ();
  }
  return gate;
}

SatWrapper::Var
SatWrapper::or_clause (ClauseConst& c) {
  const auto gate = get_new_var ();

  const auto neg_gate = negate (gate);

  Clause base;
  base.reserve (c.size() + 1);

  base.push_back (neg_gate);
  for (const auto& l: c) {
    base.push_back (l);
  }

  add_clause (base);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve (2);

    tail.push_back (gate);
    tail.push_back (negate (l));

    add_clause (tail);

    tail.clear ();
  }
  return gate;
}

SatWrapper::Var
SatWrapper::nand_clause (ClauseConst& c) {
  const auto gate = get_new_var ();

  const auto neg_gate = negate(gate);

  Clause base;
  base.reserve (c.size () + 1);

  base.push_back (neg_gate);
  for (const auto& l: c) {
    base.push_back (negate( l));
  }

  add_clause (base);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve (2);

    tail.push_back (gate);
    tail.push_back (l);

    add_clause (tail);

    tail.clear ();
  }
  return gate;
}

SatWrapper::Var
SatWrapper::nor_clause (ClauseConst& c) {
  const auto gate = get_new_var ();

  const auto neg_gate = negate (gate);

  Clause base;
  base.reserve (c.size() + 1);

  base.push_back (gate);
  for (const auto& l: c) {
    base.push_back (l);
  }

  add_clause (base);

  Clause tail;
  for (const auto& l: c) {
    tail.reserve (2);

    tail.push_back (neg_gate);
    tail.push_back (negate (l));

    add_clause (tail);

    tail.clear ();
  }
  return gate;
}

SatWrapper::Var
SatWrapper::xor_clause (ClauseConst& c) {
  ASSERT(c.size () == 2, "Must contain 2 elements, use XOR gate literals to deal with set of variables");

  const auto gate = get_new_var ();

  const auto& not_gate = negate (gate);

  const auto& l1 = c.front ();
  const auto& l2 = c.back ();

  const auto& neg_l1 = negate (l1);
  const auto& neg_l2 = negate (l2);

  Clause tmp;
  tmp.reserve (3);

  tmp.push_back (not_gate);
  tmp.push_back (l1);
  tmp.push_back (l2);

  add_clause (tmp);

  tmp.clear ();

  tmp.push_back (not_gate);
  tmp.push_back (neg_l1);
  tmp.push_back (neg_l2);

  add_clause (tmp);

  tmp.clear ();

  tmp.push_back (gate);
  tmp.push_back (l1);
  tmp.push_back (neg_l2);

  add_clause (tmp);

  tmp.clear ();

  tmp.push_back (gate);
  tmp.push_back (neg_l1);
  tmp.push_back (l2);

  add_clause (tmp);

  return gate;
}

SatWrapper::Var
SatWrapper::xnor_clause (ClauseConst& c) {
  ASSERT(c.size () == 2, "Must contain 2 elements, use XNOR gate literals to deal with set of variables");

  const auto gate = get_new_var ();

  const auto& not_gate = negate (gate);

  const auto& l1 = c.front ();
  const auto& l2 = c.back ();

  const auto& neg_l1 = negate (l1);
  const auto& neg_l2 = negate (l2);

  Clause tmp;
  tmp.reserve (3);

  tmp.push_back (gate);
  tmp.push_back (l1);
  tmp.push_back (l2);

  add_clause (tmp);

  tmp.clear ();

  tmp.push_back (gate);
  tmp.push_back (neg_l1);
  tmp.push_back (neg_l2);

  add_clause (tmp);

  tmp.clear ();

  tmp.push_back (not_gate);
  tmp.push_back (l1);
  tmp.push_back (neg_l2);

  add_clause (tmp);

  tmp.clear ();

  tmp.push_back (not_gate);
  tmp.push_back (neg_l1);
  tmp.push_back (l2);

  add_clause (tmp);

  return gate;
}

SatWrapper::Var
SatWrapper::cardinality_clause (ClauseConst& c,
  const size_t lower,
  const size_t upper) {
  ASSERT (false, "Not implemented");
  return -1;
}

SatWrapper::Clause
SatWrapper::at_most_clause (ClauseConst& c, const size_t upper) {
  const auto& counter = _sequential_counter (c, 0, upper);

  const auto& overflows = counter.second;

  for (const auto overflow: overflows) {
    const auto neg = negate (overflow);

    add_literal (neg);
  }

  _write_debug_clauses ();
  return counter.first;
}

SatWrapper::SequentialCounter
SatWrapper::_sequential_counter (ClauseConst& c,
  const size_t lower,
  const size_t upper) {
  ASSERT (not c.empty (), "Can't be empty");
  ASSERT (c.size () != 1, "Nothing to count");
  ASSERT (c.size () > lower, "Number of variables must be greater then lower bound, other way, it is always false");
  ASSERT (c.size () > upper, "Number of variables must be lesser then upper bound, other way, it is always true");

  const auto range = upper - lower;
  SatWrapper::Clause current_outs (range);

  const auto fake_sums = SatWrapper::always_false ();

  std::fill (current_outs.begin (), current_outs.end (), fake_sums);

  SatWrapper::Clause overflows;
  overflows.reserve (c.size ());

  // Compute zero step partial sum
  const auto& zero_step = _partialSum (
    c.front (),
    current_outs
  );

  current_outs = zero_step.first;

  overflows.push_back (zero_step.second);

  for (size_t idx = 1; idx < c.size (); ++idx) {
    const auto& l = c [idx];

    const auto& tmp = _partialSum (
      l,
      current_outs
    );

    overflows.push_back (tmp.second);

    current_outs = tmp.first;
  }

  return SequentialCounter (current_outs, overflows);
}

SatWrapper::PartialSum
SatWrapper::_partialSum (
  const SatWrapper::Var v,
  const SatWrapper::Clause& sums) {
  SatWrapper::Clause outs;
  outs.resize (sums.size ());

  // sum_1
  SatWrapper::Clause sum_1;

  sum_1.reserve (2);
  sum_1.push_back (sums.front ());
  sum_1.push_back (v);

  const auto s_1 = or_clause (sum_1);

  outs [0] = s_1;

  // All other sums
  for (size_t idx = 1; idx < outs.size (); ++idx) {
    const auto sum_prev = sums [idx-1];

    SatWrapper::Clause a;
    a.reserve (2);

    a.push_back (v);
    a.push_back (sum_prev);

    const auto s = and_clause (a);

    const auto current_sum = sums [idx];

    SatWrapper::Clause o;
    o.reserve (2);
    o.push_back (s);
    o.push_back (current_sum);

    const auto out = or_clause (o);

    outs [idx] = out;
  }

  // Overflow
  SatWrapper::Clause overflow_clause;
  overflow_clause.reserve (2);

  overflow_clause.push_back (v);
  overflow_clause.push_back (sums.back ());

  const auto overflow = and_clause (overflow_clause);

  return PartialSum (outs, overflow);
}

void
SatWrapper::_parallel_counter (
  ClauseConst& c,
  const size_t lower,
  const size_t upper
) {
  ASSERT (not c.empty (), "Can't be empty");
  ASSERT (c.size () != 1, "Nothing to count");
  ASSERT (c.size () > lower, "Number of variables must be greater then lower bound, other way, it is always false");
  ASSERT (c.size () > upper, "Number of variables must be lesser then upper bound, other way, it is always true");
  const auto m = std::floor (
    std::log2 (c.size ())
  );

  const auto inputs_num = static_cast<size_t> (std::pow (2, m));

  const SatWrapper::Clause first (c.begin (), c.begin () + inputs_num);

  _parallel_counter (first, lower, upper);

  const SatWrapper::Clause second (c.begin () + inputs_num, c.end ());

  // FIXME: check for corner cases when there just 1 input and no inputs at all
  if (not second.empty ()) {
    _parallel_counter (second, lower, upper);
  }
}

SatWrapper::HalfAdder
SatWrapper::_half_adder (
  const SatWrapper::Var a,
  const SatWrapper::Var b
  ) {
  SatWrapper::Clause c;

  c.reserve (2);

  c.push_back (a);
  c.push_back (b);

  const auto s = xor_clause (c);
  const auto c_out = and_clause (c);

  return SatWrapper::HalfAdder (s, c_out);
}

SatWrapper::FullAdder
SatWrapper::_full_adder (
  const SatWrapper::Var a,
  const SatWrapper::Var b,
  const SatWrapper::Var c_in
  ) {
  SatWrapper::Clause c;

  c.reserve (2);

  c.push_back (a);
  c.push_back (b);

  const auto xor_gate = xor_clause (c);
  const auto ab_and_gate = and_clause (c);

  c.clear ();

  c.push_back (xor_gate);
  c.push_back (c_in);

  const auto s = xor_clause (c);

  const auto xor_c_in_and_gate = and_clause (c);

  c.clear ();

  c.push_back (xor_c_in_and_gate);
  c.push_back (ab_and_gate);

  const auto c_out = or_clause (c);

  return SatWrapper::FullAdder(s, c_out);
}

