#ifndef COMPONENT_ODINTYPESFILTER_H
#define COMPONENT_ODINTYPESFILTER_H 1

// Include files
#include <iomanip>
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/ParsersFactory.h"


namespace OdinTypesFilter_details {
    //=======================================================
    enum class Log_t { And, Or };

    const char *toString(Log_t op) {
        switch (op) {
            case Log_t::And: return "AND";
            case Log_t::Or:  return "OR";
        }
        throw "IMPOSSIBLE";
    }

    StatusCode parse(Log_t& op, const std::string& in) {
        if (in=="AND"||in=="And"||in=="and") { op = Log_t::And; return StatusCode::SUCCESS; }
        if (in=="OR" ||in=="Or" ||in=="or" ) { op = Log_t::Or;  return StatusCode::SUCCESS; }
        return StatusCode::FAILURE;
    }

    std::ostream& toStream( const Log_t& op, std::ostream& os) {
        return os << std::quoted(toString(op), '\'' );
    }
    //=======================================================

    template <typename Type, int Max>
    struct odin_type_t  {
        int type;

        static constexpr odin_type_t All() { return {-1}; }

        friend constexpr bool operator==( const odin_type_t<Type,Max>& lhs ,
                                          const odin_type_t<Type,Max>& rhs )
        { return lhs.type == rhs.type; }

        friend constexpr bool operator==( const Type& lhs, const odin_type_t<Type,Max>& rhs)
        { return rhs == All() || lhs == (Type)rhs.type; }

        friend constexpr bool operator==( const odin_type_t<Type,Max>& lhs, const Type& rhs)
        { return rhs == lhs; }
    };

    template <typename Type, int Max>
    StatusCode parse(odin_type_t<Type,Max>& t, const std::string& in) {
        if (in=="ALL") { t = odin_type_t<Type,Max>::All(); return StatusCode::SUCCESS; }
        for (int k=0;  k <= Max; ++k ) {
            std::ostringstream s; s << (Type) k;
            if (s.str() == in) { t.type = k; return StatusCode::SUCCESS; }
        }
        return StatusCode::FAILURE;
    }

    template <typename Type, int Max>
    StatusCode parse(std::vector<odin_type_t<Type,Max>>& v, const std::string& in) {
        std::vector<std::string> vs;
        using Gaudi::Parsers::parse;
        auto sc = parse(vs,in);
        if (!sc) return sc;
        v.clear();
        v.reserve(vs.size());
        try { std::transform( vs.begin(), vs.end(), std::back_inserter(v),
                              [](const std::string& s) {
                                  odin_type_t<Type,Max> t{};
                                  auto sc = parse(t,s);
                                  if (!sc) throw GaudiException("Bad Parse","", sc);
                                  return t;
                              } );
        } catch (const GaudiException& ge) { return ge.code(); }
        return StatusCode::SUCCESS;
    }

    template <typename Type, int Max>
    std::string toString(odin_type_t<Type,Max>& t) {
        if (t==odin_type_t<Type,Max>::All()) return  "ALL";
        std::ostringstream oss; oss << (Type)t.type ;
        return oss.str();
    }

    template <typename Type, int Max>
    std::ostream& toStream(const odin_type_t<Type,Max>& t, std::ostream& os) {
        if (t==odin_type_t<Type,Max>::All()) return os << "\'ALL\'";
        return os << '\'' << (Type)t.type << '\'';
    }

    template <typename Type, int Max>
    std::ostream& operator<<(std::ostream& os, const odin_type_t<Type,Max>& t)
    { return toStream(t,os); }

    //=====

    using  odin_trigger_type_t =
         odin_type_t<LHCb::ODIN::TriggerType,
                     (LHCb::ODIN::Word8Masks::TriggerTypeMask >> LHCb::ODIN::Word8Bits::TriggerTypeBits)>;

    using  odin_readout_type_t =
        odin_type_t<LHCb::ODIN::ReadoutTypes,
                    (LHCb::ODIN::Word8Masks_v4::ReadoutTypeMask >> LHCb::ODIN::Word8Bits_v4::ReadoutTypeBits)>;

    using odin_bx_type_t =
        odin_type_t<LHCb::ODIN::BXTypes,
                    (LHCb::ODIN::Word8Masks::BXTypeMask >> LHCb::ODIN::Word8Bits::BXTypeBits)>;

    using odin_calibration_type_t =
        odin_type_t<LHCb::ODIN::CalibrationTypes,
                    (LHCb::ODIN::Word8Masks::CalibrationTypeMask >> LHCb::ODIN::Word8Bits::CalibrationTypeBits)>;

    //=======================================================
}

/** @class OdinTypesFilter OdinTypesFilter.h component/OdinTypesFilter.h
 *  @author Olivier Deschamps
 *  @date   2008-02-05
 */
class OdinTypesFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:
  Gaudi::Property<OdinTypesFilter_details::Log_t> m_log { this,  "Logical", OdinTypesFilter_details::Log_t::And };
  Gaudi::Property<std::vector<OdinTypesFilter_details::odin_trigger_type_t>> m_trs { this, "TriggerTypes", { OdinTypesFilter_details::odin_trigger_type_t::All() }    };
  Gaudi::Property<std::vector<OdinTypesFilter_details::odin_bx_type_t>> m_bxs { this, "BXTypes"     , { OdinTypesFilter_details::odin_bx_type_t::All() }    };
  Gaudi::Property<std::vector<OdinTypesFilter_details::odin_readout_type_t>> m_ros { this, "ReadoutTypes", { OdinTypesFilter_details::odin_readout_type_t::All() }    };
  Gaudi::Property<std::vector<OdinTypesFilter_details::odin_calibration_type_t>> m_cls { this, "CalibrationTypes", { OdinTypesFilter_details::odin_calibration_type_t::All() }};
  Gaudi::Property<int> m_winmin { this, "TAEWindowMoreThan" , -1 };
  Gaudi::Property<int> m_winmax { this, "TAEWindowLessThan" , 99 };
  std::atomic<long> m_all { 0 };
  std::atomic<long> m_acc { 0 };
};
#endif // COMPONENT_ODINTYPESFILTER_H
