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

    /// Standard algorithm-like constructor
    CommonBase( const std::string& name,
                ISvcLocator* pSvcLocator );

    /// Standard tool-like constructor
    CommonBase( const std::string& type,
                const std::string& name,
                const IInterface* parent );

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

   void commonInit();

   DeSTDetector* getTracker() const;

   IReadoutTool* getReadoutTool() const;

   mutable DeSTDetector* m_tracker;
   mutable IReadoutTool* m_readoutTool;
   std::string m_readoutToolName;
   std::string m_detType;
   bool m_forcedInit;
   mutable std::vector<::Property*> m_toBeFlipped;

  };
}
#endif // STKERNEL_STCOMMONBASE_H


