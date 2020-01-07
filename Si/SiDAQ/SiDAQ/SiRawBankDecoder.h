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
  SiRawBankDecoder( const SiDAQ::buffer_word* bank )
      : m_bank( bank ), m_header( SiHeaderWord( bank[0] ) ), m_nClusters( m_header.nClusters() ) {}

  /// Copy construction
  SiRawBankDecoder( const SiRawBankDecoder& ini )
      : m_bank( ini.m_bank ), m_header( ini.m_header ), m_nClusters( ini.m_nClusters ) {}

  /// Assignment
  const SiRawBankDecoder& operator=( const SiRawBankDecoder& rhs ) {
    m_bank      = rhs.m_bank;
    m_header    = rhs.m_header;
    m_nClusters = rhs.m_nClusters;
    return *this;
  }

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
  typedef std::pair<CLUSTERWORD, boost::container::small_vector<SiADCWord, 10>> SiDecodedCluster;

  // decoding iterators

  /** @class SiRawBankDecoder::pos_iterator SiRawBankDecoder.h
   *
   *  Decoding iterator for cluster positions only
   *
   *  This completly trivial iterator allows you to traverse the cluster
   *  positions in the raw bank and decode them at the same
   *  time.  Use this if you do not need the ADC counts and
   *  speed is a concern (e.g. in the trigger).
   *
   *  @see SiClusterWord
   *
   *  @author Kurt Rinnert
   *  @date   2006-02-22
   */
  class pos_iterator final {
  public:
    // live and death

    pos_iterator() = default;

    const pos_iterator& operator++() const {
      ++m_pos;
      return *this;
    }

    /** Equal
     *  True if positions match
     */
    friend bool operator==( const pos_iterator& lhs, const pos_iterator& rhs ) { return lhs.m_pos == rhs.m_pos; }

    /** Not Equal
     *  True if positions do not match
     */
    friend bool operator!=( const pos_iterator& lhs, const pos_iterator& rhs ) { return lhs.m_pos != rhs.m_pos; }

    // dereferencing
    const CLUSTERWORD operator*() const { return CLUSTERWORD( m_bank[m_pos] ); }

  private:
    /**  Construct with position in raw bank reference to decoder
     *   Only friends (i.e. the decoder class) are allowed
     *   to do that.  If applicable, the position will be
     *   decoded.
     *
     * @see SiRawBankDecoder
     */
    pos_iterator( unsigned int pos, const SiRawBankDecoder* decoder )
        : m_pos( pos ), m_bank( &( (uint16_t*)decoder->m_bank )[2] ) {}

  private:
    mutable unsigned int m_pos;
    const uint16_t*      m_bank;

    friend class SiRawBankDecoder;
  };

  friend class pos_iterator;

  /** @class SiRawBankDecoder::posadc_iterator SiRawBankDecoder.h
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
  class posadc_iterator final {
  public:
    // live and death

    posadc_iterator() = default;

    /**  Increment
     *   The implementatio of this increment operator
     *   is non-trivial.  It reads the next cluster
     *   position word from the raw bank and decodes it.
     *
     */
    const posadc_iterator& operator++() const {
      if ( *this != m_decoder->posAdcEnd() ) {
        ++m_pos;
        decode();
      }
      return *this;
    }

    /** Equal
     *  True if positions match
     */
    friend bool operator==( const posadc_iterator& lhs, const posadc_iterator& rhs ) { return lhs.m_pos == rhs.m_pos; }

    /** Not Equal
     *  True if positions do not match
     */
    friend bool operator!=( const posadc_iterator& lhs, const posadc_iterator& rhs ) { return lhs.m_pos != rhs.m_pos; }

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

  private:
    /**  Construct with position in raw bank and reference to decoder
     *   Only friends (i.e. the decoder class) are allowed
     *   to do that.  If applicable, the position is decoded.
     *
     * @see SiRawBankDecoder
     */
    posadc_iterator( unsigned int pos, const SiRawBankDecoder* decoder )
        : m_pos( pos ), m_offset( 4 + ( decoder->m_nClusters + decoder->m_nClusters % 2 ) * 2 ), m_decoder( decoder ) {
      decode();
    }

    /** Decode cluster position and ADC counts
     *  This method wraps the call to the bank type specific
     *  doDecode() methods.
     *
     * @see SiDecodedCluster
     */
    void decode() const {
      if ( m_pos < m_decoder->m_nClusters ) doDecode( typename CLUSTERWORD::adc_bank_type() );
    }

    /** Decoder for ADC bank with ADC counts only
     *  The actual decoding for ADC only banks like
     *  specified for the Velo
     */
    void doDecode( SiDAQ::adc_only_bank_tag ) const {
      m_cluster.second.clear();
      doDecodeCommon( m_cluster );
    }

    /** Decoder for ADC bank with neighboursum
     *  The actual decoding for ADC banks with prepended
     *  neighbour sums like specified for the ST
     */
    void doDecode( SiDAQ::adc_neighboursum_bank_tag ) const {
      m_cluster.second.clear();

      // fetch the neighbour sum first
      m_cluster.second.emplace_back( ( (uint8_t*)m_decoder->m_bank )[m_offset + m_nADC] );
      ++m_nADC;

      doDecodeCommon( m_cluster );
    }

    /** Decoding common to all ADC bank types
     *  The part of the decoding for ADC banks which
     *  independent of the specific ADC bank type, i.e.
     *  the common between Velo and ST
     */
    void doDecodeCommon( SiDecodedCluster& cluster ) const {
      // get the first adc count *without* checking the end-of-cluster bit
      cluster.second.emplace_back( ( (uint8_t*)m_decoder->m_bank )[m_offset + m_nADC] );
      ++m_nADC;

      // only move on if the end-of-cluster bit is not set
      while ( !cluster.second.back().endCluster() ) {
        cluster.second.emplace_back( ( (uint8_t*)m_decoder->m_bank )[m_offset + m_nADC] );
        ++m_nADC;
      }

      // get cluster position
      cluster.first = CLUSTERWORD( ( (uint16_t*)m_decoder->m_bank )[2 + m_pos] );
    }

  private:
    mutable unsigned int     m_pos  = 0;
    mutable unsigned int     m_nADC = 0;
    const unsigned int       m_offset;
    const SiRawBankDecoder*  m_decoder;
    mutable SiDecodedCluster m_cluster;

    friend class SiRawBankDecoder;
  };
  friend class posadc_iterator;

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

  /// start iterator for cluster positions
  const pos_iterator posBegin() const { return pos_iterator( 0, this ); }

  /// end iterator for cluster positions
  const pos_iterator posEnd() const { return pos_iterator( m_nClusters, this ); }

  /// start iterator for decoded clusters with ADC values
  const posadc_iterator posAdcBegin() const { return posadc_iterator( 0, this ); }

  /// end iterator for decoded clusters with ADC values
  const posadc_iterator posAdcEnd() const { return posadc_iterator( m_nClusters, this ); }

private:
  const SiDAQ::buffer_word* m_bank;
  SiHeaderWord              m_header;
  unsigned int              m_nClusters;
};
