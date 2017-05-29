/*
 * test_allocatorReleaser.cpp
 *
 *  Created on: 16/jul/2012
 *      Author: Marco Clemencic
 */
#define BOOST_TEST_MODULE BoostAllocatorReleaser test
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/logical/bool.hpp>
#include <boost/preprocessor/logical/compl.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/variadic/elem.hpp>
// header-only Boost test framework
#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/old/impl.hpp>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/tree/decorator.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/utils/wrap_stringstream.hpp>
#include <sstream>
#include <string>
// requires library "unit_test_framework"
// #include <boost/test/unit_test.hpp>

#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"

// enable debug output in BoostAllocatorReleaser
#define DEBUG_MEMORY_POOL_ALLOCATOR_RELEASER
#include "GaudiObjDesc/MemoryPoolAllocatorReleaser.h"

/// Dummy event model class to test the release method
class EventModelClass {
public:
  /// Method required by the BoostAllocatorReleaser protocol.
  static bool release_pool() {
    // increase the counter of calls to this method
    ++call_count;
    return true;
  }
  /// Counter for the calls to the release_pool method.
  static int call_count;
};
int EventModelClass::call_count = 0;

/// Dummy event model class to test the release method
class AnotherClass {
public:
  /// Method required by the BoostAllocatorReleaser protocol.
  static bool release_pool() {
    // increase the counter of calls to this method
    ++call_count;
    return false;
  }
  /// Counter for the calls to the release_pool method.
  static int call_count;
};
int AnotherClass::call_count = 0;

BOOST_AUTO_TEST_CASE( test )
{

  // Register the two event model classes
  Gaudi::RegisterReleaseFunction<EventModelClass> r1;
  Gaudi::RegisterReleaseFunction<AnotherClass>    r2;
  // Register one of them again to check the correct handling of duplicates
  Gaudi::RegisterReleaseFunction<EventModelClass> r1b;

  // ensure that the counters are cleared
  EventModelClass::call_count = 0;
  AnotherClass::call_count = 0;

  // call the release_pool methods
  Gaudi::MemoryPoolAllocatorReleaser::releaseMemory();

  // check that the counts are correct
  BOOST_CHECK_EQUAL(EventModelClass::call_count, 1);
  BOOST_CHECK_EQUAL(AnotherClass::call_count, 1);

  MsgStream log(0, "test");
  // try once more, with logging
  log << MSG::INFO;
  Gaudi::MemoryPoolAllocatorReleaser::releaseMemory(log);

  // check that the counts are correct
  BOOST_CHECK_EQUAL(EventModelClass::call_count, 2);
  BOOST_CHECK_EQUAL(AnotherClass::call_count, 2);
}
