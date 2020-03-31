/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
//-----------------------------------------------------------------------------
/** @file UTCommonBase.h
 *
 *  Header file for UT base class : UTCommonBase
 *
 *  @author Andy Beiter (based on code by Matthew Needham)
 *  @date   2018-09-04
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTCOMMONBASE_H
#define UTKERNEL_UTCOMMONBASE_H 1

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/IUTReadoutTool.h"
#include <map>
#include <string>
#include <vector>

class DeUTDetector;
class DeUTSector;

namespace LHCb {
  class UTChannelID;
}

namespace UT {

  //-----------------------------------------------------------------------------
  /** @class CommonBase UTCommonBase.h UTKernel/UTCommonBase.h
   *
   *  Base class providing common functionality for all UT tools and algorithms
   *
   *  @author Andy Beiter (based on code by Matthew Needham)
   *  @date   2018-09-04
   */
  //-----------------------------------------------------------------------------

  template <class PBASE, class IReadoutTool = IUTReadoutTool>
  class CommonBase : public PBASE {

  public:
    using PBASE::PBASE;

    /** Initialization of the algorithm after creation
     *
     * @return The status of the initialization
     * @retval StatusCode::SUCCESS Initialization was successful
     * @retval StatusCode::FAILURE Initialization failed
     */
    StatusCode initialize() override;

    /** get the top level detector element */
    DeUTDetector* tracker() const;

    /** get the readout tool */
    IReadoutTool* readoutTool() const;

    /** force init of base class tool */
    void setForcedInit();

    /** set the detType */
    void setDetType( const std::string& aString );

    /** detector type */
    const std::string& detType() const;

    /** station as a string */
    std::string station( const LHCb::UTChannelID& chan ) const;

    /** region as string */
    std::string uniqueDetRegion( const LHCb::UTChannelID& chan ) const;

    /** layer as a string */
    std::string uniqueLayer( const LHCb::UTChannelID& chan ) const;

    /** sector as a string */
    std::string uniqueSector( const LHCb::UTChannelID& chan ) const;

    /** beetle as a string */
    std::string uniqueBeetle( const LHCb::UTChannelID& chan ) const;

    /** port */
    std::string uniquePort( const LHCb::UTChannelID& chan ) const;

    /** detector type as a string */
    std::string detectorType( const LHCb::UTChannelID& chan ) const;

    /** safe finding of the sector - exception thrown if not valid */
    DeUTSector* findSector( const LHCb::UTChannelID& aChannel ) const;

    /** return a procstatus in case event is aborted */
    StatusCode procFailure( const std::string& reason, const bool aborted = false ) const;

  private:
    DeUTDetector* getTracker() const;

    IReadoutTool* getReadoutTool() const;

    mutable DeUTDetector*            m_tracker{nullptr};
    mutable IReadoutTool*            m_readoutTool{nullptr};
    Gaudi::Property<std::string>     m_detType{this, "DetType", "UT"};
    Gaudi::Property<std::string>     m_readoutToolName{this, "ReadoutTool", m_detType.value() + "ReadoutTool"};
    mutable std::vector<::Property*> m_toBeFlipped;

    Gaudi::Property<bool> m_forcedInit{this, "ForcedInit", false};
  };
} // namespace UT

// GaudiAlg
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiAlg/GaudiTupleTool.h"
#include "Kernel/LHCbConstants.h"
#include "Kernel/UTLexicalCaster.h"

//=============================================================================
// Initialisation
//=============================================================================
template <class PBASE, class IReadoutTool>
StatusCode UT::CommonBase<PBASE, IReadoutTool>::initialize() {
  // Execute the base class initialize
  const StatusCode sc = PBASE::initialize();
  if ( sc.isFailure() ) return sc;

  // Printout from initialize
  if ( this->msgLevel( MSG::DEBUG ) ) this->debug() << "Initialize" << endmsg;

  if ( m_forcedInit ) {
    // force getting of the tools
    getTracker();
    getReadoutTool();
  }

  return sc;
}
//=============================================================================

template <class PBASE, class IReadoutTool>
void UT::CommonBase<PBASE, IReadoutTool>::setDetType( const std::string& aString ) {
  m_detType = aString;
}

#include "UTDet/DeUTDetector.h"

