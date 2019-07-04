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
#include "Event/RawEvent.h"
#include <cstring> // for memcpy with gcc 4.3

namespace {
  inline LHCb::RawBank* allocateBank( size_t len ) {
    size_t        mem_len = LHCb::RawEvent::paddedBankLength( len );
    size_t        new_len = mem_len / sizeof( unsigned int );
    unsigned int* mem     = new unsigned int[new_len];
    if ( mem_len != len ) mem[new_len - 1] = 0;
    return reinterpret_cast<LHCb::RawBank*>( mem );
  }
} // namespace

// Default Destructor
LHCb::RawEvent::~RawEvent() {
  for ( Bank& b : m_banks ) {
    if ( b.ownsMemory() ) delete[] b.buffer();
  }
}

size_t LHCb::RawEvent::paddedBankLength( size_t len ) {
  size_t mem_len = len + sizeof( LHCb::RawBank ) - sizeof( unsigned int );
  if ( mem_len % sizeof( unsigned int ) ) { // Need padding
    mem_len = ( mem_len / sizeof( unsigned int ) + 1 ) * sizeof( unsigned int );
  }
  return mem_len;
}

LHCb::span<const LHCb::RawBank*> LHCb::RawEvent::mapBanks( RawBank::BankType bankType ) const {
  m_eventMap.resize( RawBank::LastType );
  for ( auto& i : m_banks ) {
    auto* bank = reinterpret_cast<const LHCb::RawBank*>( i.buffer() );
    m_eventMap[bank->type()].push_back( const_cast<LHCb::RawBank*>( bank ) );
  }
  m_mapped = true;
  return m_eventMap[bankType];
}

// For offline use only: copy data into a set of banks, adding bank header internally.
void LHCb::RawEvent::addBank( int srcID, LHCb::RawBank::BankType typ, int vsn, const std::vector<unsigned int>& data ) {
  adoptBank( createBank( srcID, typ, vsn, data.size() * sizeof( unsigned int ), data.data() ), true );
}

LHCb::RawBank* LHCb::RawEvent::createBank( int srcID, LHCb::RawBank::BankType typ, int vsn, size_t len,
                                           const void* data ) {
  LHCb::RawBank* bank = allocateBank( len );
  bank->setMagic();
  bank->setType( typ );
  bank->setVersion( vsn );
  bank->setSourceID( srcID );
  bank->setSize( len );
  if ( data ) std::memcpy( bank->data(), data, len );
  return bank;
}

/// For offline use only: copy data into a bank, adding bank header internally.
// TODO: remove all the calls to this function (in deepCopyRawEvent) and remove it
void LHCb::RawEvent::addBank( const RawBank* data ) {
  size_t len  = data->totalSize();
  auto   bank = reinterpret_cast<LHCb::RawBank*>( new unsigned int[len / sizeof( unsigned int )] );
  std::memcpy( bank, data, len );
  adoptBank( bank, true );
}

/// Take ownership of a bank, including the header
void LHCb::RawEvent::adoptBank( const LHCb::RawBank* bank, bool adopt_memory ) {
  size_t len = bank->totalSize();
  if ( !m_mapped ) mapBanks( bank->type() );
  m_eventMap[bank->type()].push_back( bank );
  m_banks.emplace_back( len / sizeof( unsigned int ), adopt_memory, reinterpret_cast<const unsigned int*>( bank ) );
}

/// Remove bank identified by its pointer
bool LHCb::RawEvent::removeBank( const RawBank* bank ) {
  auto& banks = m_eventMap[bank->type()];
  auto  j     = std::find( banks.begin(), banks.end(), bank );
  if ( j == banks.end() ) return false;
  // First remove bank from persistent array.
  auto k = std::find_if(
      m_banks.begin(), m_banks.end(),
      [ptr = reinterpret_cast<const unsigned int*>( bank )]( const Bank& b ) { return ptr == b.buffer(); } );
  if ( k != m_banks.end() ) {
    // The bank is owned by RawEvent: delete the allocated buffer
    // to prevent memory leak when reading data from a ROOT file...
    if ( k->ownsMemory() ) delete[] k->buffer();
    m_banks.erase( k );
  }
  // Now erase bank from vector with all banks of one type.
  banks.erase( j );
  return true;
}
