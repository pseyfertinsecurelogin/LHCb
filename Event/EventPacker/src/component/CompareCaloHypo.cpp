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
#include "Event/PackedCaloHypo.h"
#include "GaudiAlg/Consumer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareCaloHypo
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class CompareCaloHypo CompareCaloHypo.h
 *  Compare two containers of CaloHypo
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class CompareCaloHypo : public Gaudi::Functional::Consumer<void( const LHCb::CaloHypos&, const LHCb::CaloHypos& )> {

public:
  /// Standard constructor
  CompareCaloHypo( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {KeyValue{"InputName", LHCb::CaloHypoLocation::Electrons},
                  KeyValue{"TestName", LHCb::CaloHypoLocation::Electrons + "Test"}}} {}

  void operator()( LHCb::CaloHypos const& old, LHCb::CaloHypos const& test ) const override {
    // compare and return
    const LHCb::CaloHypoPacker packer( this );
    packer.check( old, test ).orThrow( "CompareCaloHypo failed", "CompareCaloHypo" );
  }
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareCaloHypo )
