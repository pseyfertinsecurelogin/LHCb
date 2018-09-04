//-----------------------------------------------------------------------------
/** @file UTCommonBase.h
 *
 *  Header file for UT base class : UTCommonBase
 *
 *  @author Matthew Needham    Matthew.Needham@cern.ch
 *  @date   2005-11-10
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTCOMMONBASE_H
#define UTKERNEL_UTCOMMONBASE_H 1

#include <vector>
#include <string>
#include <map>
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/IUTReadoutTool.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiAlgorithm.h"

class DeUTDetector;
class DeUTSector;

namespace LHCb{
  class UTChannelID;
}

namespace UT
{

  //-----------------------------------------------------------------------------
  /** @class CommonBase UTCommonBase.h UTKernel/UTCommonBase.h
   *
   *  Base class providing common functionality for all UT tools and algorithms
   *
   *  @author Matthew Needham
   *  @date   2008-10-11
   */
  //-----------------------------------------------------------------------------

  template <class PBASE, class IReadoutTool = IUTReadoutTool>
  class CommonBase : public PBASE
  {

  public:

    /// Standard algorithm-like constructor, only enable for base classes inheriting GaudiAlgorithm
    template<typename A = void, typename = typename std::enable_if<std::is_base_of<GaudiAlgorithm, PBASE>::value, A>::type>
    CommonBase( const std::string& name,
                ISvcLocator* pSvcLocator ) : PBASE(name, pSvcLocator) {
      declareUTConfigProperty("ReadoutTool",m_readoutToolName, m_detType+"ReadoutTool" );
    }

    /// Standard tool-like constructor, only enable for base classes inheriting GaudiTool
    template<typename A = void, typename = typename std::enable_if<std::is_base_of<GaudiTool, PBASE>::value, A>::type>
    CommonBase( const std::string& type,
                const std::string& name,
                const IInterface* parent ) : PBASE(type, name, parent) {
      declareUTConfigProperty("ReadoutTool",m_readoutToolName, m_detType+"ReadoutTool" );
    }

    /// Destructor
    virtual ~CommonBase( );

    /** Initialization of the algorithm after creation
     *
     * @return The status of the initialization
     * @retval StatusCode::SUCCESS Initialization was successful
     * @retval StatusCode::FAILURE Initialization failed
     */
    StatusCode initialize() override;

    /** Finalization of the algorithm before deletion
     *
     * @return The status of the finalization
     * @retval StatusCode::SUCCESS Finalization was successful
     * @retval StatusCode::FAILURE Finalization failed
     */
   StatusCode finalize() override;

   /** get the top level detector element */
   DeUTDetector* tracker() const;

   /** get the readout tool */
   IReadoutTool* readoutTool() const;

   /** force init of base class tool */
   void setForcedInit();

   /** set the detType */
   void setDetType(const std::string& aString);

   /** detector type */
   const std::string& detType() const;

   /** station as a string */
   std::string station(const LHCb::UTChannelID& chan) const;

   /** region as string */
   std::string uniqueDetRegion(const LHCb::UTChannelID& chan) const;

   /** layer as a string */
   std::string uniqueLayer(const LHCb::UTChannelID& chan) const;

   /** sector as a string */
   std::string uniqueSector(const LHCb::UTChannelID& chan) const;

   /** beetle as a string */
   std::string uniqueBeetle(const LHCb::UTChannelID& chan) const;

   /** port */
   std::string uniquePort(const LHCb::UTChannelID& chan) const;

   /** detector type as a string */
   std::string detectorType(const LHCb::UTChannelID& chan) const;

   /** flip all flippables **/
   void flip() const;

   /** flip the given string */
   void flip(std::string& aString) const;


   /** declarePropery the UT way **/
   ::Property* declareUTConfigProperty(const std::string& name,
                               std::string& value,
                               const std::string& def,
                               const std::string& doc="none") {
    // add to the property to the list of flippable after the normal property declaration
    return addToFlipList(this->declareProperty(name, value = def , doc));
   }

   /** accessor to the list of things to be flipped */
   const std::vector<::Property*>& flipList() const;

   /** safe finding of the sector - exception thrown if not valid */
   DeUTSector* findSector(const LHCb::UTChannelID& aChannel) const;

   /** return a procstatus in case event is aborted */
   StatusCode procFailure(const std::string& reason, const bool aborted = false) const;

  private:
   /** add to flipable list **/
   ::Property* addToFlipList(::Property* aProperty) const;

   DeUTDetector* getTracker() const;

   IReadoutTool* getReadoutTool() const;

   mutable DeUTDetector* m_tracker{nullptr};
   mutable IReadoutTool* m_readoutTool{nullptr};
   std::string m_readoutToolName;
   mutable std::vector<::Property*> m_toBeFlipped;

   Gaudi::Property<std::string> m_detType{this, "DetType", "UT"};
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
#include "Kernel/UTLexicalCaster.h"
#include "Kernel/LHCbConstants.h"

// ============================================================================
// Disable warning on windows missing definition for template instantiation
#ifdef _WIN32
#pragma warning ( disable:4661 )
#endif
// ============================================================================

//=============================================================================
// Destructor
//=============================================================================
template <class PBASE, class IReadoutTool>
UT::CommonBase<PBASE, IReadoutTool>::~CommonBase() {}

//=============================================================================
// Initialisation
//=============================================================================
template <class PBASE, class IReadoutTool>
StatusCode UT::CommonBase<PBASE, IReadoutTool>::initialize()
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

//=============================================================================
// Finalisation
//=============================================================================
template <class PBASE, class IReadoutTool>
StatusCode UT::CommonBase<PBASE, IReadoutTool>::finalize()
{
  // Printout from finalization
  if ( this -> msgLevel(MSG::DEBUG) )
    this -> debug() << "Finalize" << endmsg;

  // Finalise base class and return
  return PBASE::finalize();
}
//=============================================================================

template <class PBASE, class IReadoutTool>
inline void UT::CommonBase<PBASE, IReadoutTool>::setDetType(const std::string& aString){
 m_detType = aString;
}

#include "UTDet/DeUTDetector.h"

template <class PBASE, class IReadoutTool>
inline DeUTDetector* UT::CommonBase<PBASE, IReadoutTool>::tracker() const {
  return m_tracker != 0 ? m_tracker : getTracker();
}

#include "UTDet/DeUTSector.h"

template <class PBASE, class IReadoutTool>
inline DeUTSector* UT::CommonBase<PBASE, IReadoutTool>::findSector(const LHCb::UTChannelID&
                                                     aChannel) const{

  DeUTSector* sector = tracker()->findSector(aChannel);
  if (sector == 0) throw GaudiException( "No sector found",
                         this->name(), StatusCode::FAILURE );

  return sector;
}

template <class PBASE, class IReadoutTool>
inline DeUTDetector* UT::CommonBase<PBASE, IReadoutTool>::getTracker() const {
  m_tracker = this->template getDet<DeUTDetector>(DeUTDetLocation::location());
  return m_tracker;
}

template <class PBASE, class IReadoutTool>
inline IReadoutTool* UT::CommonBase<PBASE, IReadoutTool>::readoutTool() const {
  return m_readoutTool != 0 ? m_readoutTool : getReadoutTool();
}

template <class PBASE, class IReadoutTool>
inline IReadoutTool* UT::CommonBase<PBASE, IReadoutTool>::getReadoutTool() const {
  m_readoutTool = this -> template tool<IReadoutTool>(m_readoutToolName,m_readoutToolName);
  return m_readoutTool;
}

template <class PBASE, class IReadoutTool>
void UT::CommonBase<PBASE, IReadoutTool>::setForcedInit() {
  m_forcedInit = true;
}

template <class PBASE, class IReadoutTool>
inline const std::string& UT::CommonBase<PBASE, IReadoutTool>::detType() const {
  return m_detType;
}

#include "Kernel/UTNames.h"
#include "Kernel/UTChannelID.h"

template <class PBASE, class IReadoutTool>
inline std::string UT::CommonBase<PBASE, IReadoutTool>::station(const LHCb::UTChannelID& chan) const{
  return ( LHCb::UTNames().StationToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueDetRegion(const LHCb::UTChannelID& chan) const{
  return ( LHCb::UTNames().UniqueRegionToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueLayer(const LHCb::UTChannelID& chan) const{
  return ( LHCb::UTNames().UniqueLayerToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueSector(const LHCb::UTChannelID& chan) const{
  return ( LHCb::UTNames().UniqueSectorToString(chan) );
}

template <class PBASE, class IReadoutTool>
inline std::string UT::CommonBase<PBASE, IReadoutTool>::uniqueBeetle(const LHCb::UTChannelID& chan) const{
  const DeUTSector* theSector = findSector(chan);
  return theSector->nickname() + "Beetle" + UT::toString(theSector->beetle(chan)); ;
}

template <class PBASE, class IReadoutTool>
inline std::string UT::CommonBase<PBASE, IReadoutTool>::uniquePort(const LHCb::UTChannelID& chan) const{
  const unsigned int port = ((chan.strip()-1u)/LHCbConstants::nStripsInPort) + 1u;
  return uniqueBeetle(chan) + "Port" + UT::toString(port); ;
}

template <class PBASE, class IReadoutTool>
inline std::string UT::CommonBase<PBASE, IReadoutTool>::detectorType(const LHCb::UTChannelID& chan) const{
  const DeUTSector* sector = tracker()->findSector(chan);
  return sector ? sector->type()  : "Unknown" ;
}

#include "Kernel/UTDetSwitch.h"
template <class PBASE, class IReadoutTool>
inline Property* UT::CommonBase<PBASE, IReadoutTool>::addToFlipList(Property* aProperty) const{
  m_toBeFlipped.push_back(aProperty);
  return aProperty;
}

template <class PBASE, class IReadoutTool>
inline void UT::CommonBase<PBASE, IReadoutTool>::flip() const {
 // turn IT into TT or TT into IT
 for (Property* p : m_toBeFlipped ) {
     auto value = p->toString();
     UTDetSwitch::flip(m_detType,value);
     p->fromString(value).ignore();
 }
}

template <class PBASE, class IReadoutTool>
void UT::CommonBase<PBASE, IReadoutTool>::flip(std::string& aString) const{
  UTDetSwitch::flip(m_detType,aString);
}

template <class PBASE, class IReadoutTool>
inline const std::vector<Property*>& UT::CommonBase<PBASE, IReadoutTool>::flipList() const{
 return m_toBeFlipped;
}

#include "Event/ProcStatus.h"
template <class PBASE, class IReadoutTool>
inline StatusCode UT::CommonBase<PBASE, IReadoutTool>::procFailure(const std::string& reason, const bool aborted) const{

   LHCb::ProcStatus* procStat =
      this-> template getOrCreate<LHCb::ProcStatus,LHCb::ProcStatus>(LHCb::ProcStatusLocation::Default);

   procStat->addAlgorithmStatus(this->name(),"UT" ,reason , -3, aborted);
   return this->Warning("Processing failed: " + reason, StatusCode::SUCCESS, 1);
}

#endif // UTKERNEL_UTCOMMONBASE_H
