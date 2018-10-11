/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
//-----------------------------------------------------------------------------
/** @file STCommonBase.h
 *
 *  Header file for ST base class : STCommonBase
 *
 *  @author Matthew Needham    Matthew.Needham@cern.ch
 *  @date   2005-11-10
 */
//-----------------------------------------------------------------------------

#ifndef STKERNEL_STCOMMONBASE_H
#define STKERNEL_STCOMMONBASE_H 1

#include <vector>
#include <string>
#include <map>
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/ISTReadoutTool.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiAlgorithm.h"

class DeSTDetector;
class DeSTSector;

namespace LHCb{
  class STChannelID;
}

namespace ST
{

  //-----------------------------------------------------------------------------
  /** @class CommonBase STCommonBase.h STKernel/STCommonBase.h
   *
   *  Base class providing common functionality for all ST tools and algorithms
   *
   *  @author Matthew Needham
   *  @date   2008-10-11
   */
  //-----------------------------------------------------------------------------

  template <class PBASE, class IReadoutTool = ISTReadoutTool>
  class CommonBase : public PBASE
  {

  public:

    /// Standard algorithm-like constructor, only enable for base classes inheriting GaudiAlgorithm
    template<typename A = void, typename = typename std::enable_if<std::is_base_of<GaudiAlgorithm, PBASE>::value, A>::type>
    CommonBase( const std::string& name,
                ISvcLocator* pSvcLocator ) : PBASE(name, pSvcLocator) {
      declareSTConfigProperty("ReadoutTool",m_readoutToolName, m_detType+"ReadoutTool" );
    }

    /// Standard tool-like constructor, only enable for base classes inheriting GaudiTool
    template<typename A = void, typename = typename std::enable_if<std::is_base_of<GaudiTool, PBASE>::value, A>::type>
    CommonBase( const std::string& type,
                const std::string& name,
                const IInterface* parent ) : PBASE(type, name, parent) {
      declareSTConfigProperty("ReadoutTool",m_readoutToolName, m_detType+"ReadoutTool" );
    }

    /** Initialization of the algorithm after creation
     *
     * @return The status of the initialization
     * @retval StatusCode::SUCCESS Initialization was successful
     * @retval StatusCode::FAILURE Initialization failed
     */
    StatusCode initialize() override;

   /** get the top level detector element */
   DeSTDetector* tracker() const;

   /** get the readout tool */
   IReadoutTool* readoutTool() const;

   /** force init of base class tool */
   void setForcedInit();

   /** set the detType */
   void setDetType(const std::string& aString);

   /** detector type (IT or TT) */
   const std::string& detType() const;

   /** station as a string */
   std::string station(const LHCb::STChannelID& chan) const;

   /** region as string */
   std::string uniqueDetRegion(const LHCb::STChannelID& chan) const;

   /** layer as a string */
   std::string uniqueLayer(const LHCb::STChannelID& chan) const;

   /** sector as a string */
   std::string uniqueSector(const LHCb::STChannelID& chan) const;

   /** beetle as a string */
   std::string uniqueBeetle(const LHCb::STChannelID& chan) const;

   /** port */
   std::string uniquePort(const LHCb::STChannelID& chan) const;

   /** detector type as a string */
   std::string detectorType(const LHCb::STChannelID& chan) const;

   /** flip all flippables **/
   void flip() const;

   /** flip the given string */
   void flip(std::string& aString) const;


   /** declarePropery the ST way **/
   ::Property* declareSTConfigProperty(const std::string& name,
                               std::string& value,
                               const std::string& def,
                               const std::string& doc="none") {
    // add to the property to the list of flippable after the normal property declaration
    return addToFlipList(this->declareProperty(name, value = def , doc));
   }

   /** accessor to the list of things to be flipped */
   const std::vector<::Property*>& flipList() const;

   /** safe finding of the sector - exception thrown if not valid */
   DeSTSector* findSector(const LHCb::STChannelID& aChannel) const;

   /** return a procstatus in case event is aborted */
   StatusCode procFailure(const std::string& reason, const bool aborted = false) const;

