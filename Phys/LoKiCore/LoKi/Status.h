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
#ifndef LOKI_STATUS_H
#define LOKI_STATUS_H 1
// ============================================================================
// Include files
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/IAccept.h"
#include "Kernel/ICheckTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Interface.h"
// ============================================================================
/** @file Loki/Status.h
 *  collection of "checkers" deaing with generic tools
 *  @see ICheckTool
 *  @see IAcceptTool
 *
 *  This file is part of LoKi project:
 *   ``C++ ToolKit for Smart and Friendly Physics Analysis''
 *
 *  @author Vanya Belyaev Ivan.Belyaev@nikhef.nl
 *  @date   2010-08-02
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::Status
   *  Collection of "checkers"-functors dealing with generic tools
   *  @author Vanya Belyaev Ivan.Belyaev@nikhef.nl
   *  @date   2010-08-02
   */
  namespace Status {
    // ========================================================================
    /** @class Check     LoKi/Status.h
     *  Simple checker to "check" using ICheckTool
     *  @see ICheckTool
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @date 2010-08-03
     */
    class Check : public LoKi::Functor<void, bool> {
    public:
      // ======================================================================
      /// constructor from tool nickname
      Check( const std::string& nikname );
      /// MANDATORY: virtual destructor
      ~Check() override;
      /// MANDATORY: clone method ("virtual constructor")
      Check* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()() const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      Check(); // the default constructor is disabled
      // ======================================================================
    protected:
      // ======================================================================
      void getTool() const;
      // ======================================================================
    private:
      // ======================================================================
      /// tool nickname
      std::string m_nickname; // the tool nickname
      /// the checker
      mutable LoKi::Interface<ICheckTool> m_tool; // the checker
      // ======================================================================
    };
    // ========================================================================
    /** @class Accept LoKi/Status.h
     *  Simple checker to "check" using IAccept
     *  @see IAccept
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @date 2010-08-03
     */
    class Accept : public LoKi::Functor<void, bool> {
    public:
      // ======================================================================
      /// constructor from tool nickname
      Accept( const std::string& nikname );
      /// MANDATORY: virtual destructor
      ~Accept() override;
      /// MANDATORY: clone method ("virtual constructor")
      Accept* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()() const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      Accept(); // the default constructor is disabled
      // ======================================================================
    protected:
      // ======================================================================
      void getTool() const;
      // ======================================================================
    private:
      // ======================================================================
      /// tool nickname
      std::string m_nickname; // the tool nickname
      /// the checker
      mutable LoKi::Interface<IAccept> m_tool; // the checker
      // ======================================================================
    };
    // ========================================================================
  } // namespace Status
  // ==========================================================================
  namespace Cuts {
    // ========================================================================
    /** @typedef ACCEPT
     *  Trivial checker/function to "check"
     *  @see IAccept
     *  @see LoKi::Status::Accept
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-08-03
     */
    using ACCEPT = LoKi::Status::Accept;
    // ========================================================================
    /** @typedef CHECK
     *  Trivial checker/function to "check"
     *  @see ICheckTool
     *  @see LoKi::Status::Check
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-08-03
     */
    using CHECK = LoKi::Status::Check;
    // ========================================================================
  } // namespace Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_STATUS_H
