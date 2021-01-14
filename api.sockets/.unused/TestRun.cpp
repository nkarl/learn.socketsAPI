#include "TestRun.hpp"

bool ThisTestRun() {
  static Test thisTest;
  static bool allPassed = true;  // pass/fail mark

  /*******************************************************************************/
  thisTest.Name = "INIT OUTPUT";
  TestHeader(thisTest);
  /*******************************************************************************/
  /* TEST_BODY STARTS BELOW */
  allPassed = []() {
    std::string tString = "THIS IS A TEST STRING.";
    std::cout << tString;
    if (tString != "")
      return true;
    else
      return false;
  }();
  errCatch(allPassed, thisTest);

  /*******************************************************************************/
  thisTest.Name = "REPLACE THIS TEXT";
  TestHeader(thisTest);
  /*******************************************************************************/
  /* TEST_BODY STARTS BELOW */

  return allPassed;
}