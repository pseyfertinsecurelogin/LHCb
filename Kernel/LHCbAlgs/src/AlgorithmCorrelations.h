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
#ifndef ALGORITHMCORRELATIONS_H
#define ALGORITHMCORRELATIONS_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/IAlgorithmCorrelations.h" // Interface

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
class AlgorithmCorrelations : public GaudiTool, virtual public IAlgorithmCorrelations {
  class AlgoMatrix;
  class AlgoResult;

public:
  typedef std::vector<std::string> strings;

public:
  /// Standard constructor
  AlgorithmCorrelations( const std::string& type, const std::string& name, const IInterface* parent );

  virtual ~AlgorithmCorrelations() = default; ///< Destructor
  StatusCode initialize() override;
  StatusCode algorithms( const strings& ) override;
  StatusCode algorithmsRow( const strings& ) override;
  StatusCode printTable( void ) override;
  StatusCode printList( void ) override;
  ///< Fill results one by one for each algorithm
  StatusCode fillResult( const std::string& algo, const bool& result ) override;
  ///< Actually tell the tool that we reached the end of the event (only for one-by-one filling)
  StatusCode endEvent() override;

private:
  StatusCode   reset( void );                               ///< reset everything
  bool         isEffective( const std::string& ) const;     ///< Algo did something
  double       algoRate( const std::string& ) const;        ///< Algo did something
  int          happyAlgorithms( void ) const;               ///< Number of algos that did something
  unsigned int getDecimals( void ) const;                   ///< Number of significant decimals
  StatusCode   addResults( void );                          ///< add all results
  StatusCode   resetAlgoResult( std::vector<AlgoResult>& ); ///< reset algo Results
  StatusCode   fillResult( const std::string&, const bool&, std::vector<AlgoResult>& ); ///< fill results
  StatusCode   testAlgos( const strings& ) const;

private:
  strings                 m_conditionAlgorithms; ///< Algorithms to check against
  strings                 m_algorithmsToTest;    ///< Algorithms to check
  std::vector<AlgoMatrix> m_AlgoMatrices;        ///< Pairs of correlations
  std::vector<AlgoResult> m_conditionResults;    ///< results of algorithms in this event
  std::vector<AlgoResult> m_testResults;         ///< results of algorithms in this event

  unsigned int       m_longestName{10}; ///< Longest algorithm name
  bool               m_minimize;        ///< Use mimimal table width
  int                m_decimals;        ///< Number of decimals
  bool               m_square{false};   ///< it is a square matrix
  bool               m_useNumbers;      ///< use numbers as column labels
  unsigned long long m_nEvents{0};      ///< number of events

  // container of results for one algorithm
  class AlgoResult {

  public:
    AlgoResult() = default;
    AlgoResult( const std::string& algo ) : m_algo( algo ) {}
    ~AlgoResult() = default;

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
  class AlgoMatrix {
  public:
    /// Standard constructor
    AlgoMatrix() = default;

    /// Useful constructor
    AlgoMatrix( const std::string& a1, const std::string& a2 ) : m_algorithm1( a1 ), m_algorithm2( a2 ) {}

    /// Standard destructor
    ~AlgoMatrix() = default;

    /// Add Algo
    inline void addConditionalResult( const bool& r1, const bool& r2 ) {
      if ( r2 ) { ++m_alg2passed; }
      if ( r1 && r2 ) { ++m_bothpassed; }
    }

    /// Return full statistics
    inline unsigned long long getConditionalStatistics() const { return m_bothpassed; }

    inline unsigned long long getFullStatistics() const { return m_alg2passed; }

    /// Return full statistics
    inline double getConditionalFraction() const {
      return ( m_alg2passed > 0 ? double( m_bothpassed ) / double( m_alg2passed ) : -1. );
    }

    inline double getConditionalPercent() const { return 100. * getConditionalFraction(); }

    /// Return algorithm names
    inline void getAlgorithms( std::string& a1, std::string& a2 ) const {
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
#endif // ALGORITHMCORRELATIONS_H
