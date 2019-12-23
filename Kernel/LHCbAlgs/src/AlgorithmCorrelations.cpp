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
#include "Kernel/IAlgorithmCorrelations.h" // Interface
#include <boost/format.hpp>
#include <math.h>

//-----------------------------------------------------------------------------
// Implementation file for class : AlgorithmCorrelations
//
// 2005-04-19 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

/** @class AlgorithmCorrelations AlgorithmCorrelations.h
 *
 *  Tool to print a table of correlations of Bernoullian variables. See
 *  Interface doxygen.
 *  Options for this implementation, ff one wants to declare the algorithm directly to the tool (ignoring
 *    the corresponding methods):
 *  @code
 *      Algorithms = { "alg1", "alg2"... }
 *      AlgorithmsRow, optional, for non-square matrices. Defines headers for columns
 *  @endcode
 *    Other options:
 *  @code
 *      OnlyNonZero = true ; // skips algorithms with eff=0 in table
 *      Decimals = -1 ; // defines precision. -1 means automatic precision.
 *      UseNumbers = true ;  // labels columns by numbers
 *  @endcode
 *
 *  @author Patrick KOPPENBURG
 *  @date   2005-04-19
 */
class AlgorithmCorrelations : public extends<GaudiTool, IAlgorithmCorrelations> {
  class AlgoMatrix;
  class AlgoResult;

public:
  typedef std::vector<std::string> strings;

public:
  /// Standard constructor
  AlgorithmCorrelations( const std::string& type, const std::string& name, const IInterface* parent );

  StatusCode initialize() override;
  StatusCode algorithms( const strings& ) override;
  StatusCode algorithmsRow( const strings& ) override;
  StatusCode printTable() override;
  StatusCode printList() override;
  ///< Fill results one by one for each algorithm
  StatusCode fillResult( const std::string& algo, const bool& result ) override;
  ///< Actually tell the tool that we reached the end of the event (only for one-by-one filling)
  StatusCode endEvent() override;

private:
  StatusCode   reset();                                     ///< reset everything
  bool         isEffective( const std::string& ) const;     ///< Algo did something
  double       algoRate( const std::string& ) const;        ///< Algo did something
  int          happyAlgorithms() const;                     ///< Number of algos that did something
  unsigned int getDecimals() const;                         ///< Number of significant decimals
  StatusCode   addResults();                                ///< add all results
  StatusCode   resetAlgoResult( std::vector<AlgoResult>& ); ///< reset algo Results
  StatusCode   fillResult( const std::string&, const bool&, std::vector<AlgoResult>& ); ///< fill results
  StatusCode   testAlgos( const strings& ) const;

private:
  Gaudi::Property<strings> m_conditionAlgorithms{this, "AlgorithmsRow", {}, "Algorithms to check against"};
  Gaudi::Property<strings> m_algorithmsToTest{this, "Algorithms", {}}; ///< Algorithms to check
  std::vector<AlgoMatrix>  m_AlgoMatrices;                             ///< Pairs of correlations
  std::vector<AlgoResult>  m_conditionResults;                         ///< results of algorithms in this event
  std::vector<AlgoResult>  m_testResults;                              ///< results of algorithms in this event

  Gaudi::Property<bool> m_minimize{this, "OnlyNonZero", true, "Use mimimal table width"};
  Gaudi::Property<int>  m_decimals{this, "Decimals", -1, "Number of decimals"};
  Gaudi::Property<bool> m_useNumbers{this, "UseNumbers", true, "use numbers as column labels"};

  unsigned int       m_longestName{10}; ///< Longest algorithm name
  bool               m_square{false};   ///< it is a square matrix
  unsigned long long m_nEvents{0};      ///< number of events

  // container of results for one algorithm
  class AlgoResult final {

  public:
    AlgoResult() = default;
    AlgoResult( const std::string& algo ) : m_algo( algo ) {}

    const std::string& algo() const { return m_algo; }
    bool               result() const { return m_result; }
    StatusCode         setResult( const bool& b ) {
      if ( m_updated ) return StatusCode::FAILURE;
      m_result = b;
      return StatusCode::SUCCESS;
    }
    void setAlgo( const std::string& a ) { m_algo = a; }
    void reset() {
      m_result  = false;
      m_updated = false;
    } ///< Reset for next event

  private:
    std::string m_algo{"UNDEFINED"}; ///< Algo name
    bool        m_result{false};     ///< Result in this event
    bool        m_updated{false};    ///< Already in this event?
  };

