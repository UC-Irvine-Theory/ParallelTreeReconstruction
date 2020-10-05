#ifndef TEST_UTIL_HPP_
#define TEST_UTIL_HPP_

#include <ostream>

#include "wang.hpp"

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

#define EXPECT_EXCEPTION(code, expected_exception) { \
    bool caught_exception = false; \
    try { code; } \
    catch (const expected_exception&) { \
      caught_exception = true; \
    } \
    catch (...) { \
      assertm(false, "Expected: " #code " throws an exception of type " \
                     #expected_exception ".\n  \
                     Actual: it throws a different type."); \
    } \
    if (!caught_exception) { \
      assertm(false, "Expected: " #code " throws an exception of type " \
                     #expected_exception ".\n  \
                     Actual: no exception was thrown."); \
    } \
  }


bool operator==(const MultidirectionalPath& p1,
                const MultidirectionalPath& p2) {
  return p1.left == p2.left && p1.right == p2.right;
}

#endif
