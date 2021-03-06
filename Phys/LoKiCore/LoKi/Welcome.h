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
#ifndef LOKI_WELCOME_H
#define LOKI_WELCOME_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <atomic>
#include <iostream>
#include <string>
#include <vector>
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
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class Welcome Welcome.h LoKi/Welcome.h
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2004-02-23
   */
  class Welcome : protected std::vector<int> {
  public:
    // ========================================================================
    /// get the static instance of class LoKi::Welcome
    static const Welcome& instance();
    // ========================================================================
  public:
    // ========================================================================
    /// destructor
    ~Welcome();
    // ========================================================================
  public:
    // ========================================================================
    void welcome() const;
    void goodbye() const;
    void welcome( std::ostream& stream ) const;
    void goodbye( std::ostream& stream ) const;
    // ========================================================================
  protected:
    // ========================================================================
    /// Standard constructor
    Welcome();
    // ========================================================================
  private:
    // ========================================================================
    size_t                   m_len1;
    std::string              m_str1;
    std::string              m_fmt1;
    std::string              m_fmt2;
    std::string              m_fmt3;
    mutable std::atomic<int> m_printed{0};
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
namespace Bender {
  // ==========================================================================
  /** @class BenderWelcome
   *  welcome for Bender application
   *  @author Vanya BELYAEV@physics.syr.edu
   *  @date 2007-08-15
   */
  class Welcome {
  public:
    // ========================================================================
    /// get the static instance of class Bender::Welcome
    static const Welcome& instance();
    // ========================================================================
  public:
    // ========================================================================
    /// destructor
    ~Welcome();
    // ========================================================================
  public:
    // ========================================================================
    void welcome() const;
    void goodbye() const;
    void welcome( std::ostream& s ) const;
    void goodbye( std::ostream& s ) const;
    // ========================================================================
  protected:
    // ========================================================================
    /// Standard constructor
    Welcome();
    // ========================================================================
  private:
    // ========================================================================
    size_t                   m_len1;
    std::string              m_str1;
    std::string              m_fmt1;
    std::string              m_fmt2;
    std::string              m_fmt3;
    mutable std::atomic<int> m_printed{0};
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Bender
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_WELCOME_H