template <class PBASE, class IReadoutTool>
DeUTDetector* UT::CommonBase<PBASE, IReadoutTool>::tracker() const {
  return m_tracker ? m_tracker : getTracker();
}

#include "UTDet/DeUTSector.h"

template <class PBASE, class IReadoutTool>
DeUTSector* UT::CommonBase<PBASE, IReadoutTool>::findSector( const LHCb::UTChannelID& aChannel ) const {

  DeUTSector* sector = tracker()->findSector( aChannel );
  if ( !sector ) throw GaudiException( "No sector found", this->name(), StatusCode::FAILURE );
  return sector;
}

template <class PBASE, class IReadoutTool>
DeUTDetector* UT::CommonBase<PBASE, IReadoutTool>::getTracker() const {
  m_tracker = this->template getDet<DeUTDetector>( DeUTDetLocation::location() );
  return m_tracker;
}

template <class PBASE, class IReadoutTool>
IReadoutTool* UT::CommonBase<PBASE, IReadoutTool>::readoutTool() const {
  return m_readoutTool ? m_readoutTool : getReadoutTool();
}

template <class PBASE, class IReadoutTool>
IReadoutTool* UT::CommonBase<PBASE, IReadoutTool>::getReadoutTool() const {
  m_readoutTool = this->template tool<IReadoutTool>( m_readoutToolName, m_readoutToolName );
  return m_readoutTool;
}

template <class PBASE, class IReadoutTool>
void UT::CommonBase<PBASE, IReadoutTool>::setForcedInit() {
  m_forcedInit = true;
}

template <class PBASE, class IReadoutTool>
const std::string& UT::CommonBase<PBASE, IReadoutTool>::detType() const {
  return m_detType;
}

#include "Kernel/UTChannelID.h"
#include "Kernel/UTNames.h"

template <class PBASE, class IReadoutTool>
std::string UT::CommonBase<PBASE, IReadoutTool>::station( const LHCb::UTChannelID& chan ) const {
  return LHCb::UTNames().StationToString( chan );
}

template <class PBASE, class IReadoutTool>
std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueDetRegion( const LHCb::UTChannelID& chan ) const {
  return LHCb::UTNames().UniqueRegionToString( chan );
}

template <class PBASE, class IReadoutTool>
std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueLayer( const LHCb::UTChannelID& chan ) const {
  return LHCb::UTNames().UniqueLayerToString( chan );
}

template <class PBASE, class IReadoutTool>
std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueSector( const LHCb::UTChannelID& chan ) const {
  return LHCb::UTNames().UniqueSectorToString( chan );
}

template <class PBASE, class IReadoutTool>
std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueBeetle( const LHCb::UTChannelID& chan ) const {
  const DeUTSector* theSector = findSector( chan );
  return theSector->nickname() + "Beetle" + UT::toString( theSector->beetle( chan ) );
  ;
}

template <class PBASE, class IReadoutTool>
std::string UT::CommonBase<PBASE, IReadoutTool>::uniquePort( const LHCb::UTChannelID& chan ) const {
  const unsigned int port = ( ( chan.strip() - 1u ) / LHCbConstants::nStripsInPort ) + 1u;
  return uniqueBeetle( chan ) + "Port" + UT::toString( port );
  ;
}

template <class PBASE, class IReadoutTool>
std::string UT::CommonBase<PBASE, IReadoutTool>::detectorType( const LHCb::UTChannelID& chan ) const {
  const DeUTSector* sector = tracker()->findSector( chan );
  return sector ? sector->type() : "Unknown";
}

#include "Event/ProcStatus.h"
template <class PBASE, class IReadoutTool>
StatusCode UT::CommonBase<PBASE, IReadoutTool>::procFailure( const std::string& reason, const bool aborted ) const {

  LHCb::ProcStatus* procStat =
      this->template getOrCreate<LHCb::ProcStatus, LHCb::ProcStatus>( LHCb::ProcStatusLocation::Default );

  procStat->addAlgorithmStatus( this->name(), "UT", reason, -3, aborted );
  return this->Warning( "Processing failed: " + reason, StatusCode::SUCCESS, 1 );
}

#endif // UTKERNEL_UTCOMMONBASE_H
