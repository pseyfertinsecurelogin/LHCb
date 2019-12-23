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
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/ISequencerTimerTool.h"
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/RndmGenerators.h"
#include "GaudiUtils/Aida2ROOT.h"
#include "LHCbTimerForSequencer.h"
#include "TH1D.h"
#include "boost/algorithm/string/predicate.hpp"
#include <fstream>

//-----------------------------------------------------------------------------
// Implementation file for class : LHCbSequencerTimerTool
//
// 2004-05-19 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class LHCbSequencerTimerTool LHCbSequencerTimerTool.h
 *  Implements the time measurement inside a sequencer
 *
 *  The width of the timing table column printing the algorithm name
 *  is 30 by default. That can be changed via
 *  \verbatim
 TimingAuditor().addTool(LHCbSequencerTimerTool, name = "TIMER")
 TimingAuditor().TIMER.NameSize = 50 \endverbatim
 *
 *  @author Olivier Callot
 *  @date   2004-05-19
 */

class LHCbSequencerTimerTool final : public extends<GaudiHistoTool, ISequencerTimerTool> {

public:
  using ISequencerTimerTool::name;
  using ISequencerTimerTool::start;
  using ISequencerTimerTool::stop;

public:
  /// Standard constructor
  LHCbSequencerTimerTool( const std::string& type, const std::string& name, const IInterface* parent );

  /** initialize method, to compute the normalization factor **/
  StatusCode initialize() override;

  /** finalize method, to print the time summary table **/
  StatusCode finalize() override;

  /** add a timer entry with the specified name **/
  int addTimer( const std::string& name ) override;

  /** Increase the indentation of the name **/
  void increaseIndent() override { m_indent += 2; }

  /** Decrease the indentation of the name **/
  void decreaseIndent() override {
    m_indent -= 2;
    if ( 0 > m_indent ) m_indent = 0;
  }

  /** start the counter, i.e. register the current time **/
  void start( int index ) override { m_timerList[index].start(); }

  /** stop the counter, return the elapsed time **/
  double stop( int index ) override { return m_timerList[index].stop(); }

  /** returns the last time **/
  double lastTime( int index ) override { return m_timerList[index].lastTime(); }

  /** returns the name of the counter **/
  const std::string& name( int index ) override { return m_timerList[index].name(); }

  /** returns the index of the counter with that name, or -1 **/
  int indexByName( const std::string& name ) override;

  /** returns the flag telling that global timing is wanted **/
  bool globalTiming() override { return m_globalTiming.value(); }

  /** prepares and saves the timing histograms **/
  void saveHistograms() override;

  /** saves the output to a file **/
  StatusCode fileIO();

private:
  Gaudi::Property<int>  m_shots{this, "shots",
                               3500000}; ///< Number of shots for CPU normalization // 1s on 2.8GHz Xeon, gcc 3.2, -O2
  Gaudi::Property<bool> m_normalised{this, "Normalised", false}; ///< Is the time scaled to a nominal PIII ?
  int                   m_indent{0};                             ///< Amount of indentation
  std::vector<LHCbTimerForSequencer>      m_timerList;
  double                                  m_normFactor{0.001}; ///< Factor to convert to standard CPU (1 GHz PIII)
  double                                  m_speedRatio{0};
  Gaudi::Property<bool>                   m_globalTiming{this, "GlobalTiming", false};
  Gaudi::Property<std::string::size_type> m_headerSize{
      this, "NameSize", 30, "Number of characters to be used in algorithm name column"}; ///< Size of the name field
  Gaudi::Property<std::string> m_summaryFile{this, "SummaryFile", ""}; ///< Whether to output also to a file
  std::string                  m_sep; ///< Separator to use in fileIO, defined by extension of the file
};

