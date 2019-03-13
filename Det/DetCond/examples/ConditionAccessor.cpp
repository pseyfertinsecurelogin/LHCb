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
#include <DetDesc/Condition.h>
#include <DetDesc/ConditionAccessorHolder.h>
#include <DetDesc/ConditionTransformation.h>
#include <Gaudi/Algorithm.h>
#include <GaudiKernel/IUpdateManagerSvc.h>

#include <cmath>

namespace DetCond::Examples {
  struct CondAccessExample : LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm> {
    // inherit base class costructor
    using base_class::base_class;

    ConditionAccessor<Condition> m_cond{this, "CondPath", "TestCondition"};

    StatusCode execute( const EventContext& ctx ) const override {
      const auto& cctx = getConditionContext( ctx );
      const auto& cond = m_cond.get( cctx );
      info() << "condition value: " << cond << endmsg;
      return StatusCode::SUCCESS;
    }
  };

  struct CondAccessExampleWithDerivation : LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm> {
    // inherit base class costructor
    using base_class::base_class;

    struct MyData {
      double p1, p2, v;
    };

    Gaudi::Property<std::string> m_srcPath{this, "Source", "TestCondition"};
    ConditionAccessor<MyData>    m_cond{this, "Target", "DerivedCondition"};

    struct MyConditionTransformation : LHCb::DetDesc::ConditionTransformation {
      MyConditionTransformation( ConditionKey input, ConditionKey output )
          : LHCb::DetDesc::ConditionTransformation( {input}, std::move( output ) ) {
        m_inKey = std::move( input );
      }
      void operator()( const ConditionKey& /* target */, ConditionUpdateContext& ctx,
                       Condition& output ) const override {
        const auto cond = ctx[m_inKey];
        const auto p1   = cond->param<double>( "par1" );
        const auto p2   = cond->param<double>( "par2" );
        output.payload  = make_cond( p1, p2 );
      }

      static MyData make_cond( double p1, double p2 ) { return MyData{p1, p2, std::sqrt( p1 ) + 2.0 * p2 * p2}; }

      ConditionKey m_inKey;
    };

    std::unique_ptr<MyConditionTransformation> m_transformer;

    StatusCode initialize() override {
      const auto sc = LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm>::initialize();
      if ( !sc ) return sc;

      m_transformer = std::make_unique<MyConditionTransformation>( m_srcPath, m_cond.key() );
      m_transformer->registerTransformation( service<IUpdateManagerSvc>( "UpdateManagerSvc" ), detSvc() );

      return sc;
    }

    StatusCode execute( const EventContext& ctx ) const override {
      const auto& cctx = getConditionContext( ctx );
      const auto& cond = m_cond.get( cctx );
      info() << "condition value: {\n  p1: " << cond.p1 << "\n  p2: " << cond.p2 << "\n  v:  " << cond.v << "\n}"
             << endmsg;
      return StatusCode::SUCCESS;
    }
  };
} // namespace DetCond::Examples

DECLARE_COMPONENT( DetCond::Examples::CondAccessExample )
DECLARE_COMPONENT( DetCond::Examples::CondAccessExampleWithDerivation )
