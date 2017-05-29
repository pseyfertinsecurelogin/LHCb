#ifndef MULTIDBTEST_H
#define MULTIDBTEST_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class AlignmentCondition;
class Condition;
class ISvcLocator;

/** @class MultiDBTest MultiDBTest.h
 *
 *  Algorithm to test the multi-db functionality.
 *
 *  @author Marco Clemencic
 *  @date   2005-05-12
 */
class MultiDBTest : public GaudiAlgorithm {
public:
  /// Standard constructor
  MultiDBTest( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~MultiDBTest( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

protected:

private:

  Condition *m_cond1, *m_cond2;
  AlignmentCondition *m_align1;

};
#endif // MULTIDBTEST_H
