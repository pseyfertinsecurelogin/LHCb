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
#include <array>
#include <numeric>
#include <string_view>
// from Gaudi
#include "GaudiKernel/StateMachine.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "Kernel/STLExtensions.h"

// local
#include "L0DUConfigProvider.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0DUConfigProvider
//
//  Provide a **SINGLE** L0DU configuration according to
//  the Trigger Configuration Key
//  Configuration described in options file
//
//
// 2007-10-25 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( L0DUConfigProvider )

using namespace std::string_literals;

namespace {
  static const auto s_dataFlags = std::array{"name"s, "data"s, "operator"s};
  static const auto s_condFlags =
      std::array{"name"s, "data"s, "comparator"s, "threshold"s, "index"s, "reported"s, "bx"s};
  static const auto s_chanFlags = std::array{"name"s, "condition"s, "rate"s, "enable"s, "disable"s, "mask"s, "index"s};
  static const auto s_trigFlags = std::array{"name"s, "channel"s, "index"s, "type"s};
  // define the allowed operator and comparators
  static const auto s_comparators = std::array{">"s, "<"s, "=="s, "!="s};
  // pair(operator,dimension)
  static const auto s_operators =
      std::array{std::pair{"+"s, 2}, std::pair{"-"s, 2}, std::pair{"&"s, 2}, std::pair{"^"s, 2}};
  // index of the predefined triggers
  static const std::map<std::string, int> s_tIndices = {{"L0Ecal"s, 0}, {"L0Hcal"s, 1}, {"L0Muon"s, 2}, {"Other"s, 3}};

  // Try to find, in the Haystack, the Needle - ignore case
  bool contains_ci( std::string_view haystack, std::string_view needle ) {
    auto it = std::search( begin( haystack ), end( haystack ), begin( needle ), end( needle ),
                           []( char ch1, char ch2 ) { return std::toupper( ch1 ) == std::toupper( ch2 ); } );
    return it != end( haystack );
  }

  bool icompare( std::string const& a, std::string const& b ) {
    return std::equal( b.begin(), b.end(), a.begin(), a.end(),
                       []( char ch1, char ch2 ) { return std::toupper( ch1 ) == std::toupper( ch2 ); } );
  }

  std::vector<std::string> triggerNameFromData( const std::vector<std::string>& dataList ) {
    std::vector<std::string> name;
    // The actual trigger->data association definition
    // 1st pass for the main trigger : L0Muon, L0Ecal, L0Hcal
    for ( const auto& id : dataList ) {
      auto id_matches = [&id]( const char* cs ) { return id.compare( 0, 4, cs ) == 0; };
      if ( id_matches( "Muon" ) || id_matches( "DiMu" ) ) {
        name.push_back( "L0Muon" );
      } else if ( id_matches( "Elec" ) || id_matches( "Phot" ) || id_matches( "Loca" ) || id_matches( "Glob" ) ) {
        name.push_back( "L0Ecal" );
      } else if ( id_matches( "Hadr" ) ) {
        name.push_back( "L0Hcal" );
      }
    }
    if ( name.empty() ) name.push_back( "Other" );
    return name;
  }

} // namespace
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0DUConfigProvider::L0DUConfigProvider( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ) {
  declareProperty( "constData", m_constData )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "Data", m_data )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "Conditions", m_conditions )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "Channels", m_channels )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "Triggers", m_triggers )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "MuonFOIx", m_FOIx )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "MuonFOIy", m_FOIy )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  // for options defined configuration
  declareProperty( "Description", m_def = "NO DESCRIPTION" )
      ->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "Name", m_recipe = "" )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  m_sepMap["["] = "]";
  declareProperty( "Separators", m_sepMap )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "FullDescription", m_detail = false )->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  // TCK from name
  int         idx = name.find_last_of( "." ) + 1;
  std::string nam = name.substr( idx );
  if ( ( nam == LHCb::L0DUTemplateConfig::Name ) | ( nam == "L0DUConfig" ) ) {
    m_template = true;
  } else {
    size_t index = nam.rfind( "0x" );
    nam          = ( index != std::string::npos ) ? nam.substr( index ) : "0x0000";
  }

  declareProperty( "TCK", m_tck = m_template ? format( "0x%04X", LHCb::L0DUTemplateConfig::TCKValue ) : nam )
      ->declareUpdateHandler( &L0DUConfigProvider::handler, this );

  // expert usage
  declareProperty( "ForceConditionOrdering", m_forceOrder = false )
      ->declareUpdateHandler( &L0DUConfigProvider::handler, this );
  declareProperty( "HardwareCheckDetail", m_check = false )->declareUpdateHandler( &L0DUConfigProvider::handler, this );

  if ( context().find( "Check" ) != std::string::npos ) {
    m_detail = true;
    m_check  = true;
  }
}

//============
// finalize
//============
StatusCode L0DUConfigProvider::finalize() {

  info() << "--------------- TCK = " << format( "0x%04X", m_tckopts ) << "------------------" << endmsg;
  configChecker();
  reset();
  return base_class::finalize();
}

//=============================================================================
// Initialize is the main method
//=============================================================================
StatusCode L0DUConfigProvider::initialize() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Initialize L0DUConfigProvider" << endmsg;
  StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) return sc;

  // get condDB tool
  m_condDB = tool<IL0CondDBProvider>( "L0CondDBProvider" );
  return update();
}

void L0DUConfigProvider::handler( Property& ) {
  if ( !m_template && FSMState() >= Gaudi::StateMachine::INITIALIZED ) {
    // on-the-fly update of properties only allowed for template!
    error() << "only template L0DUConfig can be updated after  Initialize has been called" << endmsg;
    throw GaudiException( "L0DUConfig: update of properties only allowed for template", "", StatusCode::FAILURE );
  }
  m_uptodate = false;
}

void L0DUConfigProvider::reset() {
  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "reset L0DUConfigProvider" << endmsg;
    debug() << "Deleting " << m_triggersMap.size() << " L0DUTrigger* " << endmsg;
  }
  for ( auto& i : m_triggersMap ) delete i.second;
  m_triggersMap.clear();

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Deleting " << m_dataMap.size() << " L0DUElementaryData* " << endmsg;
  for ( auto& i : m_dataMap ) delete i.second;
  m_dataMap.clear();
  m_pData = 0;
  m_cData = 0;

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Deleting " << m_conditionsMap.size() << " L0DUElementaryConditions* " << endmsg;
  for ( auto& i : m_conditionsMap ) delete i.second;
  m_conditionsMap.clear();

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Deleting " << m_channelsMap.size() << " L0DUElementaryChannels* " << endmsg;
  for ( auto& i : m_channelsMap ) delete i.second;
  m_channelsMap.clear();

  m_config = nullptr;
  m_configs.clear();
}

