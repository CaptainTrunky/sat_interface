#include "SatWrapper.hpp"

#include <gtest/gtest.h>

TEST (Basic, AddClause) {
  SatWrapper sat;

  const auto inp1 = sat.get_new_var ();
  const auto inp2 = sat.get_new_var ();
  const auto inp3 = sat.get_new_var ();
  const auto inp4 = sat.get_new_var ();

  SatWrapper::Clause c;
  c.push_back (inp1);
  c.push_back (inp2);

  sat.add_clause (c);

  EXPECT_EQ (1, sat.get_solver()->nClauses());

  c.clear();
  c.push_back (sat.negate(inp1));
  c.push_back (inp4);

  sat.add_clause (c);
  EXPECT_EQ (2, sat.get_solver()->nClauses());

  EXPECT_EQ (5, sat.get_solver()->nVars());
}

TEST(TseitinPrimitives, AndOpSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& and_gate = sat.and_clause(c);

  const auto& and_gate_on = sat.get_literal(and_gate);

  const auto is_sat = sat.solve(and_gate_on);

  EXPECT_EQ (true, is_sat);
}

TEST(TseitinPrimitives, AndOpUnSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve (2);

  c.push_back (l1);
  c.push_back (l2);

  sat.add_literal (sat.negate(l1));
  sat.add_literal (l2);

  const auto& and_gate = sat.and_clause(c);

  const auto& and_gate_on = sat.get_literal(and_gate);

  const auto is_sat = sat.solve(and_gate_on);

  EXPECT_EQ(false, is_sat);
}

TEST(TseitinPrimitives, OrOpSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(sat.negate(l1));
  sat.add_literal(l2);

  const auto& or_gate = sat.or_clause(c);

  const auto& or_gate_on = sat.get_literal(or_gate);

  const auto is_sat = sat.solve(or_gate_on);

  EXPECT_EQ(true, is_sat);
}

TEST(TseitinPrimitives, OrOpUnSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(sat.negate(l1));
  sat.add_literal(sat.negate(l2));

  const auto& or_gate = sat.or_clause(c);

  const auto& or_gate_on = sat.get_literal(or_gate);

  const auto is_sat = sat.solve(or_gate_on);

  EXPECT_EQ(false, is_sat);
}

TEST(TseitinPrimitives, XorOpSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(sat.negate(l2));

  const auto& xor_gate = sat.xor_clause(c);

  const auto& xor_gate_on = sat.get_literal(xor_gate);

  const auto is_sat = sat.solve(xor_gate_on);

  EXPECT_EQ(true, is_sat);
}

TEST(TseitinPrimitives, XorOpUnSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& xor_gate = sat.xor_clause(c);

  const auto& xor_gate_on = sat.get_literal(xor_gate);

  const auto is_sat = sat.solve(xor_gate_on);

  EXPECT_EQ(false, is_sat);
}

TEST(TseitinPrimitives, NandOpSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(sat.negate(l1));
  sat.add_literal(sat.negate(l2));

  const auto& nand_gate = sat.nand_clause(c);

  const auto& nand_gate_on = sat.get_literal(nand_gate);

  const auto is_sat = sat.solve(nand_gate_on);

  EXPECT_EQ(true, is_sat);
}

TEST(TseitinPrimitives, NandOpUnSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& nand_gate = sat.nand_clause(c);

  const auto& nand_gate_on = sat.get_literal(nand_gate);

  const auto is_sat = sat.solve(nand_gate_on);

  EXPECT_EQ(false, is_sat);
}

TEST(TseitinPrimitives, NorOpSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(sat.negate(l1));
  c.push_back(sat.negate(l2));

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& nor_gate = sat.nor_clause(c);

  const auto& nor_gate_on = sat.get_literal(nor_gate);

  const auto is_sat = sat.solve(nor_gate_on);

  EXPECT_EQ(true, is_sat);
}

TEST(TseitinPrimitives, NorOpUnSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& nor_gate = sat.nor_clause(c);

  const auto& nor_gate_on = sat.get_literal(nor_gate);

  const auto is_sat = sat.solve(nor_gate_on);

  EXPECT_EQ(false, is_sat);
}

TEST(TseitinPrimitives, XnorOpSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& xnor_gate = sat.xnor_clause(c);

  const auto& xnor_gate_on = sat.get_literal(xnor_gate);

  const auto is_sat = sat.solve(xnor_gate_on);

  EXPECT_EQ(true, is_sat);
}

TEST(Tseitlrimitives, XnorOpUnSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(sat.negate(l1));
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& xnor_gate = sat.xnor_clause(c);

  const auto& xnor_gate_on = sat.get_literal(xnor_gate);

  const auto is_sat = sat.solve(xnor_gate_on);

  EXPECT_EQ(false, is_sat);
}

TEST(Cardinality, AtMostTwoSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();
  const auto l3 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);
  c.push_back(l3);

  const auto& gate = sat.at_most_clause (c, 2);

  SatWrapper::Clause asmp;

  asmp.push_back (l1);
  asmp.push_back (l2);

  auto is_sat = sat.solve(asmp);
  EXPECT_EQ(true, is_sat);

  asmp.clear();

  asmp.push_back (l2);
  asmp.push_back (l3);
  is_sat = sat.solve(asmp);

  EXPECT_EQ(true, is_sat);

  asmp.clear();

  asmp.push_back (l1);
  asmp.push_back (l3);
  is_sat = sat.solve(asmp);

  EXPECT_EQ(true, is_sat);
}

TEST(Cardinality, AtMostTwoUnSat) {
  SatWrapper sat;

  const auto l1 = sat.get_new_var();
  const auto l2 = sat.get_new_var();
  const auto l3 = sat.get_new_var();

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);
  c.push_back(l3);

  const auto& gate = sat.at_most_clause (c, 2);

  SatWrapper::Clause asmp;

  asmp.push_back (l1);
  asmp.push_back (l2);
  asmp.push_back (l3);

  const auto is_sat = sat.solve(asmp);

  EXPECT_EQ(false, is_sat);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
