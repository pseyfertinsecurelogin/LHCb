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
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IEventTimeDecoder.h" // Interface

/** Simple implementation of IEventTimeDecoder forwarding the calls to a list of
 *  other implementations.
 *
 *  @author Marco Clemencic
 *  @date   2010-09-23
 */

class TimeDecoderList final : public extends<GaudiTool, IEventTimeDecoder> {

public:
  /// Standard constructor
  TimeDecoderList( const std::string& type, const std::string& name, const IInterface* parent );

  /// Initialize the tool
  StatusCode initialize() override;

  // --- implementation of IEventTimeDecoder ---
  /// Loop over all the used decoders and return the first non-zero event time.
  /// @return The time of current event.
  Gaudi::Time getTime() const override;

private:
  /// Type of the list of decoder names.
  typedef std::vector<std::string> DecoderNamesType;

  /// Type of the list of decoders.
  typedef std::list<IEventTimeDecoder*> DecodersType;

  /// List of IEventTimeDecoder instance names.
  DecoderNamesType m_decoderNames;

  /// List of IEventTimeDecoder instances
  DecodersType m_decoders;
};

// Constructor
TimeDecoderList::TimeDecoderList( const std::string& type, const std::string& name, const IInterface* parent )
    : extends( type, name, parent ) {
  declareProperty( "Decoders", m_decoderNames, "List of IEventTimeDecoder tools to use" );
}

StatusCode TimeDecoderList::initialize() {
  const StatusCode sc = extends::initialize();
  if ( sc.isFailure() ) return sc;

  m_decoders.clear();
  for ( const auto& name : m_decoderNames ) {
    IEventTimeDecoder* p = tool<IEventTimeDecoder>( name, this );
    if ( p ) {
      m_decoders.push_back( p );
    } else {
      return Error( "Cannot retrieve " + name );
    }
  }

  return sc;
}

Gaudi::Time TimeDecoderList::getTime() const {
  Gaudi::Time t;
  for ( const auto& d : m_decoders ) {
    if ( ( t = d->getTime() ).ns() ) { return t; }
  }
  return Gaudi::Time::epoch();
}

DECLARE_COMPONENT( TimeDecoderList )
