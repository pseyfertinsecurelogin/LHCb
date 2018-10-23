/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// $Id: SequencerTimerTool.h,v 1.7 2005/07/29 16:49:43 hmd Exp $
#ifndef LHCBSEQUENCERTIMERTOOL_H
#define LHCBSEQUENCERTIMERTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/ISequencerTimerTool.h"

// local
#include "LHCbTimerForSequencer.h"


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

class LHCbSequencerTimerTool final : public GaudiHistoTool,
                                     virtual public ISequencerTimerTool
{

public:

  using ISequencerTimerTool::start;
  using ISequencerTimerTool::stop;
  using ISequencerTimerTool::name;

public:

  /// Standard constructor
  LHCbSequencerTimerTool( const std::string& type,
                          const std::string& name,
                          const IInterface* parent);

  virtual ~LHCbSequencerTimerTool( ) = default; ///< Destructor

  /** initialize method, to compute the normalization factor **/
  StatusCode initialize() override;

  /** finalize method, to print the time summary table **/
  StatusCode finalize() override;

  /** add a timer entry with the specified name **/
  int addTimer( const std::string& name ) override;

  /** Increase the indentation of the name **/
  void increaseIndent() override { m_indent += 2; }

  /** Decrease the indentation of the name **/
  void decreaseIndent() override
  {
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
  bool globalTiming() override { return m_globalTiming; }

  /** prepares and saves the timing histograms **/
  void saveHistograms() override;

  /** saves the output to a file **/
  StatusCode fileIO();

private:

  int m_shots;       ///< Number of shots for CPU normalization
  bool m_normalised; ///< Is the time scaled to a nominal PIII ?
  int m_indent{0};   ///< Amount of indentation
  std::vector<LHCbTimerForSequencer> m_timerList;
  double m_normFactor{0.001}; ///< Factor to convert to standard CPU (1 GHz PIII)
  double m_speedRatio{0};
  bool   m_globalTiming;
  std::string::size_type m_headerSize;   ///< Size of the name field
  std::string m_summaryFile; ///< Whether to output also to a file
  std::string m_sep; ///< Separator to use in fileIO, defined by extension of the file

};

#endif // LHCbSEQUENCERTIMERTOOL_H
