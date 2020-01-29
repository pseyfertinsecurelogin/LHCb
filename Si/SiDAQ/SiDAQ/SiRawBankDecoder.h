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
#pragma once

#include <boost/container/small_vector.hpp>

#include "Kernel/STLExtensions.h"
#include "SiDAQ/SiADCBankTraits.h"
#include "SiDAQ/SiADCWord.h"
#include "SiDAQ/SiHeaderWord.h"
#include "SiDAQ/SiRawBufferWord.h"

// forward declaration needed for friendship with iterators
template <class CLUSTERWORD>
class SiRawBankDecoder;

/** @class SiRawBankDecoder SiRawBankDecoder.h
 *
 *  Decoder for raw Si banks
 *
 *  This class provides two ways to decode the velo raw buffer.
 *  Either you can iterate over the cluster positions only,
 *  using the pos_iterator,
 *  or you can iterate over cluster positions bundled with
 *  the ADC values of the strips contributing to the cluster.
 *  The latter method uses more CPU and memory.
 *  So only use it when you really need the ADCs.
 *  Otherwise (e.g. in the trigger) stick to the first method.
 *
 *  One instance of this class can decode exactly one bank, the
 *  one provided in the constructor argument.
 *
 *  This class is intentionally as independent from the LHCb/Gaudi
 *  framework as possible (it only uses some classes in Si/SiDAQ
 *  SiDAQ which are in turn independent of the framework).
 *  This is because it will be used in standalone testing of Velo DAQ.
 *  Don't break it.
 *
 *  @see SiCLusterWord
 *  @see SiHeaderWord
 *  @see SiADCWord
 *
 *  @author Kurt Rinnert
 *  @date   2006-02-22
 */
template <class CLUSTERWORD>
class SiRawBankDecoder final {
public:
  // live and death

  /** Constructor
   *  Takes constant pointer to beginning of data content
   *  of a raw Si bank.  The Constructor is non trrivial,
   *  it will imediately decode the bank header bytes.  So
   *  if you don't need it, don't create it.
   */
  explicit SiRawBankDecoder( const SiDAQ::buffer_word* bank )
      : m_bank( bank ), m_header( SiHeaderWord( bank[0] ) ), m_nClusters( m_header.nClusters() ) {}

  // shortcuts

  /** @class SiRawBankDecoder::SiDecodedCluster SiRawBankDecoder.h
   *
   *  Simple type wrapper
   *
   *  This is just a simple type wrapper for a pair of a
   *  SiClusterWord and a vector of corresponding SiADCWords.
   *
   *  @see SiClusterWord
   *  @see SiADCWord
   *
   *  @author Kurt Rinnert
   *  @date   2006-02-22
   */
  using SiDecodedCluster = std::pair<CLUSTERWORD, boost::container::small_vector<SiADCWord, 10>>;

  // decoding iterators

  /** @class SiRawBankDecoder::pos_range SiRawBankDecoder.h
   *
   *  Decoding range for cluster positions only
   *
   *  This completly trivial range allows you to traverse the cluster
   *  positions in the raw bank and decode them at the same
   *  time.  Use this if you do not need the ADC counts and
   *  speed is a concern (e.g. in the trigger).
   *
   *  @see SiClusterWord
   *
   *  @author Kurt Rinnert
   *  @date   2006-02-22
   */

  class pos_range final {

    const LHCb::span<const uint16_t> m_bank;

    struct Sentinel final {};

    class Iterator final {
      const LHCb::span<const uint16_t> m_bank;
      unsigned int                     m_pos{0};

    public:
      Iterator( LHCb::span<const uint16_t> bank ) : m_bank{bank} {}

      // dereferencing
      CLUSTERWORD operator*() const { return CLUSTERWORD{m_bank[m_pos]}; }

      Iterator& operator++() {
        ++m_pos;
        assert( m_pos <= m_bank.size() );
        return *this;
      }

      bool operator!=( Sentinel ) const { return m_pos != m_bank.size(); }
    };

  public:
    pos_range( LHCb::span<const uint16_t> bank ) : m_bank{std::move( bank )} {}
    auto begin() const { return Iterator{m_bank}; }
    auto end() const { return Sentinel{}; }
  };

  /** @class SiRawBankDecoder::Iterator SiRawBankDecoder.h
   *
   *  Decoding iterator for cluster positions only
   *
   *  This smart iterator allows you to traverse a the cluster
   *  positions and ADC counts in the raw bank and decode them at the same
   *  time.  Use this only if you really need the ADC counts an speed
   *  is not a concern. This iterator also keeps track of the number of
   *  32 bit words read from the bank, useful to compare to the size in
   *  real bank header as accessed with RawBank::size().
   *
   *  @see RawBank
   *  @see SiDecodedCluster
   *  @see SiClusterWord
   *  @see SiADCWord
   *
   *  @author Kurt Rinnert
   *  @date   2006-02-22
   */
  class posadc_range final {
    const SiRawBankDecoder* m_decoder;
    struct Sentinel         final {};
    class Iterator          final {
    public:
      /**  Increment
       *   The implementatio of this increment operator
       *   is non-trivial.  It reads the next cluster
       *   position word from the raw bank and decodes it.
       *
       */
      Iterator& operator++() {
        if ( *this != Sentinel{} ) {
          ++m_pos;
          decode();
        }
        return *this;
      }

      bool operator!=( Sentinel ) const { return m_pos != m_nClusters; }

      // dereferencing

