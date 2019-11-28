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
      : Gaudi::Functional::Consumer<void( const Condition& ), LHCb::DetDesc::usesConditions<Condition>> {
    // constructor
    CondAccessExample( const std::string& name, ISvcLocator* loc )
        : Consumer{name, loc, {KeyValue{"CondPath", "TestCondition"}}} {}

    void operator()( const Condition& cond ) const override {
      info() << "condition value: " << cond << endmsg;
    }
  };


  /// Example of a derived condition that does not inherit from Condition base class.
  struct MyData {
    double p1, p2, v;
  };

  /// Example of algorithm that requires caching of condition derivations.
  struct CondAccessExampleWithDerivation
      : Gaudi::Functional::Consumer<void( const MyData& ), LHCb::DetDesc::usesConditions<MyData>> {
    CondAccessExampleWithDerivation( const std::string& name, ISvcLocator* loc )
        : Consumer{name, loc, {KeyValue{"Target", "DerivedCondition"}}} {}

    /// Property used to change the path of the input condition, if needed
    Gaudi::Property<std::string> m_srcPath{this, "Source", "TestCondition"};

    StatusCode initialize() override {
      return Consumer::initialize().andThen( [&]() {
        addConditionDerivation( *this, m_srcPath.value(),
                                /// Callable which converts raw information into a derived condition.
                                []( const ParamValidDataObject& obj ) {
                                  const auto& [p1, p2] = obj.params<double, double>( "par1", "par2" );
                                  return MyData{p1, p2, std::sqrt( p1 ) + 2.0 * p2 * p2};
                                } );
      } );
    }

    void operator()( const MyData& cond ) const override {
      info() << "condition value: {\n  p1: " << cond.p1 << "\n  p2: " << cond.p2 << "\n  v:  " << cond.v << "\n}"
             << endmsg;
    }
  };

  /// Example of algorithm that requires caching of condition derivations and inherits from non default base class
  struct MyAlgorithm : Gaudi::Algorithm {
    using Algorithm::Algorithm;
    static constexpr double m_pi{3.1415};
    double                  pi() { return m_pi; };
  };
  struct CondAccessExampleWithDerivationAndBase
      : Gaudi::Functional::Consumer<void( const MyData& ),
                                    LHCb::DetDesc::usesBaseAndConditions<MyAlgorithm, MyData>> {
    CondAccessExampleWithDerivationAndBase( const std::string& name, ISvcLocator* loc )
        : Consumer{name, loc, {KeyValue{"Target", "DerivedCondition"}}} {}

    /// Property used to change the path of the input condition, if needed
    Gaudi::Property<std::string> m_srcPath{this, "Source", "TestCondition"};

    StatusCode initialize() override {
      const auto sc = Consumer::initialize();
      if ( !sc ) return sc;

      addConditionDerivation( m_srcPath.value(), inputLocation<0>(),
                              /// Callable which converts raw information into a derived condition.
                              [this]( const ParamValidDataObject& obj ) -> MyData {
                                const auto& [p1, p2] = obj.params<double, double>( "par1", "par2" );
                                return {p1, p2, std::sqrt( p1 ) + pi() * p2 * p2};
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
DECLARE_COMPONENT( DetCond::Examples::Functional::CondAccessExampleWithDerivationAndBase )