StatusCode L0DUConfigProvider::update() {
  int index = m_tck.rfind( "0x" ) + 2;
  m_tckopts = std::stoi( m_tck.substr( index ), nullptr, 16 );

  // init
  int num = L0DUBase::NumberOf::Data + L0DUBase::NumberOf::Compounds; // incl. BCID
  m_condOrder.clear();
  m_condOrder.resize( num );
  m_condMax.clear();
  m_condMax.resize( num, 0 );

  info() << "--------------- TCK = " << format( "0x%04X", m_tckopts ) << "------------------" << endmsg;

  // load predefined elementary data
  predefinedData(); // must be 1st
  constantData();   // must be 2nd

  //=====================================
  if ( m_def == "" ) m_def = "No Description";
  if ( m_template )
    m_def += " (L0DUConfig.Template)";
  else if ( m_channels.empty() || m_conditions.empty() ) {
    Warning( "Configuration (TCK = " + format( "0x%04X", m_tckopts ) + ") is empty", StatusCode::SUCCESS ).ignore();
  }

  StatusCode sc = createTriggers(); // the main method - crate triggers->channels->conditions(->data) chain

  if ( sc.isFailure() ) {
    fatal() << " configuring L0DU (TCK = " << format( "0x%04X", m_tckopts ) << ") failed" << endmsg;
    return StatusCode::FAILURE;
  }

  std::string slot = ( rootInTES().empty() ? "T0" : rootInTES() );
  if ( slot.empty() ) slot = "T0";

  // The main method
  createConfig( slot );

  configChecker();

  m_uptodate = true;
  return StatusCode::SUCCESS;
}

//-------------------------------------------
void L0DUConfigProvider::createConfig( std::string slot ) {

  // create L0DU configuration
  m_config = new LHCb::L0DUConfig( m_tckopts );

  m_config->setDefinition( m_def );

  if ( m_recipe.empty() ) {
    std::stringstream recipe;
    recipe << "Undefined recipe name for TCK=" << format( "0x%04X", m_tckopts );
    m_config->setRecipe( recipe.str() );
  } else {
    m_config->setRecipe( m_recipe );
  }
  //=====================================
  m_config->setData( m_dataMap );
  m_config->setConditions( m_conditionsMap );
  m_config->setChannels( m_channelsMap );
  if ( !m_triggersMap.empty() ) m_config->setTriggers( m_triggersMap );
  auto it = m_configs.find( slot );
  if ( it == m_configs.end() ) {
    auto configs = new LHCb::L0DUConfigs();
    configs->addRef();
    auto ret = m_configs.emplace( slot, configs );
    assert( ret.second );
    it = ret.first;
  }
  if ( !m_FOIx.empty() ) m_config->setMuonFOIx( m_FOIx );
  if ( !m_FOIy.empty() ) m_config->setMuonFOIy( m_FOIy );

  it->second->insert( m_config );
  //=====================================
  printConfig( *m_config, slot );
}

void L0DUConfigProvider::printConfig( const LHCb::L0DUConfig& config, std::string slot ) const {
  if ( slot.empty() ) slot = "T0";
  info() << "**** L0DU Config loading : L0TCK = " << format( "0x%04X", config.tck() ) << " for slot " << slot
         << " ==> OK " << endmsg;
  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "              - " << config.data().size() << " data with " << endmsg;
    debug() << "                    - " << m_pData << " predefined data " << endmsg;
    debug() << "                    - " << m_cData << " constant   data " << endmsg;
    debug() << "                    - " << config.data().size() - m_cData - m_pData << " new data " << endmsg;
    debug() << "              - " << config.conditions().size() << " Elementary conditions " << endmsg;
    debug() << "              - " << config.channels().size() << " Channels " << endmsg;
    debug() << " " << endmsg;
  }
  info() << "Short description :: " << config.definition() << endmsg;
  if ( m_detail ) info() << "Full description  :: " << config.description() << endmsg;
}

//-----------------
// Predefined data
//-----------------
//=============================================================================
void L0DUConfigProvider::predefinedData() {
  using namespace L0DUBase;

  for ( unsigned i = 0; i < NumberOf::Data; ++i ) {
    const std::string& name = PredefinedData::Name[i];
    auto data = std::make_unique<LHCb::L0DUElementaryData>( i, LHCb::L0DUElementaryData::Type::Predefined, name );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Predefined Data : " << data->description() << endmsg;
    m_dataMap[name] = data.release();
    m_pData++;
  }
}

//=============================================================================

//---------------
// Constant data
//---------------

void L0DUConfigProvider::constantData() {
  for ( const auto& idata : m_constData ) {
    auto data = std::make_unique<LHCb::L0DUElementaryData>( m_pData + m_cData, LHCb::L0DUElementaryData::Type::Constant,
                                                            idata.first );
    data->setDigit( idata.second );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Constant Data : " << data->summary() << endmsg;
    m_dataMap[idata.first] = data.release();
    m_cData++;
  }
}

//===============================================================
std::vector<std::string> L0DUConfigProvider::Parse( const std::string& flag, const std::vector<std::string>& config ) {

  const std::pair<std::string, std::string>& separators = *begin( m_sepMap );
  if ( m_sepMap.size() != 1 )
    warning() << "A single pair of separators must be defined - will use the first : " << separators.first << "data"
              << separators.second << endmsg;

  std::vector<std::string> values;
  for ( const auto& iconfig : config ) {
    if ( contains_ci( iconfig, flag ) ) {
      if ( msgLevel( MSG::VERBOSE ) )
        verbose() << "Flag '" << flag << "' found in the data string '" << iconfig << "'" << endmsg;

      // loop over separators
      int id = iconfig.find( separators.first );
      if ( id < 0 ) {
        error() << "Unable to parse the tag " << iconfig << " due to a missing separator (" << separators.first << "..."
                << separators.second << ")" << endmsg;
        values.clear();
        return values;
      }
      while ( id >= 0 ) {
        auto from = iconfig.find( separators.first, id );
        auto to   = iconfig.find( separators.second, from + 1 );
        if ( from != std::string::npos && to != std::string::npos ) {
          auto val = iconfig.substr( from + 1, to - from - 1 );
          if ( msgLevel( MSG::VERBOSE ) ) verbose() << "parsed value = '" << val << "'" << endmsg;
          values.push_back( std::move( val ) );
          id = iconfig.find( separators.first, to + 1 );
        } else {
          id = -1;
          error() << "Unable to parse the tag " << iconfig << " due to a missing separator (" << separators.first
                  << "..." << separators.second << ")" << endmsg;
          values.clear();
          return values;
        }
      }
    }
  }
  return values;
}

