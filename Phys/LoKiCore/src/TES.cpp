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
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/AnyDataWrapper.h"
#include "GaudiKernel/IAlgContextSvc.h"
#include "GaudiKernel/ObjectContainerBase.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GetAlgs.h"
// ============================================================================
// Gaudi
// ============================================================================
#include "Event/HCDigit.h"
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/Counters.h"
#include "Kernel/STLExtensions.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Exception.h"
#include "LoKi/ILoKiSvc.h"
#include "LoKi/Services.h"
#include "LoKi/TES.h"
// ============================================================================
#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 2259 ) // non-pointer conversion may lose significant bits
#  pragma warning( disable : 1572 ) // floating-point equality and inequality comparisons are unreliable
#endif
// ============================================================================
/** @file
 *  Implementation file for classes from namespace LoKi::TES
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2020-02-13
 */
// ============================================================================
namespace {
  // ==========================================================================
  /// the invalid algorithm name
  const std::string s_INVALID = "-<NON-YET-LOCATED-ALGORITHM>-";
  // ==========================================================================
} // namespace
// ============================================================================
/*  constructor from TES location and root-in-tes flag
 *  @param algorithm    own algorithm/context
 *  @param location     TES-location
 *  @param useRootInTES RootInTES-flag
 */
// ============================================================================
LoKi::TES::Get::Get( const GaudiAlgorithm* algorithm, const std::string& location, const bool useRootInTES )
    : LoKi::AuxFunBase( std::tie( algorithm, location, useRootInTES ) )
    , m_location( location )
    , m_useRootInTES( useRootInTES )
    , m_algorithm( algorithm )
    , m_datasvc( nullptr ) {
  // if ( gaudi() ) { getAlgSvc() ; }
}
// ============================================================================
/*  constructor from TES location
 *  @param datasvc    data service
 *  @param location   TES-location
 */
