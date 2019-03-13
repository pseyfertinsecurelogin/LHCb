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
  /** Example of algorithm accessing conditions via ConditionAccessorHolder.
   */
  struct CondAccessExample : LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm> {
    // inherit base class costructor
    using base_class::base_class;

    // ConditionAccessor data members are similar to properties:
    ConditionAccessor<Condition> m_cond{/* required for automatic declaration */ this,
                                        /* name of the property */ "CondPath",
                                        /* default value */ "TestCondition"};

    StatusCode execute( const EventContext& ctx ) const override {
      // When not using Gaudi::Functional, we have to get explicitly the condition
      // for the current event, by getting the current conditions context...
      const auto& cctx = getConditionContext( ctx );
      // ... and extracting the condition from it.
      const auto& cond = m_cond.get( cctx );
      // Then we can use the condition.
      info() << "condition value: " << cond << endmsg;

      return StatusCode::SUCCESS;
    }
  };

  /** Example of algorithm that requires caching of condition derivations.
   */
  struct CondAccessExampleWithDerivation : LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm> {
    // inherit base class costructor
    using base_class::base_class;

    /// Example of a derived condition that does not inherit from Condition base class.
    struct MyData {
      double p1, p2, v;
    };

    /// Class holding the code to transform a special input condition to the derived one.
    struct MyConditionTransformation : LHCb::DetDesc::ConditionTransformation {
      // in this case we only take one input condition, but the base class can
      // handle multiple inputs
      MyConditionTransformation( ConditionKey input, ConditionKey output )
          : LHCb::DetDesc::ConditionTransformation( {input}, std::move( output ) ) {
        m_inKey = std::move( input );
      }
      /// Method to produce the derived condition.
      // The first argument is not needed in this case, because we do not have
      // different code depending on the output key.
      // The actual work can be done by an external call (make_cond in this example)
      // that can live in another library, to be shared with other conditon derivation
      // backends (e.g. DD4hep).
      void operator()( const ConditionKey& /* target */, ConditionUpdateContext& ctx,
                       Condition& output ) const override {
        // get the input condition from the context
        const auto cond = ctx[m_inKey];
        // use the condition object to compute parameter and fill the output condition
        const auto p1  = cond->param<double>( "par1" );
        const auto p2  = cond->param<double>( "par2" );
        output.payload = make_cond( p1, p2 );
      }

      /// Function converting raw information into a derived condition.
      // In this example it's a static member of the MyConditionTransformation
      // wrapper, but it can live in another library, to be shared with other
      // conditon derivation backends (e.g. DD4hep).
      static MyData make_cond( double p1, double p2 ) { return MyData{p1, p2, std::sqrt( p1 ) + 2.0 * p2 * p2}; }

      /// Copy of the input ConditionKey needed to extract the input condition
      /// from ConditionUpdateContext.
      ConditionKey m_inKey;
    };

    /// Property used to change the path of the input condition, if needed
    Gaudi::Property<std::string> m_srcPath{this, "Source", "TestCondition"};

    /// Accessor to the derived condition (note that we do not need an accessor
    /// to the input conditions).
    ConditionAccessor<MyData> m_cond{this, "Target", "DerivedCondition"};

    /// Somebody has to own the transformation object.
    std::unique_ptr<MyConditionTransformation> m_transformer;

    StatusCode initialize() override {
      const auto sc = LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm>::initialize();
      if ( !sc ) return sc;

      // create a transformation object initializing it with source and target ConditionKey
      m_transformer = std::make_unique<MyConditionTransformation>( m_srcPath, m_cond.key() );
      // attach the transformation object to the UpdateManagerSvc
      m_transformer->registerTransformation( service<IUpdateManagerSvc>( "UpdateManagerSvc" ), detSvc() );

      return sc;
    }

    StatusCode execute( const EventContext& ctx ) const override {
      // during execute, we just access the derived condition as in CondAccessExample
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
