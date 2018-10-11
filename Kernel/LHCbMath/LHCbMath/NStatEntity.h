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
// $Id$ 
// ============================================================================
#ifndef LHCBMATH_NSTATENTITY_H 
#define LHCBMATH_NSTATENTITY_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatEntity.h"
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    /** @class NStatEntity LHCbMath/NStatEntity.h
     *  A good and efficient approximation to the running
     *  statistic for last N-events.Actually statiustic is calcualetd for 
     *  last n-events, where   N<= n < 2*N.
     *  Implemeneted as two sliding counters, each of them is reset 
     *  for every 2*N events.
     *  Something similar I've written long time ago for Velo.
     *  It can be useful for e.g. "current rate" counters 
     *  @see StatEntity 
     *  @see Gaudi::Math::SStatEntity 
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2015-04-03
     */
    class GAUDI_API NStatEntity 
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor with N-parameter  
      NStatEntity ( const unsigned long N = 1000 ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get N, such that statistic is calculated for n-events  N<= n < 2*N 
      unsigned long N () const { return m_N ; }
      // ======================================================================
    public: // conversion to the counter with longer history: N<= n < 2*N 
      // ======================================================================
      /// get the counter with longer history 
      inline const StatEntity& counter () const 
      { return m_cnt1.nEntries() > m_cnt2.nEntries() ? m_cnt1 : m_cnt2 ; }
      /// implicit cast to counters 
      inline operator const StatEntity&() const { return counter () ; }
      // ======================================================================
    public: // mimic regular counter interface, the basic quantities
      // ======================================================================
      /// numbner of entries (  N <= n < 2*N ) 
      unsigned long nEntries      () const { return counter().nEntries     () ; }
      /// accumulated value
      double        sum           () const { return counter().sum          () ; }
      /// accumulated value**2
      double        sum2          () const { return counter().sum2         () ; }
      /// mean value of counter
      double        mean          () const { return counter().mean         () ; }
      /// r.m.s of value
      double        rms           () const { return counter().rms          () ; }
      /// error in mean value of counter
      double        meanErr       () const { return counter().meanErr      () ; }
      /// minimal value
      double        min           () const { return counter().min          () ; }
      /// maximal value
      double        max           () const { return counter().max          () ; }
      // efficiency 
      double        efficiency    () const { return counter().efficiency   () ; }
      // efficiency error
      double        efficiencyErr () const { return counter().efficiencyErr() ; }
      // ======================================================================
    public: // increment and decrement 
      // ======================================================================
      /// pre-increment
      NStatEntity& operator++ ()    { return addFlag (  1 ) ; }
      /// post-increment (actually udentical to pre-increment 
      NStatEntity& operator++ (int) { return addFlag (  1 ) ; }
      /// pre-decrement
      NStatEntity& operator-- ()    { return addFlag ( -1 ) ; }
      /// post-decrement (actually udentical to pre-decrement 
      NStatEntity& operator-- (int) { return addFlag ( -1 ) ; }
      // ======================================================================
    public: // another form of increment/decrement 
      // ======================================================================
      /// another form of increment 
      NStatEntity& operator+= ( const double   f ) { return addFlag (  f ) ; }
      /// another form of decrement 
      NStatEntity& operator-= ( const double   f ) { return addFlag ( -f ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /** printout  to std::ostream
       *  @param s the reference to the output stream
       */
      std::ostream& fillStream ( std::ostream& o ) const 
      { return counter().fillStream ( o ) ; }
      // =====================================================================
      /// reset method (likely not needed at all) 
      void reset() 
      {
        m_cnt1.reset() ;
        m_cnt2.reset() ;        
      }
      // =====================================================================
   public: // the main method without decorations  
      // ======================================================================
      /// the main method without decorations  
      NStatEntity& addFlag ( const double f ) 
      {
        /// inncrement both counters 
        m_cnt1.addFlag ( f ) ;
        m_cnt2.addFlag ( f ) ;
        // reset them when needed 
        if ( 0    == m_cnt1.nEntries() % ( 2 * m_N ) ) { m_cnt1.reset() ; }
        if ( m_N  == m_cnt1.nEntries() % ( 2 * m_N ) ) { m_cnt2.reset() ; }
        //
        return *this ;
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the first  counter 
      StatEntity    m_cnt1 ;                            // the first  counter 
      /// the second counter 
      StatEntity    m_cnt2 ;                            // the second counter 
      /// the sliding window 
      unsigned long m_N    ;                            // the sliding window 
      // ======================================================================
    };
    // ========================================================================
    /// printout operator to std::ostream
    inline std::ostream& operator<<
    ( std::ostream&      stream ,
      const NStatEntity& entity ) { return entity.fillStream ( stream ) ; }
    // ========================================================================
    /// operator for addition of NStatEntity and a number
    inline NStatEntity operator+( const NStatEntity& entity , 
                                  const double      value  ) 
    { 
      NStatEntity n ( entity ) ;
      n.addFlag ( value ) ; 
      return n ;
    }
    /// operator for addition of NStatEntity and a number
    inline NStatEntity operator+( const double       value  , 
                                  const NStatEntity& entity ) 
    { return entity + value ; }
    /// operator for addition of NStatEntity and a number
    inline NStatEntity operator-( const NStatEntity& entity , 
                                  const double      value  ) 
    { return entity + (-1*value) ; }
    // ========================================================================
  } //                                             end of namespace Gaudi::Math 
  // ==========================================================================
} //                                                     end of namespace Gaudi 
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LHCBMATH_NSTATENTITY_H
// ============================================================================