// ============================================================================
LoKi::TES::Get::Get( const IDataProviderSvc* datasvc, const std::string& location )
    : LoKi::AuxFunBase( std::tie( datasvc, location ) )
    , m_location( location )
    , m_useRootInTES( false )
    , m_algorithm( nullptr )
    , m_datasvc( datasvc ) {
  // if ( gaudi() ) { getAlgSvc() ; }
}
// ============================================================================
// virtual destructor
// ============================================================================
LoKi::TES::Get::~Get() {
  if ( m_algorithm && !gaudi() ) { m_algorithm.reset(); }
  if ( m_datasvc && !gaudi() ) { m_datasvc.reset(); }
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::TES::Get::fillStream( std::ostream& s ) const {
  s << " GET("
    << "'" << location() << "'";
  if ( m_algorithm && !useRootInTES() ) { s << ", False"; }
  return s << ") ";
}
// ============================================================================
// get algorithm name
// ============================================================================
const std::string& LoKi::TES::Get::algName() const { return !algorithm() ? s_INVALID : algorithm()->name(); }
// ============================================================================

// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Exists::Exists( const GaudiAlgorithm* algorithm, const std::string& location, const bool rootInTes )
    : LoKi::AuxFunBase( std::tie( algorithm, location, rootInTes ) )
    , LoKi::TES::Get( algorithm, location, rootInTes ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Exists::Exists( const IDataProviderSvc* datasvc, const std::string& location )
    : LoKi::AuxFunBase( std::tie( datasvc, location ) ), LoKi::TES::Get( datasvc, location ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::TES::Exists* LoKi::TES::Exists::clone() const { return new LoKi::TES::Exists( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::TES::Exists::operator()() const { return LoKi::TES::exists_<DataObject>( *this ); }
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::TES::Exists::fillStream( std::ostream& s ) const {
  s << " EXISTS( ";
  Gaudi::Utils::toStream( location(), s );
  if ( useRootInTES() ) { s << " , False"; }
  return s << " ) ";
}
// ============================================================================

// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Contains::Contains( const GaudiAlgorithm* algorithm, const std::string& location, const bool useRootInTes )
    : LoKi::AuxFunBase( std::tie( algorithm, location, useRootInTes ) )
    , LoKi::TES::Get( algorithm, location, useRootInTes ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Contains::Contains( const IDataProviderSvc* datasvc, const std::string& location )
    : LoKi::AuxFunBase( std::tie( datasvc, location ) ), LoKi::TES::Get( datasvc, location ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::TES::Contains* LoKi::TES::Contains::clone() const { return new LoKi::TES::Contains( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::TES::Contains::operator()( /* LoKi::TES::Contains::argument */ ) const {
  const ObjectContainerBase* obj = LoKi::TES::get_<ObjectContainerBase>( *this );
  if ( !obj ) { return -1; }
  return obj->numberOfObjects();
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::TES::Contains::fillStream( std::ostream& s ) const {
  s << " CONTAINS( ";
  Gaudi::Utils::toStream( location(), s );
  if ( !useRootInTES() ) { s << " , False"; }
  return s << " ) ";
}
// ============================================================================

// ============================================================================
LoKi::TES::Size::Size( const GaudiAlgorithm* algorithm, const std::string& location )
    : LoKi::AuxFunBase( std::tie( algorithm, location ) ), LoKi::TES::DataHandle<DataObject>( algorithm, location ) {}
// ============================================================================
LoKi::TES::Size* LoKi::TES::Size::clone() const { return new LoKi::TES::Size( algorithm(), location() ); }
// ============================================================================
double LoKi::TES::Size::operator()( /* LoKi::TES::Size::argument */ ) const {
  auto obj = get();
  //
  auto container = dynamic_cast<ObjectContainerBase*>( obj );
  if ( container ) { return container->numberOfObjects(); } // RETURN
  auto anydata = dynamic_cast<AnyDataWrapperBase*>( obj );
  if ( anydata ) { return anydata->size().value_or( -1 ); } // RETURN
  //
  return -1; // RETURN
}
// ============================================================================
std::ostream& LoKi::TES::Size::fillStream( std::ostream& s ) const { return s << "SIZE('" << location() << "')"; }
// ============================================================================

// ============================================================================
LoKi::TES::HasData::HasData( const GaudiAlgorithm* algorithm, const std::string& location )
    : LoKi::AuxFunBase( std::tie( algorithm, location ) ), LoKi::TES::DataHandle<DataObject>( algorithm, location ) {}
// ============================================================================
LoKi::TES::HasData* LoKi::TES::HasData::clone() const { return new LoKi::TES::HasData( algorithm(), location() ); }
// ============================================================================
bool LoKi::TES::HasData::operator()( /* LoKi::TES::HasData::argument */ ) const { return exist(); }
// ============================================================================
std::ostream& LoKi::TES::HasData::fillStream( std::ostream& s ) const { return s << "HASDATA('" << location() << "')"; }
// ============================================================================

// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::HrcSumAdc::HrcSumAdc( const GaudiAlgorithm* algorithm, const std::string& location,
                                 const std::string& stationName, const bool useRootInTes )
    : LoKi::AuxFunBase( std::tie( algorithm, location, stationName, useRootInTes ) )
    , LoKi::TES::Get( algorithm, location, useRootInTes )
    , m_stationName( stationName ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::TES::HrcSumAdc* LoKi::TES::HrcSumAdc::clone() const { return new LoKi::TES::HrcSumAdc( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::TES::HrcSumAdc::operator()() const {
  //
  const LHCb::HCDigits* digits = LoKi::TES::get_<LHCb::HCDigits>( *this );
  //
  if ( !digits ) { return 99999; } // RETURN
  // Determine the station ID (internal index)
  int stationId = -1;
  if ( stationName().compare( "B0" ) == 0 )
    stationId = 0;
  else if ( stationName().compare( "B1" ) == 0 )
    stationId = 1;
  else if ( stationName().compare( "B2" ) == 0 )
    stationId = 2;
  else if ( stationName().compare( "F1" ) == 0 )
    stationId = 3;
  else if ( stationName().compare( "F2" ) == 0 )
    stationId = 4;
  //
  // Compute ADC sum
  LHCb::HCCellID id( stationId );
  const auto     digit = digits->object( id );
  return digit ? digit->adc() : 99999;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::TES::HrcSumAdc::fillStream( std::ostream& s ) const {
  s << " HRCSUMADC( ";
  Gaudi::Utils::toStream( location(), s );
  s << " , ";
  Gaudi::Utils::toStream( stationName(), s );
  if ( !useRootInTES() ) { s << " , False"; }
  return s << " ) ";
}
// ============================================================================

// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Counter::Counter( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
                             const double bad, const bool useRootInTes )
    : LoKi::AuxFunBase( std::tie( algorithm, location, counter, bad, useRootInTes ) )
    , LoKi::TES::Contains( algorithm, location, useRootInTes )
    , m_counter( counter )
    , m_bad( bad ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Counter::Counter( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter )
    : LoKi::AuxFunBase( std::tie( algorithm, location, counter ) )
    , LoKi::TES::Contains( algorithm, location )
    , m_counter( counter )
    , m_bad( LoKi::Constants::NegativeInfinity ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Counter::Counter( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter )
    : LoKi::AuxFunBase( std::tie( datasvc, location, counter ) )
    , LoKi::TES::Contains( datasvc, location )
    , m_counter( counter )
    , m_bad( LoKi::Constants::NegativeInfinity ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Counter::Counter( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter,
                             const double bad )
    : LoKi::AuxFunBase( std::tie( datasvc, location, counter, bad ) )
    , LoKi::TES::Contains( datasvc, location )
    , m_counter( counter )
    , m_bad( bad ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::TES::Counter* LoKi::TES::Counter::clone() const { return new LoKi::TES::Counter( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::TES::Counter::operator()() const {
  //
  const Gaudi::Numbers* data = LoKi::TES::get_<Gaudi::Numbers>( *this );
  if ( !data ) {
    Error( "No valid object is found for TES location, return 'bad'" );
    return m_bad;
  }
  //
  const Gaudi::Numbers::Map& m     = data->counters();
  auto                       ifind = m.find( counter() );
  if ( m.end() == ifind ) {
    Error( "No counter is found, return 'bad'" );
    return m_bad; // RETURN
  }
  //
  return ifind->second;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::TES::Counter::fillStream( std::ostream& s ) const {
  s << " COUNTER( ";
  Gaudi::Utils::toStream( location(), s );
  s << ",";
  Gaudi::Utils::toStream( counter(), s );
  //
  if ( LoKi::Constants::NegativeInfinity != m_bad ) { s << ", " << m_bad; }
  if ( !useRootInTES() ) { s << " , False"; }
  //
  return s << " ) ";
}
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace TES {
    // =======================================================================-
    /** Helper class used to extract information from a \c StatEntity object.
     *  @author Marco Clemencic <marco.clemencic@cern.ch>
     */
    class StatEntityGetter {
    private:
      /// List of known getters in \c StatEntity.
      enum StatFunction { nEntries, sum, sum2, mean, rms, meanErr, min, max, eff, effErr };
      /// Base class for the internal helper object.
      /// The derived class have to re-implement the operator() and the method
      /// name().
      struct BaseHelper {
        /// Virtual destructor.
        virtual ~BaseHelper() = default;
        /// Accessor method.
        /// Extract some data from a \c StatEntity object.
        virtual double operator()( const StatEntity& ent ) const = 0;
        /// Name of the data member of \c StatEntity that the helper gets.
        virtual std::string name() const = 0;
      };

      /// Helper used to actually access the content of \c StatEntity.
      template <StatFunction Fun>
      struct Helper final : public BaseHelper {
        /// Extract the data member specified with the template argument from a
        /// \c StatEntity object.
        double operator()( const StatEntity& ent ) const override;
        /// Return a string representing, essentially, the template argument.
        std::string name() const override;
      };

      /// Commodity function to create a new specialized \c Helper instance.
      /// This function is used only by the other, non-templated, setHelper() method.
      template <StatFunction Fun>
      inline void setHelper();

      /// Commodity function to create a new specialized Helper instance.
      /// This method uses internally the templated one for simplicity. The
      /// separation between the two member is not really needed except to ensure
      /// that all the entries in the enum StatFunction are used and (not more).
      void setHelper( StatFunction fun );

      /// Pointer to the actual instance of the Helper class.
      std::shared_ptr<const BaseHelper> m_helper;

    public:
      /// Constructor.
      /// Maps a string the the required Helper instance to extract the requested
      /// data member for \c StatEntity.
      StatEntityGetter( const std::string& fun ) {
        using _t = std::pair<StatFunction, const char*>;
        static const auto tbl =
            std::array{// basic
                       _t{nEntries, "nEntries"}, _t{sum, "sum"}, _t{sum2, "sum2"}, _t{mean, "mean"}, _t{rms, "rms"},
                       _t{meanErr, "meanErr"}, _t{min, "min"}, _t{max, "max"},
                       // derived
                       _t{eff, "efficiency"}, _t{effErr, "efficiencyErr"}, _t{eff, "eff"}, _t{effErr, "effErr"},
                       // a'la ROOT
                       _t{sum, "Sum"}, _t{sum2, "Sum2"}, _t{mean, "Mean"}, _t{rms, "Rms"}, _t{rms, "RMS"},
                       _t{meanErr, "MeanErr"}, _t{min, "Min"}, _t{max, "Max"}, _t{eff, "Eff"}, _t{effErr, "EffErr"},
                       _t{nEntries, "N"}, _t{nEntries, "Entries"}, _t{nEntries, "entries"}};

        auto i = std::find_if( tbl.begin(), tbl.end(), [&]( const _t& p ) { return fun == p.second; } );
        if ( UNLIKELY( i == tbl.end() ) ) { throw LoKi::Exception( "invalid function name '" + fun + "'" ); }
        setHelper( i->first );
      }

      /// Accessor function.
      /// Forwards the call to the \c Helper instance.
      double operator()( const StatEntity& ent ) const {
        assert( m_helper.get() );
        return ( *m_helper )( ent );
      }

      /// Name of the \c StatEntity data member.
      /// Forwards the call to the \c Helper instance.
      std::string name() const {
        assert( m_helper.get() );
        return m_helper->name();
      }
    };

// Macro to simplify the specialization of Helper
#define SpecializedHelper( Fun )                                                                                       \
  template <>                                                                                                          \
  double StatEntityGetter::Helper<StatEntityGetter::Fun>::operator()( const StatEntity& ent ) const {                  \
    return ent.Fun();                                                                                                  \
  }                                                                                                                    \
  template <>                                                                                                          \
  std::string StatEntityGetter::Helper<StatEntityGetter::Fun>::name() const {                                          \
    return #Fun;                                                                                                       \
  }                                                                                                                    \
  template <>                                                                                                          \
  inline void StatEntityGetter::setHelper<StatEntityGetter::Fun>() {                                                   \
    m_helper = std::make_shared<const Helper<Fun>>();                                                                  \
  }

    SpecializedHelper( nEntries ) SpecializedHelper( sum ) SpecializedHelper( sum2 ) SpecializedHelper( mean )
        SpecializedHelper( rms ) SpecializedHelper( meanErr ) SpecializedHelper( min ) SpecializedHelper( max )
            SpecializedHelper( eff ) SpecializedHelper( effErr )

        // Function used to convert the enum value to a Helper instance.
        // It uses a switch to profit from the compiler checks on the treated cases,
        // ensuring that all the entries in the enum (and not more) are considered.
        void StatEntityGetter::setHelper( StatFunction fun ) {
      switch ( fun ) {
      case nEntries:
        setHelper<nEntries>();
        break;
      case sum:
        setHelper<sum>();
        break;
      case sum2:
        setHelper<sum2>();
        break;
      case mean:
        setHelper<mean>();
        break;
      case rms:
        setHelper<rms>();
        break;
      case meanErr:
        setHelper<meanErr>();
        break;
      case min:
        setHelper<min>();
        break;
      case max:
        setHelper<max>();
        break;
      case eff:
        setHelper<eff>();
        break;
      case effErr:
        setHelper<effErr>();
        break;
      }
    }

  } // namespace TES
} // namespace LoKi

// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Stat::Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
                       const StatEntityGetter& function, const double bad, const bool useRootInTes )
    : LoKi::TES::Counter( algorithm, location, counter, bad, useRootInTes )
    , m_getter( std::make_shared<const StatEntityGetter>( function ) ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Stat::Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
                       const StatEntityGetter& function )
    : LoKi::TES::Counter( algorithm, location, counter )
    , m_getter( std::make_shared<const StatEntityGetter>( function ) ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Stat::Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
                       const std::string& function, const double bad, const bool useRootInTes )
    : LoKi::AuxFunBase( std::tie( algorithm, location, counter, function, bad, useRootInTes ) )
    , LoKi::TES::Counter( algorithm, location, counter, bad, useRootInTes )
    , m_getter( std::make_shared<const StatEntityGetter>( function ) ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Stat::Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
                       const std::string& function )
    : LoKi::AuxFunBase( std::tie( algorithm, location, counter, function ) )
    , LoKi::TES::Counter( algorithm, location, counter )
    , m_getter( std::make_shared<const StatEntityGetter>( function ) ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Stat::Stat( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter,
                       const std::string& function, const double bad )
    : LoKi::AuxFunBase( std::tie( datasvc, location, counter, function, bad ) )
    , LoKi::TES::Counter( datasvc, location, counter, bad )
    , m_getter( std::make_shared<const StatEntityGetter>( function ) ) {}
// ============================================================================
// constructor from TES location
// ============================================================================
LoKi::TES::Stat::Stat( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter,
                       const std::string& function )
    : LoKi::AuxFunBase( std::tie( datasvc, location, counter, function ) )
    , LoKi::TES::Counter( datasvc, location, counter )
    , m_getter( std::make_shared<const StatEntityGetter>( function ) ) {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::TES::Stat* LoKi::TES::Stat::clone() const { return new LoKi::TES::Stat( *this ); }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::TES::Stat::operator()() const {
  //
  if ( counter().empty() ) {
    const Gaudi::Counter* cnt = LoKi::TES::get_<Gaudi::Counter>( *this );
    if ( cnt ) return ( *m_getter )( cnt->counter() );
  }
  //
  const Gaudi::Counters* data = LoKi::TES::get_<Gaudi::Counters>( *this );
  if ( !data ) {
    Error( "No valid object is found for TES location, return 'bad'" );
    return bad();
  }
  //
  const Gaudi::Counters::Map& m     = data->counters();
  auto                        ifind = m.find( counter() );
  if ( m.end() == ifind ) {
    Error( "No counter is found, return 'bad'" );
    return bad(); // RETURN
  }

  return ( *m_getter )( ifind->second );
}

// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::TES::Stat::fillStream( std::ostream& s ) const {
  s << " STAT( ";
  Gaudi::Utils::toStream( location(), s );
  s << ",";
  Gaudi::Utils::toStream( counter(), s );
  //
  s << m_getter->name();
  //
  if ( LoKi::Constants::NegativeInfinity != bad() ) { s << ", " << bad(); }
  if ( !useRootInTES() ) { s << " , False"; }
  //
  return s << " ) ";
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
