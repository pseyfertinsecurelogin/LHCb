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
#include "DetDesc/Condition.h"
#include "Event/HltDecReports.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiSequencer.h"
#include "GaudiAlg/ISequencerTimerTool.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ThreadLocalContext.h"
#include "Kernel/IANNSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : TurboPrescaler
//
// Description: The class takes an output TCK and applies
// an additional prescale
//
// 2016-06-13 : Sean Benson
//-----------------------------------------------------------------------------

/** @class TurboPrescaler TurboPrescaler.h
 *
 *
 *  @author Sean Benson
 *  @date   2016-06-13
 */
class TurboPrescaler final : public GaudiAlgorithm {

public:
  TurboPrescaler( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  StatusCode execute() override;

private:
  using StringMap = std::map<std::string, double>;

private:
  void getPrescalesFromTCK( unsigned int, StringMap&, StringMap&, StringMap& );

  bool endedWith( const std::string& lineName, const std::string& ending );

  void setupPrescalers();
  void updatePrescalers();

private:
  std::string m_scalerName;
  std::string m_postScalerName;
  std::string m_preScalerName;

  StringMap m_prescalesInput;
  StringMap m_postscalesInput;
  StringMap m_scaleProductsInput;

  std::map<std::string, Algorithm*> prescalers;

  IPropertyConfigSvc*                  m_propertyConfigSvc = nullptr;
  bool                                 m_filter            = false;
  std::string                          m_propertyConfigSvcName;
  DataObjectReadHandle<HltDecReports>  m_hltDecReportsLocation{this, "HltDecReportsLocation",
                                                              HltDecReportsLocation::Default};
  unsigned int                         m_lastTCK   = 0;
  unsigned int                         m_outputTCK = 0;
  std::map<std::string, double>        m_outputPS;
  DataObjectWriteHandle<HltDecReports> m_outRepLoc{this, "ReportsOutputLoc", "Turbo/DecReports"};
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TurboPrescaler )

TurboPrescaler::TurboPrescaler( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "ChosenOutputPrescales", m_outputPS );
  declareProperty( "PrescaleVersion", m_outputTCK );
  declareProperty( "FilterOutput", m_filter = true );
  declareProperty( "ConfigQualifier", m_scalerName = "DeterministicPrescaler" );
  declareProperty( "PreScalerQualifier", m_preScalerName = "PreScaler" );
  declareProperty( "PostScalerQualifier", m_postScalerName = "PostScaler" );
  declareProperty( "IPropertyConfigSvcInstance", m_propertyConfigSvcName = "PropertyConfigSvc" );
}

StatusCode TurboPrescaler::initialize() {
  return GaudiAlgorithm::initialize()
      .andThen( [&] {
        // Get property config service
        return service( m_propertyConfigSvcName, m_propertyConfigSvc ).orElse( [&] {
          fatal() << "Failed to get the IConfigAccessSvc." << endmsg;
        } );
      } )
      .andThen( [&] { setupPrescalers(); } );
}