//===============================================================
StatusCode L0DUConfigProvider::createData() {
  //===== create compound data
  int id = m_dataMap.size();
  for ( auto iconfig = m_data.begin(); iconfig != m_data.end(); ++iconfig ) {
    std::vector<std::string> values;

    // check all tags exist
    for ( auto itag = iconfig->begin(); itag != iconfig->end(); itag++ ) {
      if ( std::none_of( s_dataFlags.begin(), s_dataFlags.end(),
                         [&]( const std::string& df ) { return contains_ci( *itag, df ); } ) ) {
        error() << "Description tag : '" << *itag
                << "' is unknown for the new DATA defined via options (num =  " << iconfig - m_data.begin() << ")"
                << endmsg;
        info() << "Allowed flags for new data description are : " << s_dataFlags << endmsg;
        return StatusCode::FAILURE;
      }
    }

    // The Data name
    //--------------
    values = Parse( "name", *iconfig );

    if ( values.size() != 1 ) {
      error() << "The DATA definied via option (num = " << iconfig - m_data.begin()
              << ") should have an unique name (found " << values.size() << ")" << endmsg;
      info() << "The syntax is ToolSvc.L0DUConfig.TCK_0xXXXXX.Data +={ {''name=[DataName]'', "
             << " ''operator=[+/-/&/^]'', ''data=[OperandName1]'', ''data=[OperandName2]'' , ...}};" << endmsg;

      return StatusCode::FAILURE;
    }
    std::string dataName = values.front(); // The NAME

    // Check the data already exist or not
    auto it = m_dataMap.find( dataName );
    if ( it != m_dataMap.end() ) {
      warning() << "One L0DUElementaryData  with name : '" << dataName
                << "' already exists - Please check your settings" << endmsg;
      return StatusCode::FAILURE;
    }

    //  The operator
    // -------------
    values = Parse( "operator", *iconfig );
    if ( values.size() != 1 ) {
      error() << "Should be an unique operator for the new data : " << dataName << " (found " << values.size() << ")"
              << endmsg;
      return StatusCode::FAILURE;
    }

    std::string op = values.front(); // The OPERATOR

    auto iop = std::find_if( s_operators.begin(), s_operators.end(), [&]( const auto& o ) { return o.first == op; } );
    if ( iop == s_operators.end() ) {
      fatal() << "requested operator " << op << " is not allowed " << endmsg;
      info() << "allowed operators are : " << endmsg;
      for ( const auto& it2 : s_operators ) { info() << "--> " << it2.first << endmsg; }
      return StatusCode::FAILURE;
    }

    unsigned int dim = iop->second;

    // the operands
    //--------------
    std::vector<std::string> operands = Parse( "data", *iconfig );
    if ( dim != operands.size() ) {
      fatal() << "Number of operands " << values.size() << "does not match the dimension of the operator " << op
              << "(dim = " << dim << ")" << endmsg;
      return StatusCode::FAILURE;
    }

    auto jop = std::find_if( begin( operands ), end( operands ),
                             [&]( const std::string& op ) { return m_dataMap.find( op ) == m_dataMap.end(); } );
    if ( jop != end( operands ) ) {
      error() << "new compound Data is based on an unknown Data '" << *jop << "' ... check  your settings " << endmsg;
      return StatusCode::FAILURE;
    }

    // create Compound Data
    auto data = std::make_unique<LHCb::L0DUElementaryData>( id, dataName, op, operands ); // compound constructor
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Created Data : " << data->description() << endmsg;
    m_dataMap[dataName] = data.release();
    id++;
  }
  return StatusCode::SUCCESS;
}

