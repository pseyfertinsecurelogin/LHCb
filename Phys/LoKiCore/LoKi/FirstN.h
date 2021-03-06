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
// ============================================================================
#ifndef LOKI_FIRSTN_H
#define LOKI_FIRSTN_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2011-10-30
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class FirstN
   *  helper class to implement the selection of first N-elements in Hybdrids
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-20
   */
  class GAUDI_API FirstN final {
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    explicit FirstN( const unsigned int N ); // constructor
  public:
    // ========================================================================
    /// get N
    unsigned int N() const { return m_N; } // get N
    /// get N
    unsigned int n() const { return m_N; } // get N
    /// printout
    virtual std::ostream& fillStream( std::ostream& s ) const;
    // ========================================================================
  private:
    // ========================================================================
    /// number of elements to be selected
    unsigned int m_N; // number of elements to be selected
    // ========================================================================
  }; //                                               end of class LoKi::FirstN
  // ==========================================================================
  /** @class FirstN
   *  helper class to implement the selection of first N-elements in Hybrids
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-20
   */
  class GAUDI_API Reverse final {
  public:
    // ========================================================================
    /// constructor
    Reverse() = default; // constructor
    // ========================================================================
    /// printout
    std::ostream& fillStream( std::ostream& s ) const;
    // ========================================================================
  };
  // ==========================================================================
  /** @class Sort
   *  helper class to implement the sort and
   *  selection of first N-elements in Hybdrids
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-20
   */
  class GAUDI_API Sort final {
  public:
    // ========================================================================
    /// constructor
    Sort( const int  N         = -1, // ALL
          const bool ascending = true );
    // ========================================================================
    /// printout
    std::ostream& fillStream( std::ostream& s ) const;
    // ========================================================================
  public:
    // ========================================================================
    bool ascending() const { return m_ascending; }
    bool all() const { return m_N < 0; }
    int  N() const { return m_N; }
    int  n() const { return m_N; }
    // ========== =============================================================
  private:
    // ========================================================================
    /// number of elements
    int m_N; // number of elements
    /// ascending order ?
    bool m_ascending; // ascending order ?
    // ========================================================================
  };
  // ==========================================================================
  /// output operator
  // ==========================================================================
  std::ostream& operator<<( std::ostream& s, const FirstN& o );
  // ==========================================================================
  /// output operator
  // ==========================================================================
  std::ostream& operator<<( std::ostream& s, const Reverse& o );
  // ==========================================================================
  /// output operator
  // ==========================================================================
  std::ostream& operator<<( std::ostream& s, const Sort& o );
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
namespace Gaudi {
  // ==========================================================================
  namespace Utils {
    // ========================================================================
    // Dump as C++ objects
    GAUDI_API std::string toCpp( const LoKi::FirstN& f );
    GAUDI_API std::string toCpp( const LoKi::Sort& s );
    GAUDI_API std::string toCpp( const LoKi::Reverse& r );
    // ========================================================================
  } // namespace Utils
  // ==========================================================================
} // namespace Gaudi
// ============================================================================
#endif // LOKI_FIRSTN_H