StatusCode TurboPrescaler::execute() {
  StatusCode sc = StatusCode::SUCCESS;

  // Get the input reports and if the TCK changes, refresh list of prescales
  auto decreports = m_hltDecReportsLocation.getIfExists();
  if ( !decreports ) return sc;

  auto tck = decreports->configuredTCK();
  if ( 0 == m_lastTCK ) { getPrescalesFromTCK( tck, m_prescalesInput, m_postscalesInput, m_scaleProductsInput ); }

  if ( tck != m_lastTCK || 0 == m_lastTCK ) {
    m_lastTCK = tck;
    updatePrescalers();
  }

  bool globalPass = false;

  auto reports = std::make_unique<HltDecReports>();

  reports->setConfiguredTCK( m_outputTCK );
  reports->setTaskID( decreports->taskID() );

  std::string lineName;
  std::string totName;
  std::string strip = "Decision";

  // loop over the Decreports
  for ( const auto& dec : *decreports ) {

    // Get the name of line, if it ends with "Decision" remove it:
    lineName = dec.first;
    totName  = lineName;
    if ( endedWith( lineName, strip ) ) { lineName.erase( lineName.end() - strip.length(), lineName.end() ); }

    // Get the report
    HltDecReport report = dec.second;

    // We only care about lines that fired:
    if ( report.decision() ) {

      // Pull the DeterministicPrescaler for this line from out map of DPs:
      auto* prescaler = prescalers[lineName];

      // Execute prescaler
      if ( prescaler && prescaler->isEnabled() ) {
        const auto result = prescaler->sysExecute( Gaudi::Hive::currentContext() );
        if ( !result.isSuccess() ) return StatusCode::FAILURE;
        // If the DP wants the line killed:
        if ( !prescaler->filterPassed() ) {
          report.setDecision( false );
        } else {
          globalPass = true;
        }
      }
    }
    reports->insert( totName, report ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  }
  m_outRepLoc.put( std::move( reports ) );

  setFilterPassed( m_filter ? globalPass : true );

  return sc;
}

void TurboPrescaler::getPrescalesFromTCK( unsigned int tck, StringMap& prescales, StringMap& postscales,
                                          StringMap& scaleProducts ) {
  prescales.clear();
  postscales.clear();
  scaleProducts.clear();
  TCK _tck( tck );
  _tck.normalize();

  ConfigTreeNodeAlias::alias_type alias( std::string( "TCK/" ) + _tck.str() );
  const auto*                     tree = m_propertyConfigSvc->resolveConfigTreeNode( alias );
  if ( !tree ) {
    fatal() << "TCK " << std::hex << _tck << " could not be resolved! Cannot continue" << endmsg;
    return;
  }

  const auto& digests = m_propertyConfigSvc->collectLeafRefs( tree->digest() );
  std::string lineName;
  for ( const auto& j : digests ) {
    const auto* config = m_propertyConfigSvc->resolvePropertyConfig( j );
    if ( config ) {

      // Check that fullyQualifiedName contains "DeterministicPrescaler"
      if ( std::string::npos != config->fullyQualifiedName().find( m_scalerName ) ) {

        // If it does, get the name
        lineName = config->name();
        if ( lineName.find( "Turbo" ) == std::string::npos ) continue;

        // Loop to find the property "AcceptFraction"
        for ( const auto& i : config->properties() ) {
          if ( i.first == "AcceptFraction" ) {
            double scale = -9999.;
            try {
              std::string str2 = ( i.second );
              scale            = std::strtod( str2.c_str(), 0 );
            } catch ( const std::exception& ) {
              Warning( "could not find pre/post scale in " + i.first ).ignore();
              scale = -9999.;
            }
            if ( scale > -9999. ) {
              // Is it a Prescale, Postscale, or something else?
              // prescale:
              if ( endedWith( lineName, m_preScalerName ) ) {
                lineName.erase( lineName.end() - m_preScalerName.length(), lineName.end() );
                prescales[lineName] = scale;           // Write the prescale to a map
                postscales.insert( {lineName, -1.0} ); // Initialise a postscale if it isn't already there with a
                                                       // nonsense value
                break;
              } else {
                // postscale:
                if ( endedWith( lineName, m_postScalerName ) ) {
                  lineName.erase( lineName.end() - m_postScalerName.length(), lineName.end() );
                  postscales[lineName] = scale;         // Write the postscale to a map
                  prescales.insert( {lineName, -1.0} ); // Initialise a prescale if it isn't already there with a
                                                        // nonsense value
                  break;
                } else {
                  Warning( "Found property AcceptFraction not associated to a pre/post scale! " + lineName ).ignore();
                }
              }
            }
          }
        }
      }
    }
  }

  // Make products of pre, post scales, print (if verbose)
  for ( const auto& i : prescales ) {
    auto pre = i.second;
    if ( pre < 0.0 ) { pre = 1.0; } // If prescale was never found it'll be -9999., and we assume it will always pass
    auto post = postscales[i.first];
    if ( post < 0.0 ) { post = 1.0; } // If postscale was never found it'll be -9999., and we assume it will always pass
    scaleProducts.insert( {i.first, pre * post} );
  }
}

void TurboPrescaler::setupPrescalers() {
  prescalers.clear();

  //== Get the "Context" option if in the file...
  auto jos = service<IJobOptionsSvc>( "JobOptionsSvc" );

  //= Get the Application manager, to see if algorithm exist
  auto appMgr = service<IAlgManager>( "ApplicationMgr" );

  for ( const auto& it : m_outputPS ) {

    std::string tn = "DeterministicPrescaler/";
    tn.append( it.first );
    tn.append( "DPForPrescaleOffline" );
    const Gaudi::Utils::TypeNameString typeName( tn );
    const std::string&                 theName = typeName.name();

    //== Check wether the specified algorithm already exists. If not, create it
    StatusCode          result = StatusCode::SUCCESS;
    SmartIF<IAlgorithm> myIAlg = appMgr->algorithm( typeName, false ); // do not create it now

    if ( !myIAlg.isValid() ) {

      // Set the acceptfraction and verbosity for the DP:
      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "Setting an acceptfraction for " << theName << endmsg;
      IntegerProperty outputLevelProperty( "OutputLevel", MSG::WARNING );
      jos->addPropertyToCatalogue( theName, outputLevelProperty ).ignore();

      DoubleProperty acceptFractionProperty( "AcceptFraction", it.second );
      jos->addPropertyToCatalogue( theName, acceptFractionProperty ).ignore();

      myIAlg = appMgr->algorithm( typeName, false );
    }

    Algorithm* myAlg = dynamic_cast<Algorithm*>( myIAlg.get() );
    if ( myAlg ) {
      prescalers.insert( {it.first, myAlg} );
    } else {
      warning() << theName << " is not an Algorithm - failed dynamic_cast" << endmsg;
    }
  }
}

void TurboPrescaler::updatePrescalers() {
  for ( const auto& i : m_prescalesInput ) {
    auto* pre = prescalers[i.first];
    if ( !pre ) continue;

    auto j = m_outputPS.find( i.first );
    if ( j != m_outputPS.end() ) {
      if ( j->second > 0.0 ) {
        const double ratio = j->second / i.second;
        if ( ratio <= 1.0 ) {
          DoubleProperty acceptFractionProperty( "AcceptFraction", ratio );
          pre->setProperty( acceptFractionProperty ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        } else {
          warning() << "*********************************************************************************" << endmsg;
          warning() << "OUTPUT HAS LARGER ACCEPTFRACTION THAN INPUT, CANNOT CREATE EVENTS. LEAVING AS UNITY" << endmsg;
          warning() << "*********************************************************************************" << endmsg;
          DoubleProperty acceptFractionProperty( "AcceptFraction", 1.0 );
          pre->setProperty( acceptFractionProperty ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        }
      }
    }
  }
}

bool TurboPrescaler::endedWith( const std::string& lineName, const std::string& ending ) {
  return ( lineName.length() > ending.length() &&
           ( 0 == lineName.compare( lineName.length() - ending.length(), ending.length(), ending ) ) );
}
