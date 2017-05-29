// ============================================================================
#ifndef CALODET_CALODETTESTALGORITHM_H
#define CALODET_CALODETTESTALGORITHM_H 1
// Include files
// from STL
#include <string>

// CaloDet
#include "CaloDet/DeCalorimeter.h"
// from CaloKernel
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;
template <class T> class AlgFactory;

/** @class CaloDetTestAlgorithm CaloDetTestAlgorithm.h
 *
 *  Simple Test         Algorithm
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   14/12/2001
 */

class CaloDetTestAlgorithm : public GaudiAlgorithm
{
  /// friend factory for instantiation
  friend class AlgFactory<CaloDetTestAlgorithm>;

public:

  /** standard algorithm initialization
   *  @return status code
   */
  StatusCode initialize() override;

protected:

  /** Standard constructor
   *  @param   name   algorithm name
   *  @param   svcloc pointer to service locator
   */
  CaloDetTestAlgorithm( const std::string& name   ,
                        ISvcLocator*       svcloc );

private:
  std::string m_DetData;

};

// ============================================================================
// The End
// ============================================================================
#endif // CaloDetTestAlgorithm_H
// ============================================================================