//===============================================================
StatusCode L0DUConfigProvider::createConditions() {

  StatusCode sc = createData();
  if ( sc.isFailure() ) return sc;

  int id = m_conditionsMap.size();
  for ( auto iconfig = m_conditions.begin(); iconfig != m_conditions.end(); ++iconfig ) {

    // check all tags exist
    for ( const auto& tag : *iconfig ) {
      if ( std::none_of( begin( s_condFlags ), end( s_condFlags ),
                         [&]( const std::string& s ) { return contains_ci( tag, s ); } ) ) {
        error() << "Description tag : '" << tag
                << "' is unknown for the new CONDITION defined via options (num =  " << iconfig - m_conditions.begin()
                << ")" << endmsg;
        info() << "Allowed flags for new CONDITION description are : " << s_condFlags << endmsg;
        return StatusCode::FAILURE;
      }
    }

    // The Condition name
    //--------------------
    std::vector<std::string> values = Parse( "name", *iconfig );
    if ( values.size() != 1 ) {
      error() << "The CONDITION defined via option (num = " << iconfig - m_conditions.begin()
              << ") should have an unique name (found " << values.size() << ")" << endmsg;
      info() << "The syntax is ToolSvc.L0DUConfig.TCK_0xXXXX.Conditions +={ {''name=[ConditionName]'', "
             << " ''data=[dataName]'', ''comparator=[>/</==/!=]'', ''threshold=[value]''} };" << endmsg;

      return StatusCode::FAILURE;
    }

    std::string conditionName = values.front(); // The NAME

    // Check the condition already exist or not (if yes overwrite it)
    auto ic = m_conditionsMap.find( conditionName );
    if ( ic != m_conditionsMap.end() ) {
      warning() << "One L0DUElementaryCondition with name : '" << conditionName
                << "' already exists  - Please check your settings" << endmsg;
      return StatusCode::FAILURE;
    }

    // The Data
    //------------------
    values = Parse( "data", *iconfig );
    if ( values.size() != 1 ) {
      error() << "Should be an unique dataName for the new CONDITION : " << conditionName << " (found " << values.size()
              << ")" << endmsg;
      return StatusCode::FAILURE;
    }

    std::string data = values.front(); // The dataName
    // check for aliases
    auto it = L0DUBase::PredefinedData::Alias.find( data );
    if ( it != L0DUBase::PredefinedData::Alias.end() ) data = it->second;

    // Special case : create RAM(BCID) data on-the-fly
    if ( data.rfind( "RAM" ) != std::string::npos && data.rfind( "(BCID)" ) != std::string::npos ) {
      int idData      = m_dataMap.size();
      m_dataMap[data] = new LHCb::L0DUElementaryData( idData, LHCb::L0DUElementaryData::Type::RAMBcid, data );
      // TEMP
      int         ind = data.rfind( "(BCID)" );
      std::string vsn = data.substr( 0, ind );
      if ( msgLevel( MSG::DEBUG ) )
        debug() << "RAM(BCID) L0DU DATA for RAM vsn = " << vsn << " HAS BEEN DEFINED" << endmsg;
      // check the RAM version exists
      const std::vector<int> ram = m_condDB->RAMBCID( vsn );
      if ( ram.empty() ) {
        fatal() << "RAM(BCID) versions '" << vsn << "' is not registered" << endmsg;
        return StatusCode::FAILURE;
      }
    }
    // Check data consistency
    auto idata = m_dataMap.find( data );
    if ( m_dataMap.end() == idata ) {
      fatal() << " Can not set-up the '" << conditionName << "' L0DU Elementary Condition, "
              << " because the required L0DU Data '" << data << "' is not defined." << endmsg;
      info() << " The predefined L0DU Data are : " << endmsg;
      for ( const auto& idata : m_dataMap ) {
        const std::string& knownName = idata.second->name();
        auto               ia        = std::find_if(
            L0DUBase::PredefinedData::Alias.begin(), L0DUBase::PredefinedData::Alias.end(),
            [&]( const std::pair<std::string, std::string>& alias ) { return alias.second == knownName; } );
        std::string knownAlias = ( ia != L0DUBase::PredefinedData::Alias.end() ? " ( aka " + ia->first + " )" : "" );
        info() << " -->  " << knownName << knownAlias << endmsg;
      }
      return StatusCode::FAILURE;
    }

    // the Comparator
    //---------------
    values = Parse( "comparator", *iconfig );
    if ( values.size() != 1 ) {
      error() << "Should be an unique comparator for the CONDITION : " << conditionName << " (found " << values.size()
              << ")" << endmsg;
      return StatusCode::FAILURE;
    }

    std::string comp = values.front(); // The COMPARATOR
    if ( std::find( s_comparators.begin(), s_comparators.end(), comp ) == s_comparators.end() ) {
      fatal() << "requested comparator " << comp << " is not allowed " << endmsg;
      info() << "Allowes comparators are : " << s_comparators << endmsg;
      return StatusCode::FAILURE;
    }

    // the Threshold
    //---------------
    values = Parse( "threshold", *iconfig );
    if ( values.size() != 1 ) {
      error() << "Should be an unique threshold for the new CONDITION : " << conditionName << " (found "
              << values.size() << ")" << endmsg;
      return StatusCode::FAILURE;
    }

    unsigned long threshold = std::stoul( values.front() ); // The THRESHOLD

    // report  (facultatif)
    //----------
    std::vector<std::string> reports = Parse( "reported", *iconfig );
    if ( reports.size() > 1 ) {
      error() << "Should be an unique report for the new CONDITION : " << conditionName << " (found " << reports.size()
              << ")" << endmsg;
      return StatusCode::FAILURE;
    }
    bool reported = true;
    if ( !reports.empty() ) {
      const std::string& report = reports.front();
      if ( icompare( report, "FALSE" ) ) {
        reported = false;
        info() << "The condition '" << conditionName << "' is NOT to be reported in L0DUReport " << endmsg;
      } else if ( !icompare( report, "TRUE" ) ) {
        error() << "the CONDITION '" << conditionName << "' report should be True or False" << endmsg;
        return StatusCode::FAILURE;
      }
    }
    if ( reported && msgLevel( MSG::DEBUG ) )
      debug() << "The condition '" << conditionName << "' is to be reported in L0DUReport " << endmsg;

    // the index (facultatif)
    //----------
    values = Parse( "index", *iconfig );
    if ( values.size() > 1 ) {
      error() << "Should be an unique index for the new CONDITION : " << conditionName << " (found " << values.size()
              << ")" << endmsg;
      return StatusCode::FAILURE;
    }
    int index = ( values.empty() ? id : std::stoi( values.front() ) ); // The INDEX
    // check the index is not already used
    for ( const auto& ii : m_conditionsMap ) {
      if ( index == ii.second->index() ) {
        error() << "The bit index " << index << " is already assigned to the Condition " << ii.second->name() << endmsg;
        return StatusCode::FAILURE;
      }
    }

    // the BX (facultative)
    //---------------------
    values = Parse( "bx", *iconfig );
    if ( values.size() > 1 ) {
      error() << "Should be an unique BX for the CONDITION : " << conditionName << " (found " << values.size() << ")"
              << endmsg;
      return StatusCode::FAILURE;
    }

    int bx = 0;
    if ( values.size() == 1 ) {
      bx = std::stoi( values.front() ); // The BX
      if ( ( std::find( m_knownBXs.begin(), m_knownBXs.end(), bx ) == m_knownBXs.end() ) )
        Warning( "'" + conditionName +
                     "' : L0DU firmware can only be configured for BX=" + Gaudi::Utils::toString( m_knownBXs ),
                 StatusCode::SUCCESS, 5 )
            .ignore();
    }

    // create condition (name,data,comparator,threshold)
    LHCb::L0DUElementaryCondition* condition =
        new LHCb::L0DUElementaryCondition( index, conditionName, idata->second, comp, threshold );

    condition->setBx( bx );
    condition->setReported( reported );
    m_conditionsMap[conditionName] = condition;
    id++;

    if ( !conditionCheck( condition ) )
      return Error( "Condition '" + conditionName + "' check failed", StatusCode::FAILURE );

    if ( msgLevel( MSG::DEBUG ) ) debug() << "Created Condition : " << condition->description() << endmsg;
  }
  //
  return conditionOrdering() ? StatusCode::SUCCESS : StatusCode::FAILURE;
}

