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
#ifndef DAQEVENT_RAWEVENT_H
#define DAQEVENT_RAWEVENT_H 1

// Include files
#include "Event/RawBank.h"
#include "GaudiKernel/DataObject.h"
#include "Kernel/STLExtensions.h"
#include <boost/container/static_vector.hpp>
#include <map>
#include <string>
#include <type_traits>
#include <vector>
namespace LHCb {

  // Class ID definition
  constexpr CLID CLID_RawEvent = 1002;

  // Namespace for locations in TES
  namespace RawEventLocation {
    inline const std::string Default  = "DAQ/RawEvent"; ///< Original FULL Raw Event
    inline const std::string Emulated = "Emu/RawEvent";
    inline const std::string Copied   = "pRec/RawEvent";
    inline const std::string Calo     = "Calo/RawEvent";    ///< Copy of Calo banks
    inline const std::string Muon     = "Muon/RawEvent";    ///< Copy of Muon banks, for MDST
    inline const std::string Rich     = "Rich/RawEvent";    ///< Copy of Rich banks
    inline const std::string Trigger  = "Trigger/RawEvent"; ///< Copy of Trigger banks for Stripping and MDST
    inline const std::string PersistReco =
        "PersistReco/RawEvent"; ///< Copy of PersistReco banks for MC Stripping and MDST
    inline const std::string Other =
        "Other/RawEvent"; ///< Copy of all banks except Calo, Muon, Rich and Trigger (now Obsolete)
    inline const std::string Velo    = "Velo/RawEvent";    ///< Copy of Velo banks
    inline const std::string Tracker = "Tracker/RawEvent"; ///< Copy of IT, OT and TT banks
    inline const std::string HC      = "HC/RawEvent";      ///< Copy of Herschel banks
    inline const std::string Unstripped =
        "Unstripped/RawEvent"; ///< Miscellaneous banks not required by stripping lines.
    inline const std::string VeloCluster = "VeloCluster/RawEvent"; ///< Copy of Velo Cluster banks
  }                                                                // namespace RawEventLocation

  /** @class LHCb::RawEvent RawEvent.h
   *
   * Raw event
   *
   * @author Helder Lopes
   * @author Markus Frank
   * created Tue Oct 04 14:45:29 2005
   *
   */

  class RawEvent : public DataObject {
  public:
    /** @class LHCb::RawEvent::Bank RawEvent.h Event/RawEvent.h
     *
     * Shadow class used to deal with persistency.
     * This class is entirely internal. Do not change.
     * In particular the field comments are hints to ROOT
     * to support the storage of variable size C-arrays in order
     * to avoid a copy of the data.
     *
     * Banks can be removed using the removeBank(RawBank*) member
     * function. The bank to be removed must be identified by its
     * pointer to ensure unambiguous bank identification also in the
     * event where multiple banks if the same bank type are present.
     * If no other bank of the category of the bank (Banktype)to
     * be removed is anymore present in the raw event, also the
     * category is removed.
     *
     * Note:
     * - The length passed to the RawEvent::createBank should NOT
     *   contain the size of the header !
     *
     * @author  M.Frank
     * @version 1.0
     */
    struct Bank final {
      int                 m_len  = 0;       // Bank length
      char                m_owns = 1;       //! transient data member: ownership flag
      const unsigned int* m_buff = nullptr; //[m_len]
      /// Default constructor
      Bank() = default;
      /// Initializing constructor
      Bank( int len, char owns, const unsigned int* b ) : m_len( len ), m_owns( owns ), m_buff( b ) {}
      /// Copy constructor
      [[deprecated( "copy ctor should not exist..." )]] Bank( const Bank& ) = default;
      Bank( Bank&& rhs ) {
        m_len  = std::exchange( rhs.m_len, 0 );
        m_owns = std::exchange( rhs.m_owns, 0 );
        m_buff = std::exchange( rhs.m_buff, nullptr );
      }
      /// Move Assignment operator
      Bank& operator=( Bank&& rhs ) {
        m_len  = std::exchange( rhs.m_len, 0 );
        m_owns = std::exchange( rhs.m_owns, 0 );
        m_buff = std::exchange( rhs.m_buff, nullptr );
        return *this;
      }
      /// Access to memory buffer
      const unsigned int* buffer() const { return m_buff; }
      /// Access to ownership flag.
      bool ownsMemory() const { return m_owns == 1; }
    };

