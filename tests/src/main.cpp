#include "SatWrapper.hpp"

#include <gtest/gtest.h>

TEST(TseitinPrimitives, AndOpSat) {
  SatWrapper sat;

  const auto inp1 = sat.get_new_var();
  const auto inp2 = sat.get_new_var();

  const auto& l1 = sat.get_literal (inp1);
  const auto& l2 = sat.get_literal (inp2);

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(l1);
  sat.add_literal(l2);

  const auto& and_gate = sat.and_clause(c);

  const auto& and_gate_on = sat.get_literal(and_gate);

  const auto is_sat = sat.solve(and_gate_on);

  EXPECT_EQ(true, is_sat);
}

TEST(TseitinPrimitives, AndOpUnsat) {
  SatWrapper sat;

  const auto inp1 = sat.get_new_var();
  const auto inp2 = sat.get_new_var();

  const auto& l1 = sat.get_literal (inp1);
  const auto& l2 = sat.get_literal (inp2);

  SatWrapper::Clause c;

  c.reserve(2);

  c.push_back(l1);
  c.push_back(l2);

  sat.add_literal(sat.negate(l1));
  sat.add_literal(l2);

  const auto& and_gate = sat.and_clause(c);

  const auto& and_gate_on = sat.get_literal(and_gate);

  const auto is_sat = sat.solve(and_gate_on);

  EXPECT_EQ(false, is_sat);
}

TEST(TseitinPrimitives, OrOpSat) {
  SatWrapper sat;

  const auto inp1 = sat.get_new_var();
  const auto inp2 = sat.get_new_var();

  const auto& l1 = sat.get_literal (inp1);
  const auto& l2 = sat.get_literal (inp2);

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

TEST(TseitinPrimitives, OrOpUnsat) {
  SatWrapper sat;

  const auto inp1 = sat.get_new_var();
  const auto inp2 = sat.get_new_var();

  const auto& l1 = sat.get_literal (inp1);
  const auto& l2 = sat.get_literal (inp2);

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

  const auto inp1 = sat.get_new_var();
  const auto inp2 = sat.get_new_var();

  const auto& l1 = sat.get_literal (inp1);
  const auto& l2 = sat.get_literal (inp2);

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

TEST(TseitinPrimitives, XorOpUnsat) {
  SatWrapper sat;

  const auto inp1 = sat.get_new_var();
  const auto inp2 = sat.get_new_var();

  const auto& l1 = sat.get_literal (inp1);
  const auto& l2 = sat.get_literal (inp2);

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

TEST(TseitinPrimitives, NandOp) {
  const int a = 1;
  const int b = 2;

  const auto is_sat = 0;
  EXPECT_EQ(is_sat, a+b);
}


TEST(TseitinPrimitives, NorOp) {
  const int a = 1;
  const int b = 2;

  const auto is_sat = 0;
  EXPECT_EQ(is_sat, a+b);
}



TEST(TseitinPrimitives, XnorOp) {
  const int a = 1;
  const int b = 2;

  const auto is_sat = 0;
  EXPECT_EQ(is_sat, a+b);
}

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