//===============================================================
StatusCode L0DUConfigProvider::createChannels() {

  StatusCode sc = createConditions();
  if ( sc.isFailure() ) return sc;

  int id = m_channelsMap.size();
  for ( auto iconfig = m_channels.begin(); iconfig != m_channels.end(); ++iconfig ) {

    // check all tags exist
    for ( const auto& tag : *iconfig ) {
      if ( std::none_of( begin( s_chanFlags ), end( s_chanFlags ),
                         [&]( const std::string& s ) { return contains_ci( tag, s ); } ) ) {
        error() << "Description tag : '" << tag
                << "' is unknown for the new CHANNEL defined via options (num =  " << iconfig - m_channels.begin()
                << ")" << endmsg;
        info() << "Allowed flags for new CHANNEL description are : " << s_chanFlags << endmsg;
        return StatusCode::FAILURE;
      }
    }

    // The Channel name
    //------------------
    std::vector<std::string> values = Parse( "name", *iconfig );
    if ( values.size() != 1 ) {
      error() << "The CHANNEL defined via option (num = " << iconfig - m_channels.begin()
              << ") should have an unique name (found " << values.size() << ")" << endmsg;
      info() << "The syntax is ToolSvc.L0DUConfig.TCK_0xXXXX.Channels +={ {''name=[ChannelName]'', "
             << " ''rate=[value]'', ''condition=[conditionName1],[conditionName2], ...'', ''disable=[FALSE]'' } };"
             << endmsg;

      return StatusCode::FAILURE;
    }

    std::string channelName = values.front(); // The NAME

    // Check if the channel already exists
    auto ic = m_channelsMap.find( channelName );
    if ( ic != m_channelsMap.end() ) {
      warning() << "One L0DUChannel with name = : '" << channelName << "' already exists "
                << " - Please check your settings" << endmsg;
      return StatusCode::FAILURE;
    }

    // The rate
    //----------
    values = Parse( "rate", *iconfig );
    if ( values.size() != 1 ) {
      error() << "Should be an unique rate for the new CHANNEL : " << channelName << " (found " << values.size() << ")"
              << endmsg;
      return StatusCode::FAILURE;
    }

    double rate = std::stod( values.front() ); // The rate

    if ( rate < 0. || rate > 100. ) {
      error() << "The channel accept rate " << rate << " (%) rate should be between 0 and 100" << endmsg;
      return StatusCode::FAILURE;
    }

    rate *= ( (double)LHCb::L0DUCounter::Scale ) / 100.;
    unsigned int irate = (unsigned int)rate;

    // decision mask (facultatif : default == 0x1 == Physics)
    //-----------------------
    std::vector<std::string> enables  = Parse( "enable", *iconfig );
    std::vector<std::string> disables = Parse( "disable", *iconfig );
    std::vector<std::string> masks    = Parse( "mask", *iconfig );

    int type = LHCb::L0DUDecision::Physics; // default
    if ( enables.size() + disables.size() + masks.size() > 1 ) {
      error() << "Is the channel " << channelName << " enabled or disabled ?  Please check your settings." << endmsg;
      return StatusCode::FAILURE;
    } else if ( enables.size() == 1 ) {
      Warning( "L0DUChannel flag 'Enable' is deprecated - please move to 'Mask' instead", StatusCode::SUCCESS )
          .ignore();
      const std::string& item = enables.front();
      if ( icompare( item, "FALSE" ) ) {
        type = LHCb::L0DUDecision::Disable;
      } else {
        error() << "Decision type for channel '" << channelName << "' is badly defined -  Please check your setting"
                << endmsg;
        return StatusCode::FAILURE;
      }
    } else if ( disables.size() == 1 ) {
      Warning( "L0DUChannel flag 'Disable' is deprecated - please move to 'Mask' instead", StatusCode::SUCCESS )
          .ignore();
      const std::string& item = disables.front();
      if ( icompare( item, "TRUE" ) ) {
        type = LHCb::L0DUDecision::Disable;
      } else {
        error() << "Decision type for channel '" << channelName << "' is badly defined -  Please check your setting"
                << endmsg;
        return StatusCode::FAILURE;
      }
    } else if ( masks.size() == 1 ) {
      const std::string& item = masks.front();
      if ( item == "1" || icompare( item, "0X1" ) || item == "001" || icompare( item, "Physics" ) ) {
        type = LHCb::L0DUDecision::Physics;
      } else if ( item == "2" || icompare( item, "0X2" ) || item == "010" || icompare( item, "Beam1" ) ) {
        type = LHCb::L0DUDecision::Beam1;
      } else if ( item == "4" || icompare( item, "0X4" ) || item == "100" || icompare( item, "Beam2" ) ) {
        type = LHCb::L0DUDecision::Beam2;
      } else if ( item == "011" || item == "3" || icompare( item, "0x3" ) ) {
        type = 3;
      } else if ( item == "101" || item == "5" || icompare( item, "0x5" ) ) {
        type = 5;
      } else if ( item == "110" || item == "6" || icompare( item, "0x6" ) ) {
        type = 6;
      } else if ( item == "111" || item == "7" || icompare( item, "0x7" ) ) {
        type = 7;
      } else if ( item == "000" || item == "0" || icompare( item, "0x0" ) ) {
        type = 0;
      } else {
        error() << "Decision type for channel '" << channelName << "' is badly defined -  Please check your setting"
                << endmsg;
        return StatusCode::FAILURE;
      }
    } else {
      // no indication => enable the channel for Physics (default)
      type = LHCb::L0DUDecision::Physics; // default
    }

    // the index (facultatif)
    // ---------
    values = Parse( "index", *iconfig );
    if ( values.size() > 1 ) {
      error() << "Should be an unique index for the new CHANNEL : " << channelName << " (found " << values.size() << ")"
              << endmsg;
      return StatusCode::FAILURE;
    }
    int index = ( values.empty() ? id : std::stoi( values.front() ) ); // The INDEX
    // check the index is not already used
    for ( const auto& ii : m_channelsMap ) {
      if ( index == ii.second->index() ) {
        error() << "The bit index " << index << " is already assigned to the Channel " << ii.second->name() << endmsg;
        return StatusCode::FAILURE;
      }
    }

    // create channel
    auto channel = std::make_unique<LHCb::L0DUChannel>( index, channelName, irate, type );

    // The conditions
    // --------------
    values = Parse( "condition", *iconfig );
    if ( values.empty() ) {
      error() << "The channel " << channelName << " has no ElementaryCondition" << endmsg;
      return StatusCode::FAILURE;
    }

    // add Elementary Condition(s)
    for ( const auto& icond : values ) {
      // Find required Elementary Condition
      auto ic  = m_conditionsMap.find( icond );
      auto icc = m_channelsMap.find( icond );
      if ( m_conditionsMap.end() != ic && m_channelsMap.end() != icc ) {
        error() << "An ElementaryCondition and a Channel have the same name - please check your setting " << endmsg;
        return StatusCode::FAILURE;
      }
      if ( m_conditionsMap.end() == ic ) {
        // check if the name  is a previously defined channel
        if ( m_channelsMap.end() == icc || icond == channelName ) {
          fatal() << " Can not set-up the '" << channelName << "' L0DU Channel "
                  << " because the required '" << icond
                  << "' is neither a  defined ElementaryCondition nor a defined Channel." << endmsg;
          return StatusCode::FAILURE;
        }
        // YES it is - add all the conditions from this channel
        for ( const auto& iccc : icc->second->elementaryConditions() ) {
          channel->addElementaryCondition( iccc.second );
        }
      } else {
        channel->addElementaryCondition( ic->second );
      }
    }
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Created Channel : " << channel->description() << endmsg;

    m_channelsMap[channelName] = channel.release();
    id++;
  }
  return StatusCode::SUCCESS;
}