// Declaration of the Tool Factory
DECLARE_COMPONENT( LHCbSequencerTimerTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
LHCbSequencerTimerTool::LHCbSequencerTimerTool( const std::string& type, const std::string& name,
                                                const IInterface* parent )
    : extends( type, name, parent ) {
  // Histograms are disabled by default in this tool.
  setProperty( "HistoProduce", false ).ignore();
}

//=========================================================================
//
//=========================================================================
StatusCode LHCbSequencerTimerTool::initialize() {
  const StatusCode sc = extends::initialize();
  if ( sc.isFailure() ) return sc;
  LHCbTimerForSequencer norm( "normalize", m_headerSize, m_normFactor );
  norm.start();
  IRndmGenSvc* rsvc = svc<IRndmGenSvc>( "RndmGenSvc", true );
  { // Use dummy loop suggested by Vanya Belyaev:
    double        sum = 0;
    Rndm::Numbers gauss;
    gauss.initialize( rsvc, Rndm::Gauss( 0., 1.0 ) ).ignore();
    unsigned int shots = m_shots;
    while ( 0 < --shots ) { sum += gauss() * sum; }
  }
  norm.stop();
  const double time = norm.lastCpu();
  m_speedRatio      = 1. / time;
  info() << "This machine has a speed about " << format( "%6.2f", 1000. * m_speedRatio )
         << " times the speed of a 2.8 GHz Xeon." << endmsg;
  if ( m_normalised.value() ) { m_normFactor = m_speedRatio; }
  if ( !m_summaryFile.empty() ) {
    if ( boost::algorithm::ends_with( m_summaryFile.value(), ".csv" ) )
      m_sep = ", ";
    else if ( boost::algorithm::ends_with( m_summaryFile.value(), ".dat" ) )
      m_sep = "\t";
    else
      return Error( "Unknown file type " + m_summaryFile.value() +
                        " please use .csv (comma separated) or .dat (tab separated)",
                    StatusCode::FAILURE );
  }

  return sc;
}

//=========================================================================
//  Finalize : Report timers
//=========================================================================
StatusCode LHCbSequencerTimerTool::finalize() {

  std::string line( m_headerSize + 68, '-' );
  info() << line << endmsg << "This machine has a speed about " << format( "%6.2f", 1000. * m_speedRatio )
         << " times the speed of a 2.8 GHz Xeon.";
  if ( m_normalised.value() ) info() << " *** All times are renormalized ***";
  info() << endmsg << LHCbTimerForSequencer::header( m_headerSize ) << endmsg << line << endmsg;

  std::string lastName;
  for ( unsigned int kk = 0; kk < m_timerList.size(); ++kk ) {
    if ( lastName == m_timerList[kk].name() ) continue; // suppress duplicate
    lastName = m_timerList[kk].name();
    info() << m_timerList[kk] << endmsg;
  }
  info() << line << endmsg;
  StatusCode sc = StatusCode::SUCCESS;

  // new, do fileIO if requested
  if ( m_summaryFile.size() ) sc = fileIO();
  if ( sc.isFailure() ) Error( "Could not write to output file " + m_summaryFile, StatusCode::FAILURE ).ignore();

  return GaudiHistoTool::finalize();
}

StatusCode LHCbSequencerTimerTool::fileIO() {
  // try open file
  std::ofstream thefile;
  thefile.open( m_summaryFile );

  if ( !thefile or !thefile.is_open() ) return StatusCode::FAILURE;

  // header line
  thefile << "Depth" << m_sep << "Algorithm" << m_sep << "<user> / ms" << m_sep << "<clock> / ms" << m_sep << "min / ms"
          << m_sep << "max / ms" << m_sep << "entries" << m_sep << "total (s)";

  for ( auto i : m_timerList ) {
    thefile << std::endl;
    std::string s = i.name();
    // trim from the right
    s.resize( s.find_last_not_of( ' ' ) + 1 );
    // depth, first count the number of spaces, yeah, I know.
    auto        mycount = s.find_first_not_of( ' ' );
    std::string depth( mycount, '|' );
    // then trim from the start
    s = s.substr( s.find_first_not_of( ' ' ) );
    thefile << depth << '+' << m_sep << s << m_sep;
    double ave = 0.;
    double cpu = 0.;

    if ( 0 != i.count() ) {
      ave = i.elapsedTotal() / i.count();
      cpu = i.cpuTotal() / i.count();
    }
    thefile << cpu << m_sep << ave << m_sep << i.min() << m_sep << i.max() << m_sep << i.count() << m_sep
            << i.cpuTotal() * 0.001;
  }
  thefile << std::endl;
  thefile.close();
  return StatusCode::SUCCESS;
}

//=========================================================================
//  Return the index of a specified name. Trailing and leading spaces ignored
//=========================================================================
int LHCbSequencerTimerTool::indexByName( const std::string& name ) {
  std::string::size_type beg  = name.find_first_not_of( " \t" );
  std::string::size_type end  = name.find_last_not_of( " \t" );
  std::string            temp = name.substr( beg, end - beg + 1 );
  for ( unsigned int kk = 0; m_timerList.size() > kk; kk++ ) {
    beg = m_timerList[kk].name().find_first_not_of( " \t" );
    end = m_timerList[kk].name().find_last_not_of( " \t" );
    if ( m_timerList[kk].name().substr( beg, end - beg + 1 ) == temp ) return kk;
  }
  return -1;
}

//=========================================================================
//  Build and save the histograms
//=========================================================================
void LHCbSequencerTimerTool::saveHistograms() {
  if ( produceHistos() ) {
    info() << "Saving Timing histograms" << endmsg;
    const size_t        bins       = m_timerList.size();
    AIDA::IHistogram1D* histoTime  = book( "ElapsedTime", 0, bins, bins );
    AIDA::IHistogram1D* histoCPU   = book( "CPUTime", 0, bins, bins );
    AIDA::IHistogram1D* histoCount = book( "Count", 0, bins, bins );
    TH1D*               tHtime     = Gaudi::Utils::Aida2ROOT::aida2root( histoTime );
    TH1D*               tHCPU      = Gaudi::Utils::Aida2ROOT::aida2root( histoCPU );
    TH1D*               tHCount    = Gaudi::Utils::Aida2ROOT::aida2root( histoCount );
    for ( size_t kk = 0; bins > kk; kk++ ) {
      LHCbTimerForSequencer& tfsq = m_timerList[kk];
      tHtime->Fill( tfsq.name().c_str(), tfsq.elapsedTotal() );
      tHCPU->Fill( tfsq.name().c_str(), tfsq.cpuTotal() );
      tHCount->Fill( tfsq.name().c_str(), tfsq.count() );
    }
  }
}

//=============================================================================
// Add a timer
//=============================================================================
int LHCbSequencerTimerTool::addTimer( const std::string& name ) {
  std::string myName;
  if ( 0 < m_indent ) {
    const std::string prefix( m_indent, ' ' );
    myName += prefix;
  }
  myName += name;
  if ( myName.size() < m_headerSize ) {
    const std::string space( m_headerSize - myName.size(), ' ' );
    myName += space;
  }

  // myName = myName.substr( 0, m_headerSize );

  m_timerList.emplace_back( myName, m_headerSize, m_normFactor );

  return m_timerList.size() - 1;
}

//=============================================================================
