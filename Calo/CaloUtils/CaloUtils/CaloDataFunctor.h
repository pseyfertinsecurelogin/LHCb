/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef CALOUTILS_CALODATAFUNCTOR_H 
#define CALOUTILS_CALODATAFUNCTOR_H 1
// ============================================================================
// STD and STL 
// ============================================================================
#include <functional>
#include <cmath> 
// ============================================================================
// MathCore
// ============================================================================
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/Point3DTypes.h"
// ============================================================================
// Event 
// ============================================================================
#include "Event/CaloDigit.h"
#include "Event/CaloCluster.h"
#include "Event/CaloClusterEntry.h"
#include "Kernel/CaloCellID.h"
#include "Event/CaloDigitStatus.h"
#include "Event/CaloDataFunctor.h"
// ============================================================================
// CaloDet
// ============================================================================
class DeCalorimeter ;
// ============================================================================
namespace LHCb
{
  // ==========================================================================
  namespace CaloDataFunctor
  {
    // ========================================================================
    /** check if the digit is local maxima 
     *  @param digit the digit to be tested 
     *  @param det the detector ("neighbours provider")
     *  @param data dat avector 
     *  @return true for local maximum
     */
    template <class DETECTOR, class DATA>
    bool isLocalMax 
    ( const LHCb::CaloDigit* digit , 
      const DETECTOR*        det   ,
      const DATA&            data  ) 
    {
      // ======================================================================
      if ( !digit || !det || data.empty() ) { return false ; } // RETURN 
      // get all neighbours 
      const LHCb::CaloCellID::Vector& cells = 
        det->neighborCells ( digit->cellID() ) ;
      return std::none_of( cells.begin(), cells.end(),
                           [&,e=digit->e()](const LHCb::CaloCellID& id) 
                           { const auto* nei = data(id);
                             return nei && nei->e() >= e; } );
      // ======================================================================
    }
    // =========================================================================
    /** check if the digit is local maxima 
     *  @param digit the digit to be tested 
     *  @param det the detector ("neighbours provider")
     *  @param data dat avector 
     *  @return true for local maximum
     */
    template <class DETECTOR, class DATA>
    bool isLocalMax 
    ( const LHCb::CaloDigit* digit , 
      const DETECTOR*        det   ,
      const DATA*            data  ) 
    {
      return data && isLocalMax ( digit , det , *data ) ;
    }
    // ========================================================================
    template <class DETECTOR, class DATA>
    class IsLocalMax 
    {
    public:
      // ======================================================================
      /// constructor from detector and data 
      IsLocalMax ( const DETECTOR* det  , 
                   const DATA*     data ) 
        : m_det ( det ) , m_data ( data ) 
      {}
      // ======================================================================
      // =====================================================================
      /// the only one essential method
      bool operator() ( const LHCb::CaloDigit* digit ) const 
      { return isLocalMax ( digit , m_det , m_data ) ; }
      // =====================================================================
    private:
      // =====================================================================
      /// the detector 
      const DETECTOR* m_det ;                                  // the detector 
      /// data 
      const DATA*     m_data ;                                 //         data
      // =====================================================================  
    };
    // ========================================================================
  } // end of namespace CaloDataFunctor
  // ==========================================================================
}// end of namespace LHCb
// ============================================================================
// The End 
// ============================================================================
#endif // CALOUTILS_CALODATAFUNCTOR_H
// ============================================================================