//===============================================================
StatusCode L0DUConfigProvider::createTriggers() {

  // crate channels -> conditions (-> compound data)
  StatusCode sc = createChannels();
  if ( sc.isFailure() ) return sc;

  if ( m_channels.empty() || m_conditions.empty() ) return StatusCode::SUCCESS;

  // pre-defined triggers
  predefinedTriggers();

  // Additional user-defined triggers
  int id = m_triggersMap.size();
  for ( auto iconfig = m_triggers.begin(); iconfig != m_triggers.end(); ++iconfig ) {

    // check all tags exist
    for ( const auto& tag : *iconfig ) {
      if ( std::none_of( s_trigFlags.begin(), s_trigFlags.end(),
                         [&]( const std::string& s ) { return contains_ci( tag, s ); } ) ) {
        error() << "Description tag : '" << tag
                << "' is unknown for the new TRIGGER set defined via options (num =  " << iconfig - m_triggers.begin()
                << ")" << endmsg;
        info() << "Allowed flags for new TRIGGER description are : " << s_trigFlags << endmsg;
        return StatusCode::FAILURE;
      }
    }

    // The Trigger name
    //------------------
    std::vector<std::string> values = Parse( "name", *iconfig );
    if ( values.size() != 1 ) {
      error() << "The SubTrigger defined via option (num = " << iconfig - m_channels.begin()
              << ") should have an unique name (found " << values.size() << ")" << endmsg;
      info() << "The syntax is ToolSvc.L0DUConfig.TCK_0xXXXX.Triggers +={ {''name=[TriggerName]'', "
             << " ''channels=[channelName1],[channelName2], ...'' } };" << endmsg;

      return StatusCode::FAILURE;
    }

    std::string triggerName = values.front(); // The NAME

    // Check if the triggers set already exists
    auto ic = m_triggersMap.find( triggerName );
    if ( ic != m_triggersMap.end() ) {
      warning() << "A L0DU SubTrigger with name  '" << triggerName << "' already exists "
                << " (NB : 'L0Ecal', 'L0Hcal', 'L0Muon' and 'Other' SubTrigger names are predefined)"
                << " - Please check your settings" << endmsg;
      return StatusCode::FAILURE;
    }

    // the index (facultatif)
    // ---------
    values = Parse( "index", *iconfig );
    if ( values.size() > 1 ) {
      error() << "Should be an unique index for the new SubTrigger: " << triggerName << " (found " << values.size()
              << ")" << endmsg;
      return StatusCode::FAILURE;
    }
    int index = ( values.empty() ? id : std::stoi( values.front() ) ); // The INDEX
    // check the index is not already used
    auto ii = std::find_if(
        m_triggersMap.begin(), m_triggersMap.end(),
        [index]( typename LHCb::L0DUTrigger::Map::const_reference tm ) { return tm.second->index() == index; } );
    if ( ii != m_triggersMap.end() ) {
      error() << "The bit index " << index << " is already assigned to the SubTrigger " << ii->second->name() << endmsg;
      return StatusCode::FAILURE;
    }

    // The decision type (facultatif)
    // -----------------
    std::vector<std::string> types = Parse( "type", *iconfig );
    int                      mask  = LHCb::L0DUDecision::Physics;
    if ( !types.empty() ) {
      const std::string& item = types.front();
      if ( icompare( item, "PHYSICS" ) )
        mask = LHCb::L0DUDecision::Physics;
      else if ( icompare( item, "BEAM1" ) )
        mask = LHCb::L0DUDecision::Beam1;
      else if ( icompare( item, "BEAM2" ) )
        mask = LHCb::L0DUDecision::Beam2;
      else {
        error() << "Trigger type '" << item << "' is not valid (must be PHYSICS, BEAM1 or BEAM2)" << endmsg;
        return StatusCode::FAILURE;
      }
    }

    // create trigger
    auto trigger = std::make_unique<LHCb::L0DUTrigger>( index, triggerName, mask );

    // The channels
    // --------------
    std::vector<std::string> channels = Parse( "channel", *iconfig );

    if ( channels.size() == 0 ) {
      error() << "The SubTrigger " << triggerName << " has no Channel" << endmsg;
      return StatusCode::FAILURE;
    }

    // check all requested channels exists
    for ( const auto& chan : channels ) {
      auto ic  = m_channelsMap.find( chan );
      auto icc = m_triggersMap.find( chan );
      if ( m_triggersMap.end() != icc && m_channelsMap.end() != ic ) {
        error() << "A Channel  and a SubTrigger have the same name - please check your setting " << endmsg;
        return StatusCode::FAILURE;
      }
      if ( m_channelsMap.end() == ic ) {
        if ( m_triggersMap.end() == icc || chan == triggerName ) {
          fatal() << " Can not set-up the '" << triggerName << "' L0DU SubTrigger "
                  << " because the required '" << chan << "' is neither a  defined Channel nor a defined SubTrigger."
                  << endmsg;
          return StatusCode::FAILURE;
        }
        for ( const auto& ccc : icc->second->channels() ) trigger->addChannel( ccc.second );
      } else {
        trigger->addChannel( ic->second );
      }
    }

    // check all channels->decisionType match the trigger->decisionType
    for ( const auto& c : trigger->channels() ) {
      LHCb::L0DUChannel* chan = c.second;
      if ( ( chan->decisionType() & trigger->decisionType() ) == 0 ) {
        warning() << "The channel '" << LHCb::L0DUDecision::Name[chan->decisionType()] << "|" << chan->name()
                  << "' associated to  subTrigger '" << LHCb::L0DUDecision::Name[trigger->decisionType()] << "|"
                  << trigger->name() << "' will be ignored in the subtrigger decision (decisionType mismatch)"
                  << endmsg;
      }
    }
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Created Trigger  : " << trigger->description() << endmsg;
    m_triggersMap[triggerName] = trigger.release();

    id++;
  } // end loop over subtrigger

  return StatusCode::SUCCESS;
}