  // container of Algos matrix for two algorithms
  class AlgoMatrix final {
  public:
    /// Standard constructor
    AlgoMatrix() = default;

    /// Useful constructor
    AlgoMatrix( const std::string& a1, const std::string& a2 ) : m_algorithm1( a1 ), m_algorithm2( a2 ) {}

    /// Add Algo
    void addConditionalResult( const bool& r1, const bool& r2 ) {
      if ( r2 ) { ++m_alg2passed; }
      if ( r1 && r2 ) { ++m_bothpassed; }
    }

    /// Return full statistics
    unsigned long long getConditionalStatistics() const { return m_bothpassed; }

    unsigned long long getFullStatistics() const { return m_alg2passed; }

    /// Return full statistics
    double getConditionalFraction() const {
      return ( m_alg2passed > 0 ? double( m_bothpassed ) / double( m_alg2passed ) : -1. );
    }

    double getConditionalPercent() const { return 100. * getConditionalFraction(); }

    /// Return algorithm names
    void getAlgorithms( std::string& a1, std::string& a2 ) const {
      a1 = m_algorithm1;
      a2 = m_algorithm2;
    }

  private:
    std::string m_algorithm1; ///< Algorithm in row
    std::string m_algorithm2; ///< Algorithm in column
    /// statistics
    unsigned long long m_bothpassed{0}; ///< both passed
    unsigned long long m_alg2passed{0}; ///< alg 2 passed
  };
};

// Declaration of the Tool Factory

