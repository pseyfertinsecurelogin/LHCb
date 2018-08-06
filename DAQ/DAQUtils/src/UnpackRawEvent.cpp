#include "GaudiKernel/ParsersFactory.h"
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include <vector>
#include <algorithm>

namespace Gaudi { namespace Parsers {

    template <typename Iterator, typename Skipper>
    struct RawBankTypeGrammar : qi::grammar<Iterator, LHCb::RawBank::BankType(), Skipper> {
      typedef LHCb::RawBank::BankType ResultT;
      RawBankTypeGrammar() : RawBankTypeGrammar::base_type( literal )
      {
        for (int i=0; i<LHCb::RawBank::BankType::LastType; ++i) {
            literal |= ( qi::lit( LHCb::RawBank::typeName( LHCb::RawBank::BankType(i) ) ) )[qi::_val = i];
        }
      }
      qi::rule<Iterator, ResultT(), Skipper> literal;
    };
    REGISTER_GRAMMAR(LHCb::RawBank::BankType, RawBankTypeGrammar) ;

}}

#include "GaudiAlg/SplittingTransformer.h"

class UnpackRawEvent : public Gaudi::Functional::SplittingTransformer<
                                 std::vector<LHCb::RawBank::ConstVector>(const LHCb::RawEvent&)
                              > {
public:
    UnpackRawEvent( const std::string& name, ISvcLocator* locator );
    std::vector<LHCb::RawBank::ConstVector> operator()(const LHCb::RawEvent& evt) const override;
private:
    Gaudi::Property<std::vector<LHCb::RawBank::BankType>> m_types { this, "BankTypes" };
};

UnpackRawEvent::UnpackRawEvent( const std::string& name, ISvcLocator* locator)
: SplittingTransformer( name, locator, { "RawEventLocation", "/Event/DAQ/RawEvent" },
                                       { "RawBankLocations", { } } )
{}

std::vector<LHCb::RawBank::ConstVector>
UnpackRawEvent::operator()(const LHCb::RawEvent& evt) const
{
    std::vector<LHCb::RawBank::ConstVector> banks;
    banks.reserve( m_types.size() );
    //TODO: move the mapping done in 'RawEvent::banks' to this code, and
    //      remove it from 'RawEvent' itself, which allows 'RawEvent' to
    //      be const without mutable
    std::transform( m_types.begin(), m_types.end(), std::back_inserter(banks),
                    [&evt](LHCb::RawBank::BankType t)
                    { const auto& bnks =  evt.banks(t);
                      return LHCb::RawBank::ConstVector{ bnks.begin(), bnks.end() };
                    } );
    return banks;
}

DECLARE_COMPONENT( UnpackRawEvent )

