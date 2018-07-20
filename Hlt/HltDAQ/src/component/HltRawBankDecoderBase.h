#ifndef HLTRAWBANKDECODERBASE_H
#define HLTRAWBANKDECODERBASE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/SplittingTransformer.h"
#include "DAQKernel/DecoderAlgBase.h"

#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/IANNSvc.h"
#include "Kernel/IIndexedANNSvc.h"
#include "Event/RawEvent.h"
#include "Event/RawBank.h"

class HltRawBankDecoderBase : public Decoder::AlgBase {
public:
    enum SourceIDs {
        kSourceID_Dummy=0,
        kSourceID_Hlt=kSourceID_Dummy,
        kSourceID_Hlt1=1,
        kSourceID_Hlt2=2,
        kSourceID_Max=7,
        kSourceID_BitShift=13,
        kSourceID_MinorMask=0x1FFF,
        kSourceID_MajorMask=0xE000
    };

    using Decoder::AlgBase::AlgBase;

    StatusCode initialize() override;

    std::vector<const LHCb::RawBank*> selectRawBanks( LHCb::span<const LHCb::RawBank*> banks ) const;

    class element_t final {
        Gaudi::StringKey m_key;
        bool             m_decode;
    public:
        element_t( Gaudi::StringKey key, bool decode )
            : m_key{ std::move(key) }, m_decode{ decode }
        {}
        operator const Gaudi::StringKey& () const { return m_key; }
        const std::string& str() const { return m_key.str(); }
        operator const std::string&() const { return str(); }
        bool operator!() const { return !m_decode; }
        explicit operator bool() const { return m_decode; }
    };

    const GaudiUtils::VectorMap<int,element_t>& id2string(unsigned int tck) const {
        auto itbl =  m_idTable.find(tck) ;
        if ( UNLIKELY(itbl  == std::end(m_idTable)) ) itbl = fetch_id2string(tck);
        return itbl->second;
    };
    const GaudiUtils::VectorMap<int,Gaudi::StringKey>& info2string(unsigned int tck) const {
        auto itbl =  m_infoTable.find(tck) ;
        static const Gaudi::StringKey InfoID{"InfoID"};
        if ( UNLIKELY(itbl  == std::end(m_infoTable)) ) itbl = fetch_info2string(tck, InfoID, m_infoTable);
        return itbl->second;
    }
    const GaudiUtils::VectorMap<int,Gaudi::StringKey>& packedObjectLocation2string(unsigned int tck) const {
        auto itbl =  m_packedObjectLocationsTable.find(tck) ;
        static const Gaudi::StringKey PackedObjectLocations{"PackedObjectLocations"};
        if ( UNLIKELY( itbl  == std::end(m_packedObjectLocationsTable)) )
            itbl = fetch_info2string(tck, PackedObjectLocations, m_packedObjectLocationsTable);
        return itbl->second;
    }
    unsigned int tck(const LHCb::RawEvent& event) const;

private:
    SmartIF<IANNSvc> m_hltANNSvc;
    SmartIF<IIndexedANNSvc> m_TCKANNSvc;

    using IdTable_t =  GaudiUtils::VectorMap<unsigned int, GaudiUtils::VectorMap<int, element_t>>;
    using Table_t =  GaudiUtils::VectorMap<unsigned int, GaudiUtils::VectorMap<int, Gaudi::StringKey>>;
    mutable IdTable_t m_idTable;
    IdTable_t::const_iterator fetch_id2string(unsigned int tck) const;
    mutable Table_t m_infoTable;
    mutable Table_t m_packedObjectLocationsTable;
    Table_t::const_iterator fetch_info2string(unsigned int tck, const IANNSvc::major_key_type& major, Table_t& table) const;

    /// SourceID to decode: 0=Hlt 1=Hlt1 2=Hlt2 ... (1,2 will decode from 0 if 1,2 not found)
    Gaudi::Property<unsigned int> m_sourceID { this, "SourceID", kSourceID_Dummy  };

};

using HltRawBankDecoder_Setup = Gaudi::Functional::Traits::BaseClass_t<HltRawBankDecoderBase>;

template <typename Out>
using HltRawBankDecoder = Gaudi::Functional::Transformer<Out(const LHCb::RawEvent&),HltRawBankDecoder_Setup>;

template <typename... Out>
using HltRawBankMultiDecoder = Gaudi::Functional::MultiTransformer<std::tuple<Out...>(const LHCb::RawEvent&),HltRawBankDecoder_Setup>;

template <typename Out>
using HltRawBankSplittingDecoder = Gaudi::Functional::SplittingTransformer<Gaudi::Functional::vector_of_optional_<Out>(const LHCb::RawEvent&),HltRawBankDecoder_Setup>;
#endif
