/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include <DetDesc/ConditionDerivation.h>

#include <DetDesc/Condition.h>
#include <DetDesc/ParamValidDataObject.h>
#include <GaudiKernel/GaudiException.h>
#include <GaudiKernel/IDataProviderSvc.h>
#include <GaudiKernel/IUpdateManagerSvc.h>

#include <memory>

namespace LHCb::DetDesc {
  ConditionDerivation::ConditionDerivation( LHCb::span<const ConditionKey> inputs, ConditionKey output,
                                            ConditionCallbackFunction func )
      : m_func{std::move( func )}, m_outputKey{std::move( output )} {
    for ( const auto& k : inputs ) m_condContext[k] = nullptr;
  }

  void ConditionDerivation::registerDerivation( IUpdateManagerSvc* ums, IDataProviderSvc* dds ) {
    for ( auto& [in_path, input] : m_condContext ) {

      DataObject* obj = nullptr;
      auto        sc  = dds->retrieveObject( in_path, obj );
      if ( !sc ) throw GaudiException( "failed to retrieve " + in_path, "ConditionDerivation", sc );
      input = dynamic_cast<ValidDataObject*>( obj );
      if ( !input ) throw GaudiException( "wrong type for " + in_path, "ConditionDerivation", sc );

      ums->registerCondition( this, in_path, &ConditionDerivation::i_handler, input );
    }

    auto tmp = std::make_unique<Condition>();
    auto sc  = dds->registerObject( m_outputKey, tmp.get() );
    if ( !sc ) throw GaudiException( "failed to add " + m_outputKey, "ConditionDerivation", sc );
    m_output = tmp.release();
    ums->registerCondition( m_output, this );
  }

  void ConditionDerivation::unregisterDerivation( IUpdateManagerSvc* ums ) { ums->unregister( this ); }

  StatusCode ConditionDerivation::i_handler() {
    m_func( m_outputKey, m_condContext, *m_output );
    return StatusCode::SUCCESS;
  }
} // namespace LHCb::DetDesc
