#ifndef BANKKILLER_H
#define BANKKILLER_H 1

// STL
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "DAQKernel/DecoderAlgBase.h"
// from Event
#include "Event/RawEvent.h"

namespace bankKiller_details {
   struct bankType_t {
       LHCb::RawBank::BankType type;

       friend bool operator==(const bankType_t& lhs, const LHCb::RawBank::BankType& rhs)
       { return lhs.type == rhs; }

       friend StatusCode parse(bankType_t& bt, const std::string& in) {
          for( int ibank = 0 ; ibank < (int)LHCb::RawBank::LastType ; ++ibank){
            if (in==LHCb::RawBank::typeName(static_cast<LHCb::RawBank::BankType>(ibank))) {
                bt.type = static_cast<LHCb::RawBank::BankType>(ibank);
                return StatusCode::SUCCESS;
            }
          }
          return StatusCode::FAILURE;
       }

       friend std::string toString(const bankType_t& t) {
           return LHCb::RawBank::typeName( t.type );
       }

       friend std::ostream& toStream(const bankType_t& t, std::ostream& os) {
           return os << std::quoted(toString(t),'\'');
       }
   };

   template <typename InnerType>
   StatusCode parse(std::vector<InnerType>& v, const std::string& in) {
        v.clear();
        using Gaudi::Parsers::parse;
        std::vector<std::string> vs;
        auto sc = parse(vs,in);
        if (!sc) return sc;
        v.reserve(vs.size());
        try { std::transform( vs.begin(), vs.end(), std::back_inserter(v),
                              [](const std::string& s) {
                                  InnerType t{};
                                  auto sc = parse(t,s);
                                  if (!sc) throw GaudiException("Bad Parse","", sc);
                                  return t;
                              } );
        } catch (const GaudiException& ge) { return ge.code(); }
        return StatusCode::SUCCESS;
   }
}

/** @class bankKiller bankKiller.h
 *
 *   Killer of raw event banks.
 *
 *   Input:
 *    - BankTypes : list of bank names to be killed or saved
 *    - DefaultIsKill : Main behaviour switch. If false (default), kill only given banks. If true, kill all BUT given banks.
 *
 *    @author: Olivier Deschamps
 *    @date:   O6 Septembre 2006
 */

class bankKiller : public Decoder::AlgBase {
 public:

  bankKiller( const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;
  StatusCode execute   () override;

 private:
  ///. kill all banks of given type
  void  killBankType(LHCb::RawEvent*, LHCb::RawBank::BankType bankType,
		             bool warningmsg ) const;

  ///. kill all banks with the current m_rawEvent
  StatusCode  killFromRawEvent(LHCb::RawEvent*) const;

  Gaudi::Property<std::vector<bankKiller_details::bankType_t>> m_bankTypes { this, "BankTypes", {}, "List of bank names" };  ///< List of bank names
  /// Main behaviour switch. If false (default), kill only given banks. If true, kill all BUT given banks.
  Gaudi::Property<bool> m_defaultIsKill { this, "DefaultIsKill" , false, "Main behaviour switch. If false (default), kill only given banks. If true, kill all BUT given banks." } ;
  //kill from all raw event locations, or just from the first one?
  Gaudi::Property<bool> m_killFromAll { this, "KillFromAll" , false, "Main behaviour switch. If false (default), kill only banks in the first location found in the search string. If false, kill *all* banks found in the search string." } ;
  Gaudi::Property<int> m_sourceKill { this, "KillSourceID", 0, "Kill bank type with specific source IDs, see KillSourceIDMask."} ;
  Gaudi::Property<int> m_sourceKillMask { this, "KillSourceIDMask", 0, "AND mask for sourceID before comparison with KillSourceID."} ;
};
#endif //    BANKKILLER_H
