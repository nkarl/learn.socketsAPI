#ifndef TESTUTIL_HPP
#define TESTUTIL_HPP

#include <iostream>

/*********************************************************************
 * Simple class container for storing a test's info.
 *********************************************************************/
class Test {
 public:
  int id;
  std::string Name;
  Test() : id{-1}, Name{""} {}
};

/*********************************************************************
 *  Unit-Test Utility Functions
 *********************************************************************/
// Print out the current test's info as header.
void TestHeader(Test& test);
// Check if the test passes or fails.
void pfCheck(bool check, Test& test);
// Exception handling wrapper for each test.
void errCatch(bool check, Test& test);

#endif