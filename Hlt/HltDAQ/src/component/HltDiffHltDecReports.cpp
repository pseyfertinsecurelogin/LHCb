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
// Include files
// from Gaudi
#include "Event/HltDecReports.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include <iterator>

class HltDiffHltDecReports : public GaudiAlgorithm {
public:
  /// Standard constructor
  HltDiffHltDecReports( const std::string& name, ISvcLocator* pSvcLocator );
  ~HltDiffHltDecReports() override = default; ///< Destructor
  StatusCode execute() override;              ///< Algorithm execution
private:
  std::string m_lhs;
  std::string m_rhs;
};

//-----------------------------------------------------------------------------
// Implementation file for class : HltDiffHltDecReports
//
// 2009-05-26 : Gerhard Raven
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( HltDiffHltDecReports )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltDiffHltDecReports::HltDiffHltDecReports( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "HltDecReportsLHS", m_lhs );
  declareProperty( "HltDecReportsRHS", m_rhs );
}

//=============================================================================
// Main execution
//=============================================================================

StatusCode HltDiffHltDecReports::execute() {
  // grab the two reports...
  auto lhs = get<LHCb::HltDecReports>( m_lhs );
  auto rhs = get<LHCb::HltDecReports>( m_rhs );

  if ( lhs->configuredTCK() != rhs->configuredTCK() ) {
    always() << " configuredTCK: " << m_lhs << " : " << lhs->configuredTCK() << m_rhs << " : " << rhs->configuredTCK()
             << endmsg;
  }

  auto l = lhs->begin(), r = rhs->begin();
  while ( l != lhs->end() || r != rhs->end() ) {
    auto rend = ( r == rhs->end() );
    auto lend = ( l == lhs->end() );

    if ( !lend && ( rend || l->first < r->first ) ) {
      always() << " only in " << m_lhs << " : " << l->first << " : " << l->second << endmsg;
      ++l;
    } else if ( !rend && ( lend || r->first < l->first ) ) {
      always() << " only in " << m_rhs << " : " << r->first << " : " << r->second << endmsg;
      ++r;
    } else {
      if ( l->second.decReport() != r->second.decReport() ) {
        always() << " dif: " << l->first << " : " << l->second << "  <-> " << r->second << endmsg;
      } else {
        // always() << " match: " << l->first << " : " << l->second << endmsg;
      }
      ++l;
      ++r;
    }
  }
  return StatusCode::SUCCESS;
}
