#include "TestUtil.hpp"

void TestHeader(Test& test) {
  ++test.id;
  std::cout << "\n\nTEST #" << test.id << ": " << test.Name;
  std::cout
      << "\n----------------------------------------------------------------"
      << std::endl;
}

void pfCheck(bool check, Test& test) {
  std::cout << "\n\n--OK!--> " << test.Name;
  if (check)
    std::cout << "\nSTATUS: PASSED." << std::endl;
  else
    throw "\n--\?\?\?--> ERROR! FAILED PASSCHECK ON TEST#" +
        std::to_string(test.id) + ": " + test.Name;
}

void errCatch(bool check, Test& test) {
  try {
    pfCheck(check, test);
  } catch (std::string msg) {
    std::cout << msg << std::endl;
  }
}