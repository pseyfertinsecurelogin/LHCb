//-----------------------------------------------------------------------------
/** @file DecoderCommonBase.h
 *
 *  Header file for DAQ base class : Decoder::CommonBase
 *
 *  @author Rob Lambert    Rob.Lambert@cern.ch
 *  @date   2005-08-27
 */
//-----------------------------------------------------------------------------

#ifndef DAQKERNEL_DECODERCOMMONBASE_H
#define DAQKERNEL_DECODERCOMMONBASE_H 1

//disable unused variable warnings which occur for every enum and string in RawEvent.h
//#pragma GCC diagnostic push
//#pragma GCC diagnostic "-Wunused-variable"
#include "Event/RawEvent.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IInterface.h"

//#pragma GCC diagnostic pop

namespace Decoder
{
  //-----------------------------------------------------------------------------
  /** @class CommonBase DecoderCommonBase.h DAQKernel/DecoderCommonBase.h
   *
   *  Base class providing common functionality for all Decoder tools and algorithms
   *
   *  @author Rob Lambert   Rob.Lambert@cern.ch
   *  @date   2014-02-10
   */
  //-----------------------------------------------------------------------------

  template <class PBASE>
  class CommonBase : public PBASE
  {
  public:
     using PBASE::PBASE;

    /** Initialization of the algorithm after creation
     *
     * @return The status of the initialization
     * @retval StatusCode::SUCCESS Initialization was successful
     * @retval StatusCode::FAILURE Initialization failed
     */
    StatusCode initialize() override;

  protected:

    /** @brief Returns a pointer to the first RawEvent in the search path
     */
    LHCb::RawEvent* findFirstRawEvent( ) const;

    /** @brief Returns a std::vector of raw banks out of a first raw event containing those banks
     */
    LHCb::span<const LHCb::RawBank *> findFirstRawBank( const LHCb::RawBank::BankType ) const;

    //get for one location
    LHCb::RawEvent* tryEventAt( const std::string& ) const;
    /// Where to look for the raw event
    std::vector<std::string> m_rawEventLocations;

    /// Initialize search paths
    void initRawEventSearch();

  private: // data

    /// Where to look first, saves the last place I was able to retrieve successfully
    mutable std::string m_tryFirstRawEventLocation = "";

    /// whether to use the RootOnTes next time
    mutable bool m_tryRootOnTes = true;

  };

}

#endif // DAQKERNEL_DECODERCOMMONBASE_H
