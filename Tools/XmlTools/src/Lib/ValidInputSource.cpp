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
#include "XmlTools/ValidInputSource.h"

/// Constructor
ValidInputSource::ValidInputSource( const XMLByte* const srcDocBytes, const XMLSize_t byteCount,
                                    const XMLCh* const bufId, const bool adoptBuffer,
                                    xercesc::MemoryManager* const manager )
    : xercesc::MemBufInputSource( srcDocBytes, byteCount, bufId, adoptBuffer, manager )
    , m_since( Gaudi::Time::epoch() )
    , m_until( Gaudi::Time::max() ) {}

/// Constructor
ValidInputSource::ValidInputSource( std::string buffer, const XMLCh* const bufId,
                                    xercesc::MemoryManager* const manager )
    : xercesc::MemBufInputSource( reinterpret_cast<const XMLByte*>( buffer.data() ), buffer.size(), bufId, false,
                                  manager )
    , m_since( Gaudi::Time::epoch() )
    , m_until( Gaudi::Time::max() )
    , m_buffer( std::move( buffer ) ) // keep the buffer alive for the lifetime of 'this'
{}

bool ValidInputSource::isValid() const { return validSince() <= validTill(); }

bool ValidInputSource::isValid( const Gaudi::Time& time ) const { return validSince() <= time && time < validTill(); }

const Gaudi::Time& ValidInputSource::validSince() const { return m_since; }

const Gaudi::Time& ValidInputSource::validTill() const { return m_until; }

void ValidInputSource::setValidity( const Gaudi::Time& since, const Gaudi::Time& until ) {
  m_since = since;
  m_until = until;
}

void ValidInputSource::setValiditySince( const Gaudi::Time& since ) { m_since = since; }

void ValidInputSource::setValidityTill( const Gaudi::Time& until ) { m_until = until; }
