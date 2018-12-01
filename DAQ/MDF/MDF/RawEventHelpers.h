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
//  ====================================================================
//  MDFIO.h
//  --------------------------------------------------------------------
//
//  Author    : Markus Frank
//
//  ====================================================================
#ifndef MDF_RAWEVENTHELPERS_H
#define MDF_RAWEVENTHELPERS_H

// Framework include files
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/STLExtensions.h"

// C++ includes
#include <vector>

/*
 *    LHCb namespace
 */
namespace LHCb  {

  // Forward declarations
  class RawBank;
  class RawEvent;
  class MDFHeader;
  typedef std::pair<char*,int> MDFDescriptor;

  unsigned int hash32Checksum(const void* ptr, size_t len); 
  unsigned int adler32Checksum(unsigned int old, const char *buf, size_t len);

  /// Check sanity of raw bank structure
  bool checkRawBank(const RawBank* b, bool throw_exc=true,bool print_cout=false);
  /// Check consistency of raw bank sequence using magic words
  bool checkRawBanks(const char* start, const char* end, bool throw_exc=true,bool print_cout=false);
  /// Check consistency of MEP multi event fragment
  bool checkMDFRecord(const MDFHeader* h, int opt_len=~0x0, bool throw_exc=true,bool print_cout=false);

  /// Deep copy raw event structure (including baw bank memory - hence heavy)
  StatusCode deepCopyRawEvent(RawEvent* source, RawEvent*& result);
  /// Clone rawevent structure
  StatusCode cloneRawEvent(RawEvent* source, RawEvent*& result);
  /// Determine length of the sequential buffer from RawEvent object
  size_t rawEventLength(const RawEvent* evt);
  /// Determine length of the sequential buffer from vector of raw banks
  size_t rawEventLength(LHCb::span<const RawBank*> banks);
   /// Determine length of the sequential buffer from RawEvent object
  size_t rawEventLengthTAE(LHCb::span<const RawBank*> banks);
  /// Determine length of the sequential buffer from RawEvent object
  size_t rawEventLengthTAE(const RawEvent* evt);

  /// Determine number of banks from rawEvent object
  size_t numberOfBanks(const RawEvent* evt);
  /// Determine number of bank types from rawEvent object
  size_t numberOfBankTypes(const RawEvent* evt);
  /// Generate XOR Checksum
  unsigned int genChecksum(int flag, const void* ptr, size_t len);
  /// Compress opaque data buffer
  /** The algorithm applied is the ROOT compression mechanism.
    * Option "algtype" is used to specify the compression level:
    * compress = 0 objects written to this file will not be compressed.
    * compress = 1 minimal compression level but fast.
    * ....
    * compress = 9 maximal compression level but slow.
    */
  StatusCode compressBuffer(int algtype, char* tar, size_t tar_len, char* src, size_t src_len, size_t& new_len);
  /// Decompress opaque data buffer using the ROOT (de-)compression mechanism.
  StatusCode decompressBuffer(int algtype, char* tar, size_t tar_len, const char* src, size_t src_len, size_t& new_len);

  /// Copy RawEvent data from the object to sequential buffer
  StatusCode encodeRawBanks(const RawEvent* evt,char* const data, size_t len, bool skip_hdr_bank);
  /// Copy RawEvent data from bank vectors to sequential buffer
  StatusCode encodeRawBanks(LHCb::span<const RawBank*> banks, char* const data,
                            size_t size, bool skip_hdr_bank, size_t* length);
  /// Conditional decoding of raw buffer from MDF to raw event object
  StatusCode decodeRawBanks(const char* start, const char* end, RawEvent* raw, bool copy_banks);

  /// Return vector of TAE event names
  std::vector<std::string> buffersTAE(const char* start);

  /// Returns the prefix on TES according to bx number, - is previous, + is next
  std::string rootFromBxOffset(int bxOffset);

  /// Returns the offset of the TAE with respect to the central bx
  int bxOffsetTAE(const std::string& root);

  /// Access to TAE bank (if present)
  RawBank* getTAEBank(const char* start);

  /// Unpacks the buffer given by the start and end pointers, and fill the rawevent structure
  StatusCode unpackTAE(const char* start, const char* end, const std::string& loc, RawEvent* raw, bool copy_banks);

  /// Check if a given RawEvent structure belongs to a TAE event
  bool isTAERawEvent(RawEvent* raw);

}
#endif // MDF_RAWEVENTHELPERS_H
