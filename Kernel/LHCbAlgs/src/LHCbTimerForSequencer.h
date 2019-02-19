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
// $Id: LHCbTimerForSequencer.h,v 1.5 2004/11/25 13:26:26 mato Exp $
#ifndef LHCBTIMERFORSEQUENCER_H

#  include "GaudiKernel/MsgStream.h"
#  include "GaudiKernel/Timing.h"

/** Auxilliary class. Measure the time between start and stop, and compute
 *  average, min and max. In fact, measure the cpu time, and the elapsed time
 *  but givesmin/max only for elapsed.
 *
 * @author O.Callot
 */

class LHCbTimerForSequencer {

public:
  /** Constructor. Specify the name, for later printing. */
  LHCbTimerForSequencer( const std::string& name, const unsigned int size, const double factor )
      : m_name( name ), m_size( size ), m_factor( factor ) {}

  /** Start a time measurement */
  inline void start() {
    m_startClock = System::currentTime( System::microSec );
    m_startCpu   = System::cpuTime( System::microSec );
  }

  /** Stop time measurement and return the last elapsed time.
      @return Measured time in ms
  */
  double stop();

  /** returns the name **/
  inline const std::string& name() const noexcept { return m_name; }

  /** returns the last measured time **/
  inline double lastTime() const noexcept { return m_lastTime; }

  /** returns the last measured time **/
  inline double lastCpu() const noexcept { return m_lastCpu; }

  /** returns the total elapsed time */
  inline double elapsedTotal() const noexcept { return m_sum; }

  /** returns the total cpu time */
  inline double cpuTotal() const noexcept { return m_sumCpu; }

  /** Returns the number run count*/
  inline double count() const noexcept { return (double)m_num; }

  /** Returns the min*/
  inline double min() const noexcept { return (double)m_min; }

  /** Returns the max*/
  inline double max() const noexcept { return (double)m_max; }

  /** Write measured time into the message stream. */
  MsgStream& fillStream( MsgStream& s ) const;

  /** header matching the previous format **/
  static std::string header( std::string::size_type size );

private:
  std::string  m_name;
  unsigned int m_size{0};
  double       m_factor{0};
  long long    m_startClock{0LL};
  long long    m_startCpu{0LL};

  long long m_num{0LL};
  double    m_lastTime{0};
  double    m_lastCpu{0};
  double    m_min{0};
  double    m_max{0};
  double    m_sum{0};
  double    m_sumCpu{0};
};

inline MsgStream& operator<<( MsgStream& ms, const LHCbTimerForSequencer& count ) { return count.fillStream( ms ); }

#endif // LHCBTIMERFORSEQUENCER_H
