/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "CondDBDQScanner.h"

#include "GaudiKernel/IAddressCreator.h"
#include "GaudiKernel/IConverter.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"

#include "DetDesc/Condition.h"

// ----------------------------------------------------------------------------
// Implementation file for class: CondDBDQScanner
//
// 04/11/2011: Marco Clemencic
// ----------------------------------------------------------------------------
DECLARE_COMPONENT( CondDBDQScanner )

namespace {
  class EvtTimeMgr {
  public:
    EvtTimeMgr( IDetDataSvc& dds ) : m_dds{dds} { m_orig_time = m_dds.eventTime(); }
    void set( const Gaudi::Time& time ) { m_dds.setEventTime( time ); }
    ~EvtTimeMgr() { m_dds.setEventTime( m_orig_time ); }

  private:
    IDetDataSvc& m_dds;
    Gaudi::Time  m_orig_time;
  };
} // namespace

IDQFilter::FlagsType CondDBDQScanner::scan( const Gaudi::Time& since, const Gaudi::Time& until ) const {
  IDQFilter::FlagsType flags;

  const std::string   par[2]  = {{"git:" + m_condPath}, {"Flags"}};
  const unsigned long ipar[1] = {0};

  SmartIF<IDetDataSvc> dds{detSvc()};
  if ( !dds ) Exception( "Invalid Detector Data Service" );
  EvtTimeMgr timeMgr{*dds};

  Gaudi::Time cursor = since;
  while ( cursor < until ) {
    timeMgr.set( cursor );
    // Retrieve the condition data
    IOpaqueAddress* addr = nullptr;
    DataObject*     obj  = nullptr;
    Condition*      cond = nullptr;
    if ( m_converter->addressCreator()
             ->createAddress( XML_StorageType, Condition::classID(), par, ipar, addr )
             .isFailure() ||
         m_converter->createObj( addr, obj ).isFailure() || m_converter->fillObjRefs( addr, obj ).isFailure() ||
         ( cond = dynamic_cast<Condition*>( obj ) ) == nullptr ) { // assignment intended
      if ( addr ) delete addr;
      if ( obj ) delete obj;
      Exception( "Conversion of Condition failed" );
      return flags; // never reached, but helps Coverity
    }

    // Merge the condition map with the collected one.
    const IDQFilter::FlagsType& condFlags = cond->param<IDQFilter::FlagsType>( "map" );
    flags.insert( condFlags.begin(), condFlags.end() );

    cursor = cond->validTill();
  }

  return flags;
}

StatusCode CondDBDQScanner::initialize() {
  StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) return sc;

  m_converter = service( m_converterName );
  if ( UNLIKELY( !m_converter.isValid() ) ) {
    error() << "Cannot get the IConverter implementation " << m_converterName << endmsg;
    return StatusCode::FAILURE;
  }

  return sc;
}

StatusCode CondDBDQScanner::finalize() {
  m_converter.reset();
  return base_class::finalize();
}
