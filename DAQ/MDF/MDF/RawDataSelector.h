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
//====================================================================

#pragma once

// Include files
#include "GaudiKernel/IEvtSelector.h"
#include "GaudiKernel/Service.h"
#include "MDF/StreamDescriptor.h"
#include <utility>

// Forward declarations
namespace Gaudi {
  class IIODataManager;
  class IDataConnection;
} // namespace Gaudi

/*
 *  LHCb namespace declaration
 */
namespace LHCb {

  /** @class RawDataSelector RawDataSelector.h MDF/RawDataSelector.h
   * Component for reading MDF (Master Data File) files.
   *
   * @author M.Frank
   * @date   12/12/2005
   */
  class RawDataSelector : public extends<Service, IEvtSelector> {
  public:
    /** @class LoopContext
     *
     *  @author  M.Frank
     *  @version 1.0
     */
    class LoopContext : public IEvtSelector::Context {
    protected:
      /// Owning event selector
      const RawDataSelector* m_sel = nullptr;
      /// Connection specs of current file
      std::string m_conSpec;
      /// Data holder
      mutable std::pair<char*, int> m_data{nullptr, 0};
      /// Current file offset
      long long m_fileOffset{0};
      /// Pointer to file manager service
      Gaudi::IIODataManager* m_ioMgr = nullptr;
      /// Pointer to file connection
      Gaudi::IDataConnection* m_connection = nullptr;
      /// Cached pointer to trigger mask of the event selector
      const std::vector<unsigned int>* m_trgMask = nullptr;
      /// Cached pointer to veto mask of the event selector
      const std::vector<unsigned int>* m_vetoMask = nullptr;

    public:
      /// Standard constructor
      LoopContext( const RawDataSelector* pSelector );
      /// Standard destructor
      virtual ~LoopContext() { close(); }
      /// IEvtSelector::Context overload; context identifier
      void* identifier() const override { return (void*)m_sel; }
      /// Connection specification
      const std::string& specs() const { return m_conSpec; }
      /// Access to file offset(if possible)
      virtual long long offset() const { return m_fileOffset; }
      /// Raw data buffer (if it exists)
      virtual std::pair<char*, int> data() const { return m_data; }
      /// Release data buffer and give ownership to caller
      virtual std::pair<char*, int> releaseData() const { return std::exchange( m_data, {nullptr, 0} ); }
      /// Receive event and update communication structure
      virtual StatusCode receiveData( IMessageSvc* msg ) = 0;
      /// Skip N events
      virtual StatusCode skipEvents( IMessageSvc* msg, int numEvt ) = 0;
      /// Set connection
      virtual StatusCode connect( const std::string& specs );
      /// close connection
      virtual void close();
    };

    /// IService implementation: initialize the service
    StatusCode initialize() override;

    /// IService implementation: finalize the service
    StatusCode finalize() override;

    /** Create a new event loop context
     * @param[in,out] refpCtxt  Reference to pointer to store the context
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode createContext( Context*& refpCtxt ) const override = 0;

    /** Access last item in the iteration
     * @param[in,out] refContext Reference to the Context object.
     */
    StatusCode last( Context& /* refContext */ ) const override { return StatusCode::FAILURE; }

    /** Get next iteration item from the event loop context
     * @param[in,out] refCtxt   Reference to the context
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode next( Context& refCtxt ) const override;

    /** Get next iteration item from the event loop context, but skip jump elements
     * @param[in,out] refCtxt   Reference to the context
     * @param[in]     jump      Number of events to be skipped
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode next( Context& refCtxt, int jump ) const override;

    /** Get previous iteration item from the event loop context
     * @param[in,out] refCtxt   Reference to the context
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode previous( Context& refCtxt ) const override;

    /** Get previous iteration item from the event loop context, but skip jump elements
     * @param[in,out] refCtxt   Reference to the context
     * @param[in]     jump      Number of events to be skipped
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode previous( Context& refCtxt, int jump ) const override;

    /** Rewind the dataset
     * @param[in,out] refCtxt   Reference to the context
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode rewind( Context& /* refCtxt */ ) const override { return StatusCode::FAILURE; }

    /** Create new Opaque address corresponding to the current record
     * @param[in,out] refCtxt   Reference to the context
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode createAddress( const Context& refCtxt, IOpaqueAddress*& ) const override;

    /** Release existing event iteration context
     * @param[in,out] refCtxt   Reference to the context
     *
     * @return StatusCode indicating success or failure
     */
    StatusCode releaseContext( Context*& refCtxt ) const override;

    /** Will set a new criteria for the selection of the next list of events and will change
        the state of the context in a way to point to the new list.

        @param cr The new criteria string.
        @param c  Reference pointer to the Context object.
    */
    StatusCode resetCriteria( const std::string& cr, Context& c ) const override;

    /// Access to the file manager
    Gaudi::IIODataManager* fileMgr() const { return m_ioMgr; }

    /// Additional dataspace in buffer [BYTES]
    int additionalSpace() const { return m_addSpace * 1024; }

    /// Access to required trigger mask
    const std::vector<unsigned int>& triggerMask() const { return m_trgMask; }

    /// Access to required veto mask
    const std::vector<unsigned int>& vetoMask() const { return m_vetoMask; }

    /// Service Constructor
    RawDataSelector( const std::string& name, ISvcLocator* svcloc );

    /// Standard destructor
    virtual ~RawDataSelector() {}

  protected:
    /// Definition of the masj type
    typedef std::vector<unsigned int> Mask;

    // Data Members
    /// ROOT class ID in TES
    CLID m_rootCLID = CLID_NULL;
    /// Name of ROOT element in TES
    std::string m_rootName;
    /// Property: name of the file manager service
    std::string m_ioMgrName;
    /// Property: input dataset name
    std::string m_input;
    /// Pointer to file manager service
    Gaudi::IIODataManager* m_ioMgr = nullptr;
    /// Property: First event to process
    int m_skipEvents{0};
    /// Property: printout frequency
    int m_printFreq{0};
    /// Property: additional dataspace to be used to add data [KBYTES]. Default=0
    int m_addSpace{0};
    /// Property: required trigger mask from MDF header (only 128 bits significant)
    Mask m_trgMask;
    /// Property: veto mask from MDF header (only 128 bits significant)
    Mask m_vetoMask;
    /// Event record count
    mutable int m_evtCount = 0;
  };
} // namespace LHCb
