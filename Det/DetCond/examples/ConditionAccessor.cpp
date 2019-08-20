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
#include <DetDesc/IConditionDerivationMgr.h>
#include <Gaudi/Algorithm.h>

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

    /// Property used to change the path of the input condition, if needed
    Gaudi::Property<std::string> m_srcPath{this, "Source", "TestCondition"};

    /// Accessor to the derived condition (note that we do not need an accessor
    /// to the input conditions).
    ConditionAccessor<MyData> m_cond{this, "Target", "DerivedCondition"};

    StatusCode initialize() override {
      const auto sc = LHCb::DetDesc::ConditionAccessorHolder<Gaudi::Algorithm>::initialize();
      if ( !sc ) return sc;

      addConditionDerivation( m_srcPath.value(), m_cond.key(),
                              /// Callable which converts the object at m_srcPath.value() into a derived
                              /// condition, of type MyData which will be stored at m_cond.key()
                              []( const ParamValidDataObject& obj ) {
                                const auto& [p1, p2] = obj.params<double, double>( "par1", "par2" );
                                return MyData{p1, p2, std::sqrt( p1 ) + 2.0 * p2 * p2};
                              } );

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
