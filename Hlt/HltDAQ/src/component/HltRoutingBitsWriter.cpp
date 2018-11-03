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
// Include files
// from Boost
#include <boost/format.hpp>
#include "boost/algorithm/string/join.hpp"

// from Gaudi
#include "GaudiKernel/IIncidentSvc.h"
#include "AIDA/IHistogram1D.h"

// Event
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include "Event/HltDecReports.h"
#include "Event/L0DUReport.h"
#include "Event/ODIN.h"

// Hlt Interfaces
#include "Kernel/RateCounter.h"

// local
#include "HltRoutingBitsWriter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : HltRoutingBitsWriter
//
// 2008-07-29 : Gerhard Raven
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltRoutingBitsWriter )

//=============================================================================
StatusCode HltRoutingBitsWriter::decode() {
   zeroEvaluators();

   // Create the right type of evaluator and build it
   auto build = [this](const unsigned int bit, const std::string expr) -> StatusCode {
      if (expr.empty()) return StatusCode::SUCCESS;

      std::string title = boost::str(boost::format("%02d:%s") % bit % expr);
      std::string htitle = boost::str(boost::format("RoutingBit%02d") % bit);

      decltype(m_evaluators)::iterator it;
      bool placed{false};
      if (bit < 8) {
         std::tie(it, placed) = m_evaluators.emplace(bit, ODINEval{m_odin_location});
      } else if (bit < 32) {
         std::tie(it, placed) = m_evaluators.emplace(bit, L0Eval{m_l0_location});
      } else if (bit < 64) {
         std::tie(it, placed) = m_evaluators.emplace(bit, HltEval{m_hlt_location[0]});
      } else {
         std::tie(it, placed) = m_evaluators.emplace(bit, HltEval{m_hlt_location[1]});
      }
      assert(placed && it->first == bit);
      auto builder = Builder{this, expr, title, htitle};
      return boost::apply_visitor(builder, it->second);
   };

   // Build the routing bits
   for (const auto& i : m_bits) {
      if (i.first > nBits) return StatusCode::FAILURE;
      auto sc = build(i.first, i.second);
      if (!sc.isSuccess()) return sc;
   }

   m_evals_updated      = false;
   m_preambulo_updated = false;
   return StatusCode::SUCCESS;
}

//=============================================================================
void HltRoutingBitsWriter::zeroEvaluators() {
   Deleter deleter;
   for (auto& entry : m_evaluators) {
      boost::apply_visitor(deleter, entry.second);
   }
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode HltRoutingBitsWriter::execute() {
   StatusCode sc;
   if (m_evals_updated || m_preambulo_updated) {
      sc = decode();
      if (!sc.isSuccess()) return Error(" Unable to Decode ???? ", std::move(sc));
   }

   // Get the fill time, weight and event time
   double t = 0, weight = 0, evt_time = 0;
   sc = times(t, weight, evt_time);
   if (!sc.isSuccess()) return sc;

   // Create the evaluator
   Evaluator evaluator{this, t, weight, evt_time};

   // The routing bits
   std::vector<unsigned int> bits(3,0);

   // Evaluate the routing bits
   for (auto& entry : m_evaluators) {
      auto result = boost::apply_visitor(evaluator, entry.second);
      auto bit = entry.first;
      int word = bit / 32;
      if (result) bits[word] |= (0x01UL << (bit - 32 * word));
   }

   // Get the raw event and update or add the routing bits bank.
   LHCb::RawEvent* rawEvent = get<LHCb::RawEvent>(m_raw_location);

   if (m_updateBank) {
      const auto& banks = rawEvent->banks(LHCb::RawBank::HltRoutingBits);
      if (banks.size()!=1) {
         return Error(" Multiple RoutingBits RawBanks -- don't know which to update. Skipping... ",
                      StatusCode::SUCCESS, 20);
      }
      const LHCb::RawBank *bank = banks[0];
      if (bank->size()!=3*sizeof(unsigned int) ) {
         return Error(" RoutingBits RawBanks has unexpected size.. Skipping",
                      StatusCode::SUCCESS, 20);
      }
      const unsigned int *data = bank->data();
      if (data[0]!=bits[0] || data[1]!=bits[1]) {
         Warning(" RoutingBits RawBanks: requested to update bank, but first two entries not the same" ,
                 StatusCode::SUCCESS, 20).ignore();
      }
      if (data[2]!=0 ) {
         Warning(" RoutingBits RawBanks: requested to update bank, but non-zero third entry",
                 StatusCode::SUCCESS, 20).ignore();
      }
      const_cast<unsigned int*>(data)[2] = bits[2];
   } else {
      if (!rawEvent->banks( LHCb::RawBank::HltRoutingBits).empty()) {
         Warning(" Pre-existing RoutingBits bank in the event...",
                 StatusCode::SUCCESS, 0).ignore();
      }
      rawEvent->addBank(0, LHCb::RawBank::HltRoutingBits, 0, bits);
   }

   return StatusCode::SUCCESS;
}