//===============================================================
void L0DUConfigProvider::predefinedTriggers() {

  // create predefined triggers (decisionType = physics)
  for ( auto imap = s_tIndices.begin(); s_tIndices.end() != imap; imap++ ) {
    m_triggersMap[imap->first] = new LHCb::L0DUTrigger( imap->second, imap->first );
  }

  // Associate one channel to one (or several) trigger(s) according to elementary data (physics decisionType only)
  for ( const auto& c : m_channelsMap ) {
    LHCb::L0DUChannel* channel = c.second;
    if ( ( channel->decisionType() & LHCb::L0DUDecision::Physics ) == 0 ) continue;
    // loop over conditions
    std::vector<std::string> dataList;
    for ( const auto& ie : channel->elementaryConditions() ) {
      const LHCb::L0DUElementaryCondition* condition = ie.second;
      const LHCb::L0DUElementaryData*      data      = condition->data();
      if ( !getDataList( data->name(), dataList ) )
        Warning( "Cannot associate the data name '" + data->name() + "' to any (list of) L0DUElementaryData" ).ignore();
    }
    for ( const auto& n : triggerNameFromData( dataList ) ) {
      auto imap = m_triggersMap.find( n );
      if ( imap == m_triggersMap.end() ) {
        error() << " Unknow trigger name '" << n << "'" << endmsg;
        continue;
      }
      imap->second->addChannel( channel );
    }
  }
}

bool L0DUConfigProvider::getDataList( const std::string& dataName, std::vector<std::string>& dataList ) {
  auto it = m_dataMap.find( dataName );
  if ( it == m_dataMap.end() ) return false;

  LHCb::L0DUElementaryData* data = it->second;
  if ( LHCb::L0DUElementaryData::Type::Predefined == data->type() ) {
    dataList.push_back( data->name() );
    return true;
  }
  bool ok = true;
  if ( LHCb::L0DUElementaryData::Type::Compound == data->type() ) {
    ok = std::accumulate( data->componentsName().begin(), data->componentsName().end(), ok,
                          [&]( bool b, const std::string& op ) { return b && this->getDataList( op, dataList ); } );
  }
  return ok;
}

bool L0DUConfigProvider::conditionCheck( LHCb::L0DUElementaryCondition* condition ) {
  using namespace L0DUBase;

  if ( !condition ) return false;
  const LHCb::L0DUElementaryData* data = condition->data();
  if ( !data ) return false;

  // check the data consistency
  std::vector<std::string> dataList;
  if ( !getDataList( data->name(), dataList ) ) {
    warning() << "Cannot associate the data name '" << data->name() << "' to any (list of) L0DUElementaryData"
              << endmsg;
    return false;
  }

  int compoundID = CompoundData::None;
  int dataID     = -1;
  for ( const auto& il : dataList ) {
    auto it = m_dataMap.find( il );
    if ( it == m_dataMap.end() ) {
      warning() << "data '" << il << "' associated to condition '" << condition->name() << "' not found" << endmsg;
      return false;
    }

    LHCb::L0DUElementaryData* elData = it->second;
    if ( elData->type() != LHCb::L0DUElementaryData::Type::Predefined ) continue;
    dataID = elData->id();

    if ( dataID < 0 || dataID >= (int)NumberOf::Data ) {
      warning() << "data ID not correct : " << dataID << endmsg;
      return false;
    }
    int type = L0DUBase::PredefinedData::CompoundType[dataID];
    if ( compoundID == L0DUBase::CompoundData::None ) compoundID = type;
    if ( type != compoundID ) error() << "Compound data does not support inconsistent data" << endmsg;
  }

  //
  unsigned int max   = 0;
  int          order = -1;
  if ( LHCb::L0DUElementaryData::Type::RAMBcid == data->type() ) {
    order = L0DUBase::RAMBCID::ConditionOrder;
    max   = L0DUBase::RAMBCID::MaxNumber;
  } else if ( LHCb::L0DUElementaryData::Type::Compound == data->type() && compoundID != CompoundData::None ) {
    order = CompoundData::ConditionOrder[compoundID];
    max   = CompoundData::MaxNumber[compoundID];
  } else if ( LHCb::L0DUElementaryData::Type::Predefined == data->type() && dataID != -1 ) {
    order = PredefinedData::ConditionOrder[dataID];
    max   = PredefinedData::MaxNumber[dataID];
  }

  if ( order <= 0 ) {
    warning() << "Cannot associate a condition order to '" << condition->name() << "'" << endmsg;
    return false;
  }
  //  if( condition->threshold() == 0 && condition->comparator() == ">" ){ // special case for empty register
  //  auto it = m_condOrder[order].begin();
  //  m_condOrder[order].insert(it, condition);
  //}else{
  m_condOrder[order].push_back( condition );
  //  }
  m_condMax[order] = max;
  return true;
}

bool L0DUConfigProvider::conditionOrdering() {
  unsigned int iorder = 0;
  m_reOrder           = false;
  for ( const auto& conds : m_condOrder ) {
    for ( const auto& condition : conds ) {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << "Configuration " << format( "0x%04X", m_tckopts ) << " Condition '" << condition->name()
                << "'  index = " << condition->id() << "   order = " << iorder << endmsg;

      if ( condition->id() != iorder ) {
        m_reOrder = true;
        if ( !m_forceOrder ) condition->setId( iorder );
      }

      iorder++;
    }
  }

  if ( m_reOrder ) {
    if ( m_forceOrder )
      warning() << "The conditions ordering (that does not match the hardware) is FORCED" << endmsg;
    else
      info() << "The conditions have been re-ordered to match the hardware constraints" << endmsg;
  }

  if ( iorder != m_conditionsMap.size() ) {
    error() << "Failed to re-order all conditions " << iorder << "/" << m_conditionsMap.size() << endmsg;
    return false;
  }

  // set the reportBit
  unsigned int reportBit = 0;
  for ( unsigned int ib = 0; ib < m_conditionsMap.size(); ++ib ) {
    for ( auto& i : m_conditionsMap ) {
      LHCb::L0DUElementaryCondition* iCond = i.second;
      if ( iCond->id() != ib ) continue;
      if ( iCond->reported() ) {
        iCond->setReportBit( reportBit );
        reportBit++;
      } else {
        iCond->setReportBit( 0 );
      }
    }
  }
  m_reported = reportBit;

  // check all indices & all reportBits are unique
  for ( auto& i : m_conditionsMap ) {
    LHCb::L0DUElementaryCondition* iCond = i.second;
    for ( auto& j : m_conditionsMap ) {
      LHCb::L0DUElementaryCondition* jCond = j.second;
      if ( iCond == jCond ) continue;
      if ( iCond->id() == jCond->id() ) {
        error() << "The condition index MUST be unique : " << iCond->name() << "/" << jCond->name() << endmsg;
        return false;
      }
      if ( !iCond->reported() || !jCond->reported() ) continue;
      if ( iCond->reportBit() == jCond->reportBit() ) {
        error() << "The condition reportBit MUST be unique : " << iCond->name() << "/" << jCond->name() << endmsg;
        return false;
      }
    }
  }
  return true;
}