    /// Default Constructor
    RawEvent() = default;

    /// Default Destructor
    ~RawEvent() override;

    /// Move constructor
    RawEvent( RawEvent&& ) = default;

    // Move assignemnt
    RawEvent& operator=( RawEvent&& ) = default;

    /// Retrieve class identifier (static)
    static const CLID& classID() { return CLID_RawEvent; }
    /// Retrieve class identifier (virtual overload)
    const CLID& clID() const override { return RawEvent::classID(); }

    /// accessor method to the vector of Raw banks for a given bank type
    LHCb::span<const RawBank*> banks( RawBank::BankType bankType ) const {
      // The optimizer should be able to deal with this...
      return m_mapped ? m_eventMap[bankType] : mapBanks( bankType );
    }

    /// allows to reserve space for future banks
    void reserve( unsigned int n ) {
      m_banks.reserve( n );
      m_eventMap.resize( RawBank::LastType );
    }

    /// returns size of the RawEvent, aka number of banks it contains
    unsigned int size() { return m_banks.size(); }

    /// For offline use only: copy data into a set of banks, adding bank header internally.
    void addBank( int sourceID, RawBank::BankType bankType, int version, LHCb::span<const std::byte> data ) {
      adoptBank( createBank( sourceID, bankType, version, data ), true );
    }

    template <typename ValueType, typename = std::enable_if_t<!std::is_convertible_v<ValueType, std::byte>>>
    void addBank( int sourceID, RawBank::BankType bankType, int version, LHCb::span<ValueType> data ) {
      addBank( sourceID, bankType, version, as_bytes( data ) );
    }

    template <typename ValueType>
    void addBank( int sourceID, RawBank::BankType bankType, int version, const std::vector<ValueType>& data ) {
      addBank( sourceID, bankType, version, LHCb::make_span( data ) );
    }

    /// For offline use only: copy data into a bank, adding bank header internally.
    void addBank( const RawBank* data ); // Pointer to data block (payload) of bank

    /// Take ownership of a bank, including the header
    void adoptBank( const RawBank* bank, // Pointer to beginning of bank (i.e. bank header)
                    bool           adopt_memory ); // Flag to adopt memory

    /// Remove bank identified by its pointer
    /** Remove raw data bank from bankset and update bank map.
     * The bank removal can fail if the specified bank was not found.
     *
     *  @param bank          [IN]      Pointer to raw bank structure to be removed.
     *
     *  @return Boolean value indicating success (=true) or failure(=false)
     */
    bool removeBank( const RawBank* bank );

    /// Rawbank creator
    /** Create raw bank and fill values
     *  @param srcID          [IN]     Source identifier
     *  @param typ            [IN]     Bank type (from RawBank::BankType enum)
     *  @param vsn            [IN]     Bank version
     *  @param len            [IN]     Length of data segment in bytes
     *  @param data           [IN]     Data buffer (if NULL, no data are copied)
     *
     *  @return Initialized Pointer to RawBank structure
     */
    static RawBank* createBank( int srcID, RawBank::BankType typ, int vsn, size_t len, const void* data = nullptr );

    template <typename ValueType>
    static RawBank* createBank( int srcID, RawBank::BankType typ, int vsn, LHCb::span<ValueType> data ) {
      return createBank( srcID, typ, vsn, data.size_bytes(), data.data() );
    }

    /// Access the full length 32 bit aligned length of a bank in bytes
    /** Access full bank length
     * @param  len           [IN]     Raw unaligned bank length
     *
     * @return padded bank size in bytes
     */
    static size_t paddedBankLength( size_t len );

  private:
    /// Map banks on first request
    /** @param bankType        [IN]     type of banks to be returned (from RawBank::BankType enum)
     *
     * @return vector of mapped banks corresponding to bankType
     */
    LHCb::span<const RawBank*> mapBanks( RawBank::BankType bankType ) const;

    mutable std::vector<boost::container::static_vector<const RawBank*, 350>> m_eventMap; //! transient Map with
                                                                                          //! RawBanks (values) for each
                                                                                          //! bank type
    std::vector<Bank> m_banks;          // Vector with persistent bank structure
    mutable bool      m_mapped = false; //! transient
  };                                    // class RawEvent
} // namespace LHCb

#endif /// DAQEVENT_RAWEVENT_H