DECLARE_COMPONENT( AlgorithmCorrelations )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
AlgorithmCorrelations::AlgorithmCorrelations( const std::string& type, const std::string& name,
                                              const IInterface* parent )
    : extends( type, name, parent ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode AlgorithmCorrelations::initialize() {
  return extends::initialize().andThen( [&]() -> StatusCode {
    if ( !m_algorithmsToTest.empty() )
      return reset(); // options are active
    else
      return StatusCode::SUCCESS;
  } );
}
//=============================================================================
// Set matrix
//=============================================================================
StatusCode AlgorithmCorrelations::reset() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Reset" << endmsg;

  if ( m_algorithmsToTest.empty() ) {
    err() << "No algorithms defined. Use Algorithms option, or algorithms method before algorithmsRow." << endmsg;
    return StatusCode::FAILURE;
  }

  m_AlgoMatrices.clear();
  m_conditionResults.clear();
  m_testResults.clear();

  info() << "Algorithms to check correlations: " << m_algorithmsToTest << endmsg;
  if ( !testAlgos( m_algorithmsToTest ) ) return StatusCode::FAILURE;
  if ( m_conditionAlgorithms.empty() ) {
    info() << "No algorithms row defined. Will make a square matrix using algorithms column." << endmsg;
    m_square              = true;
    m_conditionAlgorithms = m_algorithmsToTest; // copy one in the other
  } else {
    info() << "Algorithms to check correlations against: " << m_conditionAlgorithms << endmsg;
    if ( !testAlgos( m_conditionAlgorithms ) ) return StatusCode::FAILURE;
    m_square = false;
  }

  // minimize?
  if ( m_minimize.value() ) warning() << "Will only print non-zero efficient algorithms in table" << endmsg;

  if ( m_decimals >= 0 )
    info() << "Will print efficiencies with " << m_decimals << " decimals (percent)" << endmsg;
  else
    info() << "Will print efficiencies with automatic precision" << endmsg;

  //  int i1 = 0 ;

  m_conditionResults.reserve( m_conditionAlgorithms.size() );
  for ( const auto& alg : m_conditionAlgorithms ) { m_conditionResults.emplace_back( alg ); }

  m_testResults.reserve( m_algorithmsToTest.size() );
  m_AlgoMatrices.reserve( m_algorithmsToTest.size() * m_conditionAlgorithms.size() );
  for ( const auto& alg1 : m_algorithmsToTest ) {
    m_testResults.emplace_back( alg1 );
    const auto l = alg1.length();
    if ( l > m_longestName ) { m_longestName = l; }
    for ( const auto& alg2 : m_conditionAlgorithms ) {
      m_AlgoMatrices.emplace_back( alg1, alg2 );
      if ( msgLevel( MSG::DEBUG ) ) debug() << "Pushed back pair " << alg1 << ", " << alg2 << endmsg;
    }
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
// end of event - do the work
//=============================================================================
StatusCode AlgorithmCorrelations::endEvent() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "End of event... do the work" << endmsg;
  ++m_nEvents;
  auto sc = addResults();
  if ( !sc ) return sc;
  sc = resetAlgoResult( m_conditionResults );
  if ( !sc ) return sc;
  sc = resetAlgoResult( m_testResults );
  return sc;
}
//=============================================================================
// Clear for next event
//=============================================================================
StatusCode AlgorithmCorrelations::resetAlgoResult( std::vector<AlgoResult>& Results ) {
  for ( auto& r : Results ) { r.reset(); }
  return StatusCode::SUCCESS;
}
//=============================================================================
// Add all results to matrix elements
//=============================================================================
StatusCode AlgorithmCorrelations::addResults() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Put all results in matrices" << endmsg;
  // fill statistics
  auto isrm = m_AlgoMatrices.begin();
  for ( const auto& ar1 : m_testResults ) {
    for ( const auto& ar2 : m_conditionResults ) {
      if ( isrm == m_AlgoMatrices.end() ) { return Error( "Reached end of matrices" ); }
      std::string a1, a2;
      isrm->getAlgorithms( a1, a2 );
      if ( ( a1 != ar1.algo() ) || ( a2 != ar2.algo() ) ) {
        err() << "Confused! I see " << a1 << " and " << a2 << " and expect " << ar1.algo() << " and " << ar2.algo()
              << endmsg;
        return StatusCode::FAILURE;
      }
      if ( a2 == "ALWAYS" ) {
        isrm->addConditionalResult( ar1.result(), true );
        if ( msgLevel( MSG::VERBOSE ) )
          verbose() << "A1 " << a1 << " has now " << isrm->getFullStatistics() << " in " << m_nEvents << endmsg;
      } else if ( a1 == "ALWAYS" ) {
        isrm->addConditionalResult( true, ar2.result() );
        if ( msgLevel( MSG::VERBOSE ) )
          verbose() << "A2 " << a2 << " has now " << isrm->getConditionalStatistics() << " in " << m_nEvents << endmsg;
      } else {
        isrm->addConditionalResult( ar1.result(), ar2.result() );
        verbose() << "Added result " << ar1.result() << " " << ar2.result() << " for " << a1 << " " << a2
                  << " and have now " << isrm->getConditionalFraction() << " of " << isrm->getFullStatistics()
                  << endmsg;
      }
      ++isrm;
    }
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
// Fill a result - entry point
//=============================================================================
StatusCode AlgorithmCorrelations::fillResult( const std::string& algo, const bool& result ) {
  return StatusCode{fillResult( algo, result, m_conditionResults ) && fillResult( algo, result, m_testResults )};
}
//=============================================================================
// Fill a result
//=============================================================================
StatusCode AlgorithmCorrelations::fillResult( const std::string& algo, const bool& result,
                                              std::vector<AlgoResult>& Results ) {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "fillResult " << algo << " " << result << endmsg;
  bool found = false;
  for ( auto& ar : Results ) {
    if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Checking algorithm " << ar.algo() << endmsg;
    if ( ( ar.algo() == algo ) || ( "/Event/Phys/" + ar.algo() == algo ) ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << "Updating algorithm " << ar.algo() << endmsg;
      const auto sc = ar.setResult( result );
      if ( !sc ) {
        return Error( "Resetting result for algorithm " + ar.algo() +
                      " in same event. Are you missing a m_AlgorithmCorrelations->endEvent()?" );
      }
      found = true;
      break;
    }
  }
  if ( found ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Algorithm " << algo << " has been found" << endmsg;
  } else {
    if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Algorithm " << algo << " has not been found" << endmsg;
  }

  return StatusCode::SUCCESS;
}
//=============================================================================
// Set algorithms
//=============================================================================
StatusCode AlgorithmCorrelations::algorithms( const strings& algos ) {
  m_algorithmsToTest = algos;
  m_algorithmsToTest.value().insert( m_algorithmsToTest.begin(), "ALWAYS" );
  return reset();
}
//=============================================================================
StatusCode AlgorithmCorrelations::algorithmsRow( const strings& algos ) {
  m_conditionAlgorithms = algos;
  m_conditionAlgorithms.value().insert( m_conditionAlgorithms.begin(), "ALWAYS" );
  return reset();
}
//=============================================================================
//  Test
//=============================================================================
StatusCode AlgorithmCorrelations::testAlgos( const strings& algos ) const {
  for ( auto i1 = algos.begin(); i1 != algos.end(); ++i1 ) {
    if ( std::find( std::next( i1 ), algos.end(), *i1 ) != algos.end() ) {
      err() << "Duplicate instance of " << std::quoted( *i1 ) << endmsg;
      err() << "Fix you options" << endmsg;
      return StatusCode::FAILURE;
    }
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
//  Print a list
//=============================================================================
StatusCode AlgorithmCorrelations::printList() {

  info() << "Correlation list is ugly but useful with grep" << endmsg;

  boost::format rate( "%1$6.4f" );
  boost::format algo( "%1$-25S" );

  always() << "Correlation list:\n";
  for ( const auto& srm : m_AlgoMatrices ) {
    std::string a1, a2;
    srm.getAlgorithms( a1, a2 );
    if ( isEffective( a2 ) ) {
      const auto y12 = srm.getConditionalFraction();
      always() << algo % a1;
      always() << " is accepted with " << rate % y12;
      always() << " probability when " << algo % a2;
      always() << " is true\n";
    }
  }
  always() << endmsg;
  ;
  return StatusCode::SUCCESS;
}
//=============================================================================
//  Print a table
//=============================================================================
StatusCode AlgorithmCorrelations::printTable( void ) {

  const unsigned int nalgow   = happyAlgorithms();
  const unsigned int decimals = getDecimals();

  if ( msgLevel( MSG::DEBUG ) ) debug() << nalgow << " columns find something " << endmsg;

  const unsigned int nl         = 4 + 9 + decimals + m_longestName + ( 6 + decimals ) * nalgow;
  const unsigned int namelength = name().length();
  const double       halflength = ( (double)nl - namelength ) / 2.;
  const unsigned int nlh        = ( halflength > 0 ? int( halflength ) : 0 );
  if ( msgLevel( MSG::VERBOSE ) )
    verbose() << name() << " namelength: " << namelength << " decimals: " << decimals << " longest: " << m_longestName
              << " nl: " << nl << " halflength: " << halflength << " nlh: " << nlh << endmsg;
  std::string halfspace = "";
  if ( nlh > 0 ) halfspace = std::string( nlh, ' ' );
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Will print table with " << nl << " " << nlh << endmsg;

  const std::string hyphenline( nl, '-' );
  const std::string equalline( nl, '=' );
  const std::string crosses = std::string( "  " ) + std::string( 4 + decimals, '*' );
  const std::string hashes  = std::string( "  " ) + std::string( 4 + decimals, '#' );

  std::string   mdec  = std::to_string( decimals );
  std::string   mdec2 = std::to_string( 4 + decimals );
  std::string   mdec3 = std::to_string( 5 + decimals );
  boost::format percent( " %1$" + mdec2 + "." + mdec + "f" );
  std::string   mln = std::to_string( m_longestName );
  boost::format algo( "%1$-" + mln + "S " );        // longest name left aligned
  boost::format number( "  %1$=" + mdec2 + "d" );   // number of decimals+4 length
  boost::format smallnumber( " %1$2i " );           // number of 2 positions
  boost::format empty( "    " );                    // empty string
  boost::format shortalgo( "%1$=" + mdec3 + "S " ); // string decimals+5 in length, centered

  always() << "Correlation Table:\n\n";
  // header
  always() << equalline << "\n";
  always() << halfspace << name() << "\n";
  always() << equalline << "\n";
  always() << "    " << algo % "Algorithm"
           << "   Eff.  ";

  if ( !m_useNumbers.value() ) always() << std::string( decimals, ' ' ); // don't ask why I need this

  unsigned int i = 0;
  for ( const auto& a : m_conditionAlgorithms ) {
    if ( a == "ALWAYS" ) continue;
    if ( ( isEffective( a ) ) || ( !m_minimize.value() ) ) {
      if ( m_useNumbers.value() ) {
        ++i;
        always() << number % i;
      } else {
        always() << shortalgo % ( a.size() < 5 + decimals ? a : a.substr( 0, 5 + decimals ) );
      }
    }
  }

  always() << "\n";
  always() << hyphenline;
  unsigned int ia1 = 0;

  // the table
  bool doprint = true;
  bool addline = false;
  for ( const auto& srm : m_AlgoMatrices ) {
    std::string a1, a2;
    srm.getAlgorithms( a1, a2 );
    doprint = ( ( !m_minimize.value() ) || isEffective( a1 ) );
    if ( ( a1 == "ALWAYS" ) && ( m_square ) ) continue; // no efficiency if square
    /*
     * efficiency row
     */
    if ( a2 == "ALWAYS" ) {
      if ( addline ) {
        always() << "\n" << hyphenline;
        addline = false;
      }
      if ( a1 == "ALWAYS" ) {
        always() << "\n"
                 << "    " << algo % "Efficiency" << crosses << " |";
        addline = true;
      } else {
        if ( doprint ) {
          ++ia1; // increment
          always() << "\n";
          always() << smallnumber % ia1 << algo % a1;
          const auto y12 = srm.getConditionalPercent();
          always() << percent % y12 << "% |";
        }
      }
      /*
       * standard
       */
    } else if ( doprint || ( a1 == "ALWAYS" ) ) {
      if ( ( m_minimize.value() ) && ( !isEffective( a2 ) ) ) continue;
      if ( a1 == a2 )
        always() << hashes;
      else {
        auto y12 = srm.getConditionalPercent();
        if ( a1 == "ALWAYS" ) y12 = 100.0 * srm.getFullStatistics() / (double)m_nEvents;
        if ( y12 < 0 )
          always() << crosses;
        else
          always() << percent % y12 << "%";
      }
    }
  }
  always() << "\n" << equalline << "\n";
  if ( ( !m_square ) && ( m_useNumbers.value() ) ) {
    unsigned int j = 0;
    always() << "Column labels are : \n";
    for ( const auto& a : m_conditionAlgorithms ) {
      if ( a == "ALWAYS" ) continue;
      if ( ( m_minimize.value() ) && !( isEffective( a ) ) ) continue;
      ++j;
      always() << smallnumber % j << algo % a << "\n";
    }
  }

  always() << endmsg;

  if ( msgLevel( MSG::VERBOSE ) ) {
    printList().ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

    for ( const auto& srm : m_AlgoMatrices ) {
      std::string a1, a2;
      srm.getAlgorithms( a1, a2 );
      verbose() << a1 << " " << a2 << " " << srm.getConditionalStatistics() << " " << srm.getFullStatistics() << " "
                << m_nEvents << " " << algoRate( a1 ) << " " << algoRate( a2 ) << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
//  number of algos that did something
//=============================================================================
int AlgorithmCorrelations::happyAlgorithms( void ) const {

  if ( !m_minimize.value() ) return m_conditionAlgorithms.size();
  int nalgow = 0;
  for ( const auto& a : m_conditionAlgorithms ) {
    if ( ( isEffective( a ) ) && ( a != "ALWAYS" ) ) ++nalgow;
    if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Algorithm " << a << " says: " << isEffective( a ) << endmsg;
  }
  return nalgow;
}

//=============================================================================
//  Number of significant decimals
//=============================================================================
unsigned int AlgorithmCorrelations::getDecimals( void ) const {
  if ( m_decimals >= 0 ) return m_decimals;

  unsigned int maxevt = 1;
  for ( const auto& srm : m_AlgoMatrices ) {
    const auto sum = srm.getFullStatistics();
    if ( sum > maxevt ) maxevt = sum;
  }
  double minerror  = 100. / maxevt;
  double signif    = -std::log10( minerror ) + 1;
  int    IntSignif = int( signif );
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Minimal error is " << minerror << "% -> gets " << IntSignif << endmsg;
  return ( IntSignif <= 0 ? 0 : IntSignif );
}

//=============================================================================
//  Algo did something
//=============================================================================
bool AlgorithmCorrelations::isEffective( const std::string& name ) const { return ( algoRate( name ) > 0 ); }

//=============================================================================
//  Algo did something
//=============================================================================
double AlgorithmCorrelations::algoRate( const std::string& name ) const {
  for ( const auto& srm : m_AlgoMatrices ) {
    std::string a1, a2;
    srm.getAlgorithms( a1, a2 );
    if ( ( a1 == name ) && ( a2 == "ALWAYS" ) ) return srm.getConditionalFraction(); // standard efficiency
    if ( ( a2 == name ) && ( a1 == "ALWAYS" ) ) {
      return ( m_nEvents > 0 ? (double)srm.getFullStatistics() / (double)m_nEvents : 0 );
    }
  }
  Error( "Could not find algorithm " + name ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  return -1;
}