  private:
   /** add to flipable list **/
   ::Property* addToFlipList(::Property* aProperty) const;

   DeSTDetector* getTracker() const;

   IReadoutTool* getReadoutTool() const;

   mutable DeSTDetector* m_tracker{nullptr};
   mutable IReadoutTool* m_readoutTool{nullptr};
   std::string m_readoutToolName;
   mutable std::vector<::Property*> m_toBeFlipped;

   Gaudi::Property<std::string> m_detType{this, "DetType", "TT"};
   Gaudi::Property<bool> m_forcedInit{this, "ForcedInit", false};

  };
}

// GaudiAlg
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiAlg/GaudiTupleTool.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/GaudiHistoTool.h"
#include "Kernel/STLexicalCaster.h"
#include "Kernel/LHCbConstants.h"

// ============================================================================
// Disable warning on windows missing definition for template instantiation
#ifdef _WIN32
#pragma warning ( disable:4661 )
#endif
// ============================================================================

//=============================================================================
// Initialisation
//=============================================================================
template <class PBASE, class IReadoutTool>
StatusCode ST::CommonBase<PBASE, IReadoutTool>::initialize()
{
  // Execute the base class initialize
  const StatusCode sc = PBASE::initialize();
  if ( sc.isFailure() )
  { return this -> Error( "Failed to initialise Gaudi Base class", sc ); }

  // Printout from initialize
  if ( this -> msgLevel(MSG::DEBUG) )
    this -> debug() << "Initialize" << endmsg;
  flip(); // flip all that needs to be flipped

  if (m_forcedInit == true){
     // force getting of the tools
     getTracker();
     getReadoutTool();
  }

  return sc;
}
//=============================================================================

template <class PBASE, class IReadoutTool>
inline void ST::CommonBase<PBASE, IReadoutTool>::setDetType(const std::string& aString){
 m_detType = aString;
}

#include "STDet/DeSTDetector.h"

template <class PBASE, class IReadoutTool>
inline DeSTDetector* ST::CommonBase<PBASE, IReadoutTool>::tracker() const {
  return m_tracker != 0 ? m_tracker : getTracker();
}

#include "STDet/DeSTSector.h"

template <class PBASE, class IReadoutTool>
inline DeSTSector* ST::CommonBase<PBASE, IReadoutTool>::findSector(const LHCb::STChannelID&
                                                     aChannel) const{

  DeSTSector* sector = tracker()->findSector(aChannel);
  if (sector == 0) throw GaudiException( "No sector found",
                         this->name(), StatusCode::FAILURE );

  return sector;
}

template <class PBASE, class IReadoutTool>
inline DeSTDetector* ST::CommonBase<PBASE, IReadoutTool>::getTracker() const {
  m_tracker = this->template getDet<DeSTDetector>(DeSTDetLocation::location(m_detType));
  return m_tracker;
}

template <class PBASE, class IReadoutTool>
inline IReadoutTool* ST::CommonBase<PBASE, IReadoutTool>::readoutTool() const {
  return m_readoutTool != 0 ? m_readoutTool : getReadoutTool();
}

template <class PBASE, class IReadoutTool>
inline IReadoutTool* ST::CommonBase<PBASE, IReadoutTool>::getReadoutTool() const {
  m_readoutTool = this -> template tool<IReadoutTool>(m_readoutToolName,m_readoutToolName);
  return m_readoutTool;
}

template <class PBASE, class IReadoutTool>
void ST::CommonBase<PBASE, IReadoutTool>::setForcedInit() {
  m_forcedInit = true;
}

template <class PBASE, class IReadoutTool>
inline const std::string& ST::CommonBase<PBASE, IReadoutTool>::detType() const {
  return m_detType;
}

#include "Kernel/ITNames.h"
#include "Kernel/TTNames.h"
#include "Kernel/UTNames.h"
#include "Kernel/STChannelID.h"