      /**  dereference to reference to SiDecodedCluster instance
       *   Gives access to the decoded cluster.  Note that the return value
       *   is a const reference.  The object it refers to does no longer
       *   exist once the iterator goes out of scope.  So if you need to hold
       *   a copy longer than that you have to initialise an instance.
       *   I am well aware that this is a somewhat dangerous behaviour.  However,
       *   this is performance critical code.  So please simply refrain from
       *   creating instances and be aware of the lifetime of the returned
       *   reference.
       *
       * @see SiDecodedCluster
       */
      const SiDecodedCluster& operator*() const { return m_cluster; }

      /**  dereference to pointer
       *   Gives access to pointer to decoded cluster
       *
       * @see SiDecodedCluster
       */
      const SiDecodedCluster* operator->() const { return &m_cluster; }

      /** Number of bytes read
       *  Returns the number of bytes (of 8 bit size) read by this
       *  iterator so far.  The purpose is to compare this to
       *  RawBank::size() after reading the whole bank as a
       *  consistency check.
       *  For a newly constructed iterator that did not yet
       *  read anything the returned number is 4, corresponding to the number
       *  of bytes in the bank header.
       *  After reading the first cluster it always includes
       *  the number of padding bytes between the cluster position and
       *  ADC part of the bank.  This means this method can return the
       *  actual number of bytes read or this number plus two, depending
       *  on whether the number of clusters in the bank is even or odd.
       *
       *  @see RawBank
       */
      int bytesRead() const { return static_cast<int>( 4 + ( m_pos + m_pos % 2 ) * 2 + m_nADC ); }

      /**  Construct with position in raw bank and reference to decoder
       *   Only friends (i.e. the decoder class) are allowed
       *   to do that.  If applicable, the position is decoded.
       *
       * @see SiRawBankDecoder
       */
      Iterator( const SiRawBankDecoder* decoder )
          : m_nClusters{decoder->m_nClusters}
          , m_offset( 4 + ( decoder->m_nClusters + decoder->m_nClusters % 2 ) * 2 )
          , m_bank( decoder->m_bank ) {
        decode();
      }

    private:
      /** Decode cluster position and ADC counts
       *  This method wraps the call to the bank type specific
       *  doDecode() methods.
       *
       * @see SiDecodedCluster
       */
      void decode() {
        if ( m_pos < m_nClusters ) doDecode( typename CLUSTERWORD::adc_bank_type() );
      }

      /** Decoder for ADC bank with ADC counts only
       *  The actual decoding for ADC only banks like
       *  specified for the Velo
       */
      void doDecode( SiDAQ::adc_only_bank_tag ) {
        m_cluster.second.clear();
        doDecodeCommon( m_cluster );
      }

      /** Decoder for ADC bank with neighboursum
       *  The actual decoding for ADC banks with prepended
       *  neighbour sums like specified for the ST
       */
      void doDecode( SiDAQ::adc_neighboursum_bank_tag ) {
        m_cluster.second.clear();

        // fetch the neighbour sum first
        m_cluster.second.emplace_back( adcWord( m_nADC ) );
        ++m_nADC;

        doDecodeCommon( m_cluster );
      }

      /** Decoding common to all ADC bank types
       *  The part of the decoding for ADC banks which
       *  independent of the specific ADC bank type, i.e.
       *  the common between Velo and ST
       */
      void doDecodeCommon( SiDecodedCluster& cluster ) {
        // get the first adc count *without* checking the end-of-cluster bit
        cluster.second.emplace_back( adcWord( m_nADC ) );
        ++m_nADC;

        // only move on if the end-of-cluster bit is not set
        while ( !cluster.second.back().endCluster() ) {
          cluster.second.emplace_back( adcWord( m_nADC ) );
          ++m_nADC;
        }

        // get cluster position
        cluster.first = clusterWord( m_pos );
      }

    private:
      uint8_t     adcWord( unsigned int n ) const { return reinterpret_cast<const uint8_t*>( m_bank )[m_offset + n]; }
      CLUSTERWORD clusterWord( unsigned int n ) const {
        return CLUSTERWORD{reinterpret_cast<const uint16_t*>( m_bank )[2 + n]};
      }

      unsigned int              m_pos = 0;
      const unsigned int        m_nClusters;
      unsigned int              m_nADC = 0;
      const unsigned int        m_offset;
      const SiDAQ::buffer_word* m_bank;
      SiDecodedCluster          m_cluster;
    };

  public:
    posadc_range( const SiRawBankDecoder* decoder ) : m_decoder{decoder} {}
    auto begin() const { return Iterator{m_decoder}; }
    auto end() const { return Sentinel{}; }
  };

  // accessors

  /** Access decoded bank header
   *
   * @see SiHeaderWord
   */
  const SiHeaderWord& header() const { return m_header; }

  /** Shortcut to error condition
   *  Simply parrots the error flag from the header word
   *
   * @see SiHeaderWord
   */
  bool hasError() const { return m_header.hasError(); }

  /** Access number of clusters
   *  Yields the total number of clusters encoded
   *  in the raw bank.
   *
   * @return number of clusters in raw bank
   */
  unsigned int nClusters() const { return m_nClusters; }

  /// range of cluster positions
  auto posRange() const { return pos_range{{reinterpret_cast<const uint16_t*>( m_bank ) + 2, m_nClusters}}; }
  [[deprecated( "please use posRange() instead" )]] auto posBegin() const { return posRange().begin(); }
  [[deprecated( "please use posRange() instead" )]] auto posEnd() const { return posRange().end(); }

  /// range of clusters with ADC values
  auto posAdcRange() const { return posadc_range( this ); }
  auto posAdcBegin() const { return posAdcRange().begin(); }
  auto posAdcEnd() const { return posAdcRange().end(); }

private:
  const SiDAQ::buffer_word* m_bank;
  SiHeaderWord              m_header;
  unsigned int              m_nClusters;
};
