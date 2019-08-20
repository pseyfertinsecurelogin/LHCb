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
#include "GaudiAlg/Consumer.h"
#include <DetDesc/Condition.h>
#include <DetDesc/ConditionAccessorHolder.h>
#include <Gaudi/Algorithm.h>
#include <type_traits>

namespace DetCond::Examples::Functional {

  // Example of algorithm accessing conditions
  struct CondAccessExample
      : Gaudi::Functional::Consumer<void( const Condition&, const int& ), LHCb::DetDesc::usesConditions<Condition>> {
    // constructor
    CondAccessExample( const std::string& name, ISvcLocator* loc )
        : Consumer{name, loc, {KeyValue{"CondPath", "TestCondition"}, KeyValue{"IntPath", "IntFromTES"}}} {}

    void operator()( const Condition& cond, const int& ) const override {
      info() << "condition value: " << cond << endmsg;
    }
  };

  // Example of algorithm that requires caching of condition derivations.

  /// Example of a derived condition that does not inherit from Condition base class.
  struct MyData {
    double p1, p2, v;
  };

  struct CondAccessExampleWithDerivation
      : Gaudi::Functional::Consumer<void( const MyData& ), LHCb::DetDesc::usesConditions<MyData>> {
    // costructor
    CondAccessExampleWithDerivation( const std::string& name, ISvcLocator* loc )
        : Consumer{name, loc, {KeyValue{"Target", "DerivedCondition"}}} {}

    /// Property used to change the path of the input condition, if needed
    Gaudi::Property<std::string> m_srcPath{this, "Source", "TestCondition"};

    StatusCode initialize() override {
      const auto sc = Consumer::initialize();
      if ( !sc ) return sc;

      addConditionDerivation( m_srcPath.value(), inputLocation<0>(),
                              /// Callable which converts raw information into a derived condition.
                              []( const ParamValidDataObject& obj ) -> MyData {
                                const auto& [p1, p2] = obj.params<double, double>( "par1", "par2" );
                                return {p1, p2, std::sqrt( p1 ) + 2.0 * p2 * p2};
                              } );

      return sc;
    }

    void operator()( const MyData& cond ) const override {
      info() << "condition value: {\n  p1: " << cond.p1 << "\n  p2: " << cond.p2 << "\n  v:  " << cond.v << "\n}"
             << endmsg;
    }
  };

} // namespace DetCond::Examples::Functional

DECLARE_COMPONENT( DetCond::Examples::Functional::CondAccessExample )
DECLARE_COMPONENT( DetCond::Examples::Functional::CondAccessExampleWithDerivation )