template <class PBASE, class IReadoutTool>
inline std::string ST::CommonBase<PBASE, IReadoutTool>::station(const LHCb::STChannelID& chan) const{
  return ( m_detType == "TT" ? LHCb::TTNames().StationToString(chan) :
           m_detType == "IT" ? LHCb::ITNames().StationToString(chan) :
                               LHCb::UTNames().StationToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string ST::CommonBase<PBASE, IReadoutTool>::uniqueDetRegion(const LHCb::STChannelID& chan) const{
  return ( m_detType == "TT" ? LHCb::TTNames().UniqueRegionToString(chan) :
           m_detType == "IT" ? LHCb::ITNames().UniqueBoxToString(chan) :
                               LHCb::UTNames().UniqueRegionToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string ST::CommonBase<PBASE, IReadoutTool>::uniqueLayer(const LHCb::STChannelID& chan) const{
  return ( m_detType == "TT" ? LHCb::TTNames().UniqueLayerToString(chan) :
           m_detType == "IT" ? LHCb::ITNames().UniqueLayerToString(chan) :
                               LHCb::UTNames().UniqueLayerToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string ST::CommonBase<PBASE, IReadoutTool>::uniqueSector(const LHCb::STChannelID& chan) const{
  return ( m_detType == "TT" ? LHCb::TTNames().UniqueSectorToString(chan) :
           m_detType == "IT" ? LHCb::ITNames().UniqueSectorToString(chan) :
                               LHCb::UTNames().UniqueSectorToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string ST::CommonBase<PBASE, IReadoutTool>::uniqueBeetle(const LHCb::STChannelID& chan) const{
  const DeSTSector* theSector = findSector(chan);
  return theSector->nickname() + "Beetle" + ST::toString(theSector->beetle(chan)); ;
}

template <class PBASE, class IReadoutTool>
inline std::string ST::CommonBase<PBASE, IReadoutTool>::uniquePort(const LHCb::STChannelID& chan) const{
  const unsigned int port = ((chan.strip()-1u)/LHCbConstants::nStripsInPort) + 1u;
  return uniqueBeetle(chan) + "Port" + ST::toString(port); ;
}

template <class PBASE, class IReadoutTool>
inline std::string ST::CommonBase<PBASE, IReadoutTool>::detectorType(const LHCb::STChannelID& chan) const{
  const DeSTSector* sector = tracker()->findSector(chan);
  return sector ? sector->type()  : "Unknown" ;
}

#include "Kernel/STDetSwitch.h"
template <class PBASE, class IReadoutTool>
inline Property* ST::CommonBase<PBASE, IReadoutTool>::addToFlipList(Property* aProperty) const{
  m_toBeFlipped.push_back(aProperty);
  return aProperty;
}

template <class PBASE, class IReadoutTool>
inline void ST::CommonBase<PBASE, IReadoutTool>::flip() const {
 // turn IT into TT or TT into IT
 for (Property* p : m_toBeFlipped ) {
     auto value = p->toString();
     STDetSwitch::flip(m_detType,value);
     p->fromString(value).ignore();
 }
}

template <class PBASE, class IReadoutTool>
void ST::CommonBase<PBASE, IReadoutTool>::flip(std::string& aString) const{
  STDetSwitch::flip(m_detType,aString);
}

template <class PBASE, class IReadoutTool>
inline const std::vector<Property*>& ST::CommonBase<PBASE, IReadoutTool>::flipList() const{
 return m_toBeFlipped;
}

#include "Event/ProcStatus.h"
template <class PBASE, class IReadoutTool>
inline StatusCode ST::CommonBase<PBASE, IReadoutTool>::procFailure(const std::string& reason, const bool aborted) const{

   LHCb::ProcStatus* procStat =
      this-> template getOrCreate<LHCb::ProcStatus,LHCb::ProcStatus>(LHCb::ProcStatusLocation::Default);

   procStat->addAlgorithmStatus(this->name(),"ST" ,reason , -3, aborted);
   return this->Warning("Processing failed: " + reason, StatusCode::SUCCESS, 1);
}

#endif // STKERNEL_STCOMMONBASE_H