bool L0DUConfigProvider::configChecker() {

  if ( m_conditionsMap.empty() && m_channelsMap.empty() ) return true;

  using namespace L0DUBase;
  // check number of channels
  double chRate = (double)m_channelsMap.size() / (double)NumberOf::Channels;
  bool   ok     = true;

  if ( m_check )
    info() << "ConfigChecker : channels usage    : " << m_channelsMap.size() << " / " << NumberOf::Channels << "  ["
           << format( "%3.1f", 100. * chRate ) << "% ] " << endmsg;
  if ( chRate > 1. ) {
    warning() << "L0DU ConfigChecker : the number of channels exceeds the hardware capabilities "
              << m_channelsMap.size() << " / " << NumberOf::Channels << endmsg;
    ok = false;
  }

  // check number of conditions
  double cdRate = (double)m_conditionsMap.size() / (double)NumberOf::Conditions;
  if ( m_check )
    info() << "ConfigChecker : conditions usage  : " << m_conditionsMap.size() << " / " << NumberOf::Conditions << "  ["
           << format( "%3.1f", 100. * cdRate ) << "% ] " << endmsg;
  if ( cdRate > 1. ) {
    warning() << "L0DU ConfigChecker : the number of conditions exceeds the hardware capabilities "
              << m_conditionsMap.size() << " / " << NumberOf::Conditions << endmsg;
    ok = false;
  }
  if ( m_conditionsMap.size() > NumberOf::ConditionsInBank ) {
    warning() << "L0DU ConfigChecker : only " << NumberOf::ConditionsInBank << " condition-bits among the "
              << m_conditionsMap.size() << " conditions are stored in the L0DU bank from hardware" << endmsg;
  }
  if ( m_check )
    info() << "ConfigChecker : reported conditions : " << m_reported << " / " << m_conditionsMap.size()
           << " [max = " << NumberOf::ConditionsInBank << "]" << endmsg;

  // check number of conditions / type
  unsigned int k       = 0;
  double       maxRate = -1.;
  std::string  tCheck;
  for ( const auto& conds : m_condOrder ) {
    double ctRate = 0.;
    if ( conds.size() == 0 ) {
      k++;
      continue;
    }

    ctRate = ( m_condMax[k] > 0 ) ? (double)conds.size() / (double)m_condMax[k] : 999.;
    if ( ctRate >= maxRate ) {
      tCheck                                     = ( ctRate > maxRate ) ? "" : tCheck + "&";
      maxRate                                    = ctRate;
      const LHCb::L0DUElementaryCondition* cond  = conds.front();
      const LHCb::L0DUElementaryData*      data  = ( cond ? cond->data() : nullptr );
      std::string                          nData = ( data ? data->name() : "???" );
      tCheck += "[" + nData + " : " + Gaudi::Utils::toString( conds.size() ) + "/" +
                Gaudi::Utils::toString( m_condMax[k] ) + "]";
    }

    if ( m_check ) {
      std::string name = "??";
      if ( k == L0DUBase::RAMBCID::ConditionOrder ) {
        name = L0DUBase::RAMBCID::Name;
      } else {
        for ( unsigned int i = 0; i < L0DUBase::NumberOf::Data; ++i ) {
          if ( k == L0DUBase::PredefinedData::ConditionOrder[i] ) name = L0DUBase::PredefinedData::Name[i];
        }
        for ( unsigned int i = 0; i < L0DUBase::NumberOf::Compounds; ++i ) {
          if ( k == L0DUBase::CompoundData::ConditionOrder[i] ) name = L0DUBase::CompoundData::Name[i];
        }
      }
      info() << "ConfigChecker : condition usage  [type = " << name << "] :" << conds.size() << " / " << m_condMax[k]
             << " = [" << format( "%3.1f", 100. * ctRate ) << "% ] " << endmsg;
    }

    if ( ctRate > 1. ) {
      warning() << "ConfigChecker : number of conditions of type " << k << " exceeds the hardware capabilities "
                << conds.size() << " / " << m_condMax[k] << endmsg;
      ok = false;
    }
    k++;
  }

  //
  if ( m_check && m_reOrder ) {
    info() << "Hardware-like condition  ";
    if ( !m_forceOrder )
      info() << "ordering has been restored : " << endmsg;
    else
      info() << "ordering has NOT been applied : " << endmsg;
    int kk = 0;
    for ( const auto& conds : m_condOrder ) {
      for ( const auto& itt : conds ) {
        int bx = itt->bx();
        info() << " condition = '" << itt->name() << "' index = " << itt->id() << " |   hardware  " << kk++
               << " |  reported ? " << itt->reported() << " (reportBit  " << itt->reportBit() << ")";
        if ( bx != 0 ) info() << " applied to BX=[" << bx << "]";
        info() << endmsg;
      }
    }
  }

  std::string order = "OK";

  if ( m_reOrder ) {
    order = "SWAP";
    if ( m_forceOrder ) {
      ok = false;
      order += " (forced)";
    } else {
      order += " (restored)";
    }
  }
  for ( auto ic = m_conditionsMap.begin(); m_conditionsMap.end() != ic; ic++ ) {
    if ( ( std::find( m_knownBXs.begin(), m_knownBXs.end(), ic->second->bx() ) == m_knownBXs.end() ) ) ok = false;
  }
  if ( ok )
    info() << "The configuration " << format( "0x%04X", m_tckopts ) << " matches the hardware limitations " << endmsg;
  else
    warning() << "The configuration " << format( "0x%04X", m_tckopts ) << " DOES NOT match the hardware limitations "
              << endmsg;
  info() << "- Usage : #Channels   : " << m_channelsMap.size() << " [" << format( "%3.1f", 100. * chRate ) << "% ]    "
         << endmsg;
  info() << "- Usage : #Conditions : " << m_conditionsMap.size() << " [" << format( "%3.1f", 100. * cdRate )
         << "% ]; order : " << order << " ; reported  : " << m_reported << "/" << m_conditionsMap.size() << endmsg;
  info() << "- Usage : #Conditions/type (max)  :" << tCheck << endmsg;
  for ( const auto& ic : m_conditionsMap ) {
    if ( ic.second && ic.second->bx() != 0 ) {
      auto stamp = ( ( std::find( m_knownBXs.begin(), m_knownBXs.end(), ic.second->bx() ) == m_knownBXs.end() ) )
                       ? "Warning"
                       : "Info   ";
      info() << "- " << stamp << " : the condition '" << ic.first << "' relies on BX=["
             << Gaudi::Utils::toString( ic.second->bx() ) << "]" << endmsg;
    }
  }
  return ok;
}
