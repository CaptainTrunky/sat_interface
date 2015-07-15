#include "Journal.hpp"
#include <iostream>

#include <simp/SimpSolver.h>

#include "SatWrapper.hpp"

int main (int argc, char* argv[]) {
  LOG(true) << "ololo" << std::endl;

  Glucose::SimpSolver s;
  SatWrapper sat(s);

  Glucose::Lit tmp = Glucose::mkLit(2,false);

  auto t = sat.get_solver();

  t.addClause(tmp);
  t.printLit(tmp);

  return 0;
}
