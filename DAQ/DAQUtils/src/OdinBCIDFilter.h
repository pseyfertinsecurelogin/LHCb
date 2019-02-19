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
// $Id: OdinBCIDFilter.h,v 1.1 2008/10/28 11:20:32 odescham Exp $
#ifndef COMPONENT_ODINBCIDFILTER_H
#define COMPONENT_ODINBCIDFILTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

namespace OdinBCIDFilter_details {

  enum class Comparator_t { eq, neq, lt, le, gt, ge };

  const char* toString( Comparator_t cmp ) {
    switch ( cmp ) {
    case Comparator_t::eq:
      return "==";
    case Comparator_t::neq:
      return "!=";
    case Comparator_t::lt:
      return "<";
    case Comparator_t::le:
      return "<=";
    case Comparator_t::gt:
      return ">";
    case Comparator_t::ge:
      return ">=";
    }
    throw "IMPOSSIBLE!";
  }

  std::ostream& toStream( Comparator_t cmp, std::ostream& os ) { return os << std::quoted( toString( cmp ), '\'' ); }

  StatusCode parse( Comparator_t& cmp, const std::string& in ) {
    for ( Comparator_t ref : {Comparator_t::eq, Comparator_t::neq, Comparator_t::lt, Comparator_t::le, Comparator_t::gt,
                              Comparator_t::ge} ) {
      if ( in != toString( ref ) ) continue;
      cmp = ref;
      return StatusCode::SUCCESS;
    }
    if ( in == "=" ) {
      cmp = Comparator_t::eq;
      return StatusCode::SUCCESS;
    }
    return StatusCode::FAILURE;
  }

} // namespace OdinBCIDFilter_details

/** @class OdinBCIDFilter OdinBCIDFilter.h component/OdinBCIDFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-02-05
 */
class OdinBCIDFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  // Default : filtering odd-parity BXID
  Gaudi::Property<long>                                 m_mask{this, "Mask", 0x1};
  Gaudi::Property<long>                                 m_value{this, "Value", 1};
  Gaudi::Property<OdinBCIDFilter_details::Comparator_t> m_comparator{this, "Comparator",
                                                                     OdinBCIDFilter_details::Comparator_t::eq};
  Gaudi::Property<bool>                                 m_revert{this, "Revert", false};

  std::atomic<long> m_all{0l};
  std::atomic<long> m_acc{0l};
};
#endif // COMPONENT_ODINBCIDFILTER_H
