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
#ifndef LOKI_PIDFUNCTIONS_H
#define LOKI_PIDFUNCTIONS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
#include <vector>
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/ParticleID.h"
#include "Kernel/ParticleProperty.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/AuxFunBase.h"
// ============================================================================
namespace LoKi {
  // =========================================================================
  /** @namespace LoKi::PidFunctions PidFunctions.h LoKi/PidFunctions.h
   *  Collections of funtors dealing wioth LHCb::ParticleID
   *  @see LHCb::ParticleID
   *  @see LoKi::Particles
   *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
   *  @date   2015-01-18
   */
  namespace Pids {
    // ========================================================================
    /** @class GetPids
     *  helper class to get PIDs
     *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
     *  @date   2015-01-18
     */
    class GAUDI_API GetPids : public virtual LoKi::AuxFunBase {
    public:
      // ======================================================================
      GetPids( const long id );
      GetPids( const unsigned long id );
      GetPids( const LHCb::ParticleID& id );
      GetPids( const std::string& id );
      GetPids( const std::vector<int>& ids );
      GetPids( const std::vector<long>& ids );
      GetPids( const std::vector<unsigned int>& ids );
      GetPids( const std::vector<unsigned long>& ids );
      GetPids( const std::vector<LHCb::ParticleID>& ids );
      GetPids( const std::vector<std::string>& ids );
      // ======================================================================
    public:
      // ======================================================================
      bool in_list( const long id ) const;
      bool not_in_list( const long id ) const { return !in_list( id ); }
      bool in_list( const LHCb::ParticleID& id ) const { return in_list( id.pid() ); }
      bool not_in_list( const LHCb::ParticleID& id ) const { return !in_list( id ); }
      // ======================================================================
      bool in_abs_list( const long id ) const;
      bool not_in_abs_list( const long id ) const { return !in_abs_list( id ); }
      bool in_abs_list( const LHCb::ParticleID& id ) const { return in_abs_list( id.pid() ); }
      bool not_in_abs_list( const LHCb::ParticleID& id ) const { return !in_abs_list( id ); }
      // ======================================================================
    private:
      // ======================================================================
      // load data (if needed)
      StatusCode getData() const;
      // ======================================================================
    protected:
      // ======================================================================
      mutable std::vector<long> m_ints;
      std::vector<std::string>  m_names;
      // ======================================================================
    };
    // ========================================================================
  } // namespace Pids
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_PIDFUNCTIONS_H
