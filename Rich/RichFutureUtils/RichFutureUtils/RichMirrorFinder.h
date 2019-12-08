/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

// STL
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

// Utils
#include "RichUtils/RichSIMDTypes.h"

// Vc
#include <Vc/vector>

// from Gaudi
#include "GaudiKernel/SystemOfUnits.h"

// from LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"

// from MathCore
#include "GaudiKernel/Point3DTypes.h"

// Det Desc
#include "DetDesc/ConditionKey.h"
#include "DetDesc/IConditionDerivationMgr.h"

// RichDet
#include "RichDet/DeRichSphMirror.h"
class DeRich1;
class DeRich2;

namespace Rich::Utils {

  //-----------------------------------------------------------------------------
  /** @class MirrorFinder RichMirrorFinder.h
   *
   *  Utility to find the appropriate mirror segment for a given reflection point.
   *
   *  @author Chris Jones
   *  @date   2017-01-19
   */
  //-----------------------------------------------------------------------------

  class MirrorFinder final {

  public:
    // constructors

    /// Constructor from dependent detector elements
    MirrorFinder( const DeRich1& rich1, //
                  const DeRich2& rich2 );

  private:
    // helper classes

    /// Type for list of mirrors
    using Mirrors = Vc::vector<const DeRichSphMirror*>;

    /// SIMD Type for array of Mirrors
    template <typename TYPE>
    using SIMDMirrors = SIMD::STDArray<const DeRichSphMirror*, TYPE>;

    //-----------------------------------------------------------------------------
    /** @class LookupTableFinder RichMirrorSegFinderLookUpTable.h
     *
     *  Helper object implementing a lookup table based mirror search
     *
     *  @author Chris Jones
     *  @date   2019-11-27
     */
    //-----------------------------------------------------------------------------
    template <typename FPTYPE, std::uint32_t NXBINS, std::uint32_t NYBINS, //
              typename SIMDTYPE = SIMD::DefaultScalarFP>
    class LookupTableFinder {

    private:
      /// Type for Mirror Number. Small to minimise LUT size.
      using MirrorNum = std::uint8_t;
      /// Type for Scalar index
      using ScalarIndex = std::uint32_t;
      /// Type for SIMD array of indices
      using SIMDIndices = LHCb::SIMD::INT<ScalarIndex>;

    public:
      /// Constructor from extra size
      explicit LookupTableFinder( const FPTYPE eSize ) : m_eSize( eSize ) {}
      /// Default constructor not allowed
      LookupTableFinder() = delete;

    public:
      /// Reset this object
      void reset() {
        mirrors.clear();
        m_lookupTable.clear();
        m_minX = m_minY = 9e9;
        m_maxX = m_maxY = -9e9;
        m_incX = m_incY = 0.0;
        m_minXSIMD = m_minYSIMD = 9e9;
        m_maxXSIMD = m_maxYSIMD = -9e9;
        m_incXSIMD = m_incYSIMD = 0.0;
      }

    public:
      /// Initialise this object
      void init() {
        // sanity check
        if ( UNLIKELY( std::numeric_limits<MirrorNum>::max() < mirrors.size() ) ) {
          throw GaudiException( "Too many mirrors", "MirrorSegFinderLookUpTable::LookupTableFinder",
                                StatusCode::FAILURE );
        }

        // sort mirrors by distance from beam line (proxy for most used)
        std::sort( mirrors.begin(), mirrors.end(), //
                   []( const auto m1, const auto m2 ) {
                     return ( ( std::pow( m1->mirrorCentre().X(), 2 ) +   //
                                std::pow( m1->mirrorCentre().Y(), 2 ) ) < //
                              ( std::pow( m2->mirrorCentre().X(), 2 ) +   //
                                std::pow( m2->mirrorCentre().Y(), 2 ) ) );
                   } );

        // Loop over the mirrors to find the (x,y) range
        for ( const auto* M : mirrors ) {
          const auto& mC = M->mirrorCentre();
          if ( mC.X() > m_maxX ) { m_maxX = mC.X(); }
          if ( mC.X() < m_minX ) { m_minX = mC.X(); }
          if ( mC.Y() > m_maxY ) { m_maxY = mC.Y(); }
          if ( mC.Y() < m_minY ) { m_minY = mC.Y(); }
        }

        // Add/Subtract a little to take the extent of the mirrors into account
        m_maxX += m_eSize;
        m_maxY += m_eSize;
        m_minX -= m_eSize;
        m_minY -= m_eSize;

        // 1 / X and Y increments ( inverse, for speed in lookup )
        m_incX = (FPTYPE)nXBins() / ( m_maxX - m_minX );
        m_incY = (FPTYPE)nYBins() / ( m_maxY - m_minY );

        // Set cached SIMD values
        m_minXSIMD = SIMD::FP<SIMDTYPE>( m_minX );
        m_maxXSIMD = SIMD::FP<SIMDTYPE>( m_maxX );
        m_minYSIMD = SIMD::FP<SIMDTYPE>( m_minY );
        m_maxYSIMD = SIMD::FP<SIMDTYPE>( m_maxY );
        m_incXSIMD = SIMD::FP<SIMDTYPE>( m_incX );
        m_incYSIMD = SIMD::FP<SIMDTYPE>( m_incY );

        // initialise the look up table
        for ( auto iX = 0u; iX < nXBins(); ++iX ) {
          for ( auto iY = 0u; iY < nYBins(); ++iY ) {
            // find the mirror for this bin
            const auto m = closestXY( binX( iX ), binY( iY ) );
            // get the index for this mirror
            const auto im = std::find( mirrors.begin(), mirrors.end(), m );
            // Find the nearest mirror segment to the centre of this bin
            // and save it in the lookup table
            m_lookupTable.set( iX, iY, im - mirrors.begin() );
          }
        }
      }
      /// Find the mirror for a given position
      template <typename POINT>
      inline decltype( auto ) find( const POINT& p ) const noexcept {
        return find( p.x(), p.y() );
      }
      /// Find the mirror for a given position (x,y)
      template <typename TYPE>
      inline decltype( auto ) find( const TYPE x, const TYPE y ) const noexcept {
        if constexpr ( std::is_arithmetic<TYPE>::value ) {
          // Scalar
          return mirrors[m_lookupTable.get( xIndex( x ), yIndex( y ) )];
        } else {
          // SIMD
          // Get the mirror indices
          const auto xyi = m_lookupTable.xyIndex( xIndex( x ), yIndex( y ) );
          // gather lookup seems slower for now ... :(
          // const auto xyi = m_lookupTable.get( xIndex(x), yIndex(y) );
          // Mirrors to return
          SIMDMirrors<TYPE> mirrs;
          // Fill the mirrors. Can this be vectorised ??
          GAUDI_LOOP_UNROLL( LHCb::SIMD::FPF::Size )
          for ( std::size_t i = 0; i < TYPE::Size; ++i ) {
            // just load the mirror using the number
            // mirrs[i] = mirrors[ xyi[i] ];
            // Revert to scalar lookup
            mirrs[i] = mirrors[m_lookupTable.get( (ScalarIndex)xyi[i] )];
          }
          // return the filled mirrors
          return mirrs;
        }
      }

    private:
      /// Get the mirror closest to a given (x,y) point
      [[nodiscard]] const DeRichSphMirror* closestXY( const FPTYPE x, //
                                                      const FPTYPE y ) const {
        const DeRichSphMirror* minM = nullptr;
        // Loop over all the mirrors to find the closest
        FPTYPE minDist( 9e30 );
        for ( const auto* M : mirrors ) {
          const auto dist = distance2( x, y, M );
          if ( dist < minDist ) {
            minM    = M;
            minDist = dist;
          }
        }
        return minM;
      }
      /// Returns the 2D (x,y) distance^2 between a given point and mirror centre
      static inline decltype( auto ) distance2( const FPTYPE           x, //
                                                const FPTYPE           y, //
                                                const DeRichSphMirror* M ) {
        return ( std::pow( x - M->mirrorCentre().X(), 2 ) + //
                 std::pow( y - M->mirrorCentre().Y(), 2 ) );
      }

    private:
      /// Type for lookup storage
      using MirrorArray = Vc::vector<MirrorNum>;

      /** @class LookupTable RichMirrorSegFinderLookUpTable.h
       *  2D (x,y) Lookup table for RICH mirrors */
      class LookupTable final : private MirrorArray {

      public:
        /// Constructor
        LookupTable() { clear(); }

      public:
        /// Combine two (scalar) x,y indices in a single one
        [[nodiscard]] ScalarIndex xyIndex( const ScalarIndex ix, //
                                           const ScalarIndex iy ) const noexcept {
          return ( NYBINS * ix ) + iy;
        }
        /// Combine two (SIMD) x,y indices in a single one
        [[nodiscard]] typename SIMDIndices::IndexType xyIndex( const typename SIMDIndices::IndexType& ix, //
                                                               const typename SIMDIndices::IndexType& iy  //
                                                               ) const noexcept {
          return ( SIMDIndices::IndexType( NYBINS ) * ix ) + iy;
        }

      public:
        /// Access the mirror for a given combined xy index (Scalar)
        [[nodiscard]] MirrorNum get( const ScalarIndex ixy ) const noexcept { return ( *this )[ixy]; }
        /// Access the mirror for a given set of (x,y) indices (Scalar)
        [[nodiscard]] MirrorNum get( const ScalarIndex ix, //
                                     const ScalarIndex iy ) const noexcept {
          return get( xyIndex( ix, iy ) );
        }

      public:
        /// Access the mirror for a given xy index (SIMD)
        [[nodiscard]] SIMDIndices get( const typename SIMDIndices::IndexType& ixy ) const noexcept {
          // gather SIMD lookup
          return ( *this )[ixy];
        }

        /// Access the mirror for a given set of (x,y) indices (SIMD)
        [[nodiscard]] SIMDIndices get( const typename SIMDIndices::IndexType& ix, //
                                       const typename SIMDIndices::IndexType& iy ) const noexcept {
          // Make '1D' indices from X and Y
          return get( xyIndex( ix, iy ) );
        }

      public:
        /// Set the mirror for a given bin
        void set( const ScalarIndex ix, //
                  const ScalarIndex iy, //
                  const MirrorNum   im ) noexcept {
          ( *this )[xyIndex( ix, iy )] = im;
        }
        /// Clear the table
        void clear() noexcept {
          MirrorArray::clear();
          resize( NXBINS * NYBINS, 0 );
        }
      };

    private:
      /// Get the number of bins in X (Scalar)
      [[nodiscard]] constexpr std::uint32_t nXBins() const noexcept { return NXBINS; }
      /// Get the number of bins in Y (Scalar)
      [[nodiscard]] constexpr std::uint32_t nYBins() const noexcept { return NYBINS; }
      /// Get the minimum range in X (Scalar)
      [[nodiscard]] FPTYPE minX() const noexcept { return m_minX; }
      /// Get the maximum range in X (Scalar)
      [[nodiscard]] FPTYPE maxX() const noexcept { return m_maxX; }
      /// Get the minimum range in Y (Scalar)
      [[nodiscard]] FPTYPE minY() const noexcept { return m_minY; }
      /// Get the maximum range in Y (Scalar)
      [[nodiscard]] FPTYPE maxY() const noexcept { return m_maxY; }

    private:
      /// Get x for a given index value
      [[nodiscard]] FPTYPE binX( const ScalarIndex i ) const noexcept {
        return minX() + ( ( (FPTYPE)i + 0.5f ) / m_incX );
      }
      /// Get y for a given index value
      [[nodiscard]] FPTYPE binY( const ScalarIndex i ) const noexcept {
        return minY() + ( ( (FPTYPE)i + 0.5f ) / m_incY );
      }

    private:
      /// Get the x index
      template <typename TYPE>
      inline decltype( auto ) xIndex( const TYPE& x ) const noexcept {
        if constexpr ( std::is_arithmetic<TYPE>::value ) {
          // scalar
          return ( ScalarIndex )( x < minX() ? 0 : x > maxX() ? nXBins() - 1 : ( ( x - minX() ) * m_incX ) );
        } else {
          // SIMD
          // mask for x < min value
          const auto mask = x < m_minXSIMD;
          // form indices
          auto xtmp = ( x - m_minXSIMD ) * m_incXSIMD;
          // Underflow protection
          xtmp.setZero( mask );
          auto xi = LHCb::SIMD::simd_cast<SIMDIndices::IndexType>( xtmp );
          // Overflow protection
          xi( xi >= SIMDIndices::IndexType( NXBINS ) ) = SIMDIndices::IndexType( NXBINS - 1 );
          // return
          return xi;
        }
      }
      /// Get the y index
      template <typename TYPE>
      inline decltype( auto ) yIndex( const TYPE& y ) const noexcept {
        if constexpr ( std::is_arithmetic<TYPE>::value ) {
          // scalar
          return ( ScalarIndex )( y < minY() ? 0 : y > maxY() ? nYBins() - 1 : ( ( y - minY() ) * m_incY ) );
        } else {
          // SIMD
          // mask for y < min value
          const auto mask = y < m_minYSIMD;
          // form indices
          auto ytmp = ( y - m_minYSIMD ) * m_incYSIMD;
          // Underflow protection
          ytmp.setZero( mask );
          auto yi = LHCb::SIMD::simd_cast<SIMDIndices::IndexType>( ytmp );
          // Overflow protection
          yi( yi >= SIMDIndices::IndexType( NYBINS ) ) = SIMDIndices::IndexType( NYBINS - 1 );
          // return
          return yi;
        }
      }

    private:                                                      // SIMD cached values
      SIMD::FP<SIMDTYPE> m_minXSIMD = SIMD::FP<SIMDTYPE>::Zero(); ///< Minimum X (SIMD)
      SIMD::FP<SIMDTYPE> m_maxXSIMD = SIMD::FP<SIMDTYPE>::Zero(); ///< Maximum X (SIMD)
      SIMD::FP<SIMDTYPE> m_minYSIMD = SIMD::FP<SIMDTYPE>::Zero(); ///< Minimum Y (SIMD)
      SIMD::FP<SIMDTYPE> m_maxYSIMD = SIMD::FP<SIMDTYPE>::Zero(); ///< Maximum Y (SIMD)
      SIMD::FP<SIMDTYPE> m_incXSIMD = SIMD::FP<SIMDTYPE>::Zero(); ///< 1 / Increment in X
      SIMD::FP<SIMDTYPE> m_incYSIMD = SIMD::FP<SIMDTYPE>::Zero(); ///< 1 / Increment in Y
    private:
      FPTYPE      m_minX{9e9};   ///< Minimum X
      FPTYPE      m_maxX{-9e9};  ///< Maximum X
      FPTYPE      m_minY{9e9};   ///< Minimum Y
      FPTYPE      m_maxY{-9e9};  ///< Maximum Y
      FPTYPE      m_incX{0};     ///< 1 / Increment in X
      FPTYPE      m_incY{0};     ///< 1 / Increment in Y
      LookupTable m_lookupTable; ///< The lookup table
    public:
      /// Vector of all mirrors in this panel
      Mirrors mirrors;

    private:
      /// Extra size to take into account mirror extents
      FPTYPE m_eSize{0};
    };

    /// Specialisation for RICH1
    template <typename FPTYPE, std::uint32_t NXBINS, std::uint32_t NYBINS, typename SIMDTYPE = float>
    class R1LookupTableFinder final : public LookupTableFinder<FPTYPE, NXBINS, NYBINS, SIMDTYPE> {
    public:
      R1LookupTableFinder() : LookupTableFinder<FPTYPE, NXBINS, NYBINS, SIMDTYPE>( 100 * Gaudi::Units::mm ) {}
    };

    /// Specialisation for RICH2
    template <typename FPTYPE, std::uint32_t NXBINS, std::uint32_t NYBINS, typename SIMDTYPE = float>
    class R2LookupTableFinder final : public LookupTableFinder<FPTYPE, NXBINS, NYBINS, SIMDTYPE> {
    public:
      R2LookupTableFinder() : LookupTableFinder<FPTYPE, NXBINS, NYBINS, SIMDTYPE>( 2500 * Gaudi::Units::mm ) {}
    };

    //-----------------------------------------------------------------------------
    /** @class TwoSegmentFinder RichMirrorSegFinderLookUpTable.h
     *
     *  Helper object for a simple two mirror segment panel
     *
     *  @author Chris Jones
     *  @date   2015-02-01
     */
    //-----------------------------------------------------------------------------
    class TwoSegmentXFinder final {
    public:
      /// Reset this object
      void reset() { mirrors.clear(); }
      /// Initialise this object
      void init() {
        // Must have 2 mirrors in -ve and +ve x positions
        if ( 2 != mirrors.size() || ( mirrors[0]->mirrorCentre().X() * mirrors[1]->mirrorCentre().X() > 0 ) ) {
          throw GaudiException( "Problem with mirror segments", "MirrorSegFinderLookUpTable::TwoSegmentXFinder",
                                StatusCode::FAILURE );
        }
        // sort the mirrors so the -ve X one is in [0] and +ve in [1]
        std::sort( mirrors.begin(), mirrors.end(),
                   []( const auto m1, const auto m2 ) { return ( m1->mirrorCentre().X() < m2->mirrorCentre().X() ); } );
      }
      /// Find the mirror for a given position (Scalar)
      template <typename POINT,
                typename std::enable_if<std::is_arithmetic<typename POINT::Scalar>::value>::type* = nullptr>
      inline decltype( auto ) find( const POINT& p ) const noexcept {
        return find( p.x() );
      }
      /// Find the mirror for a given position (SIMD)
      template <typename POINT,
                typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type* = nullptr>
      inline decltype( auto ) find( const POINT& p ) const noexcept {
        return find( p.x() );
      }
      /// Find the mirror for a given position (x,y)
      template <typename TYPE>
      inline decltype( auto ) find( const TYPE x, const TYPE /* y */ ) const noexcept {
        return find( x );
      }
      /// Find the mirror for a given x position
      template <typename TYPE>
      inline decltype( auto ) find( const TYPE x ) const noexcept {
        if constexpr ( std::is_arithmetic<TYPE>::value ) {
          // Scalar
          return ( x < 0 ? mirrors[0] : mirrors[1] );
        } else {
          // SIMD - revert to scalar loop here... Is there a way to avoid this ?
          SIMDMirrors<TYPE> mirrs;
          GAUDI_LOOP_UNROLL( LHCb::SIMD::FPF::Size )
          for ( std::size_t i = 0; i < TYPE::Size; ++i ) { mirrs[i] = ( x[i] < 0 ? mirrors[0] : mirrors[1] ); }
          return mirrs;
        }
      }

    public:
      /// Vector of all mirrors in this panel
      Mirrors mirrors;
    };

    /// Overall RICH mirror finder
    template <typename R1FINDER, typename R2FINDER>
    class RichMirrorFinder {
    public:
      /// Reset the finders
      void reset() {
        m_r1Finder[Rich::top].reset();
        m_r1Finder[Rich::bottom].reset();
        m_r2Finder[Rich::left].reset();
        m_r2Finder[Rich::right].reset();
      }
      /// Validity check
      [[nodiscard]] bool isOK() const {
        return ( !m_r1Finder[Rich::top].mirrors.empty() &&    //
                 !m_r1Finder[Rich::bottom].mirrors.empty() && //
                 !m_r2Finder[Rich::left].mirrors.empty() &&   //
                 !m_r2Finder[Rich::right].mirrors.empty() );
      }
      /// Add a mirror to the given RICH and side
      void addMirror( const Rich::DetectorType rich, //
                      const Rich::Side         side, //
                      const DeRichSphMirror*   m ) {
        if ( Rich::Rich1 == rich ) {
          m_r1Finder[side].mirrors.push_back( m );
        } else {
          m_r2Finder[side].mirrors.push_back( m );
        }
      }
      /// Initialise
      void init() {
        m_r1Finder[Rich::top].init();
        m_r1Finder[Rich::bottom].init();
        m_r2Finder[Rich::left].init();
        m_r2Finder[Rich::right].init();
      }

    public:
      /// Find the mirrors for the given RICH, panel and point
      template <typename POINT>
      inline decltype( auto ) find( const Rich::DetectorType rich, //
                                    const Rich::Side         side, //
                                    const POINT&             p     //
                                    ) const noexcept {
        return find( rich, side, p.x(), p.y() );
      }
      /// Find the mirrors for the given RICH, panels and point (SIMD)
      template <typename POINT,
                typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type* = nullptr>
      inline decltype( auto ) find( const Rich::DetectorType rich,  //
                                    const Rich::SIMD::Sides& sides, //
                                    const POINT&             p      //
                                    ) const noexcept {
        return find( rich, sides, p.x(), p.y() );
      }

    public:
      /// Find the mirror for the given RICH, panel and point (x,y)
      template <typename TYPE>
      inline decltype( auto ) find( const Rich::DetectorType rich, //
                                    const Rich::Side         side, //
                                    const TYPE               x,    //
                                    const TYPE               y     //
                                    ) const noexcept {
        return ( Rich::Rich1 == rich ? m_r1Finder[side].find( x, y ) : m_r2Finder[side].find( x, y ) );
      }
      /// Find the mirror for the given RICH, panels and point (x,y) (SIMD)
      template <typename TYPE, typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type* = nullptr>
      inline decltype( auto ) find( const Rich::DetectorType rich,  //
                                    const Rich::SIMD::Sides& sides, //
                                    const TYPE               x,     //
                                    const TYPE               y      //
                                    ) const noexcept {
        using namespace LHCb::SIMD;
        // Side masks
        const auto m1 = ( sides == Rich::SIMD::Sides( (int)Rich::firstSide ) );
        const auto m2 = ( sides == Rich::SIMD::Sides( (int)Rich::secondSide ) );
        // return the right ones
        if ( all_of( m1 ) ) {
          return find( rich, Rich::firstSide, x, y );
        } else if ( all_of( m2 ) ) {
          return find( rich, Rich::secondSide, x, y );
        } else {
          auto mirrs1 = find( rich, Rich::firstSide, x, y );
          auto mirrs2 = find( rich, Rich::secondSide, x, y );
          GAUDI_LOOP_UNROLL( Rich::SIMD::Sides::Size )
          for ( std::size_t i = 0; i < Rich::SIMD::Sides::Size; ++i ) {
            if ( m2[i] ) { mirrs1[i] = mirrs2[i]; }
          }
          return mirrs1;
        }
      }

    public:
      /// Get the list of mirrors
      [[nodiscard]] const Mirrors& mirrors( const Rich::DetectorType rich, //
                                            const Rich::Side         side  //
                                            ) const noexcept {
        return ( Rich::Rich1 == rich ? m_r1Finder[side].mirrors : m_r2Finder[side].mirrors );
      }

    private:
      /// RICH1 finder
      PanelArray<R1FINDER> m_r1Finder;
      /// RICH2 finder
      PanelArray<R2FINDER> m_r2Finder;
    };

  private:
    // Finder types for each RICH and mirror type
    using R1Primary   = TwoSegmentXFinder;
    using R2Primary   = R2LookupTableFinder<double, 400, 400>;
    using R1Secondary = R1LookupTableFinder<double, 200, 100>;
    using R2Secondary = R2LookupTableFinder<double, 400, 400>;

    /// Primary mirrors
    RichMirrorFinder<R1Primary, R2Primary> m_sphMirrFinder;

    /// Secondary mirrors
    RichMirrorFinder<R1Secondary, R2Secondary> m_secMirrFinder;

  public:
    /** Locates the spherical mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  @param rich       The RICH detector
     *  @param side       The RICH HPD panel side
     *  @param reflPoint  The reflection point on the spherical mirror
     *
     *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
     */
    template <typename POINT>
    inline decltype( auto ) findSphMirror( const Rich::DetectorType rich,     //
                                           const Rich::Side         side,     //
                                           const POINT&             reflPoint //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return findSphMirror( rich, side, reflPoint.x(), reflPoint.y() );
    }

    /** Locates the spherical mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  SIMD implementation
     *
     *  @param rich       The RICH detector
     *  @param sides      The RICH HPD panel sides
     *  @param reflPoint  The reflection point on the spherical mirror
     *
     *  @return Array of DeRichSphMirror objects for the associated mirror segments
     */
    template <typename POINT,
              typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type* = nullptr>
    inline decltype( auto ) findSphMirror( const Rich::DetectorType rich,     //
                                           const Rich::SIMD::Sides& sides,    //
                                           const POINT&             reflPoint //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return findSphMirror( rich, sides, reflPoint.x(), reflPoint.y() );
    }

  public:
    /** Locates the secondary mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  @param rich       The RICH detector
     *  @param side       The RICH HPD panel side
     *  @param reflPoint  The reflection point on the secondary mirror
     *
     *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
     */
    template <typename POINT>
    inline decltype( auto ) findSecMirror( const Rich::DetectorType rich,     //
                                           const Rich::Side         side,     //
                                           const POINT&             reflPoint //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return findSecMirror( rich, side, reflPoint.x(), reflPoint.y() );
    }

    /** Locates the secondary mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  SIMD implementation
     *
     *  @param rich       The RICH detector
     *  @param sides      The RICH HPD panel sides
     *  @param reflPoint  The reflection point on the spherical mirror
     *
     *  @return Array of DeRichSphMirror objects for the associated mirror segments
     */
    template <typename POINT,
              typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type* = nullptr>
    inline decltype( auto ) findSecMirror( const Rich::DetectorType rich,     //
                                           const Rich::SIMD::Sides& sides,    //
                                           const POINT&             reflPoint //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return findSecMirror( rich, sides, reflPoint.x(), reflPoint.y() );
    }

  public:
    /** Locates the spherical mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  @param rich       The RICH detector
     *  @param side       The RICH HPD panel side
     *  @param reflPoint  The reflection point on the spherical mirror
     *
     *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
     */
    template <typename TYPE>
    inline decltype( auto ) findSphMirror( const Rich::DetectorType rich, //
                                           const Rich::Side         side, //
                                           const TYPE               x,    //
                                           const TYPE               y     //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return m_sphMirrFinder.find( rich, side, x, y );
    }

    /** Locates the spherical mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  SIMD implementation
     *
     *  @param rich       The RICH detector
     *  @param sides      The RICH HPD panel sides
     *  @param reflPoint  The reflection point on the spherical mirror
     *
     *  @return Array of DeRichSphMirror objects for the associated mirror segments
     */
    template <typename TYPE, typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type* = nullptr>
    inline decltype( auto ) findSphMirror( const Rich::DetectorType rich,  //
                                           const Rich::SIMD::Sides& sides, //
                                           const TYPE               x,     //
                                           const TYPE               y      //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return m_sphMirrFinder.find( rich, sides, x, y );
    }

  public:
    /** Locates the secondary mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  @param rich       The RICH detector
     *  @param side       The RICH HPD panel side
     *  @param reflPoint  The reflection point on the secondary mirror
     *
     *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
     */
    template <typename TYPE>
    inline decltype( auto ) findSecMirror( const Rich::DetectorType rich, //
                                           const Rich::Side         side, //
                                           const TYPE               x,    //
                                           const TYPE               y     //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return m_secMirrFinder.find( rich, side, x, y );
    }

    /** Locates the secondary mirror Segment given a reflection point,
     *  RICH identifier and panel
     *
     *  SIMD implementation
     *
     *  @param rich       The RICH detector
     *  @param sides      The RICH HPD panel sides
     *  @param reflPoint  The reflection point on the spherical mirror
     *
     *  @return Array of DeRichSphMirror objects for the associated mirror segments
     */
    template <typename TYPE, typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type* = nullptr>
    inline decltype( auto ) findSecMirror( const Rich::DetectorType rich,  //
                                           const Rich::SIMD::Sides& sides, //
                                           const TYPE               x,     //
                                           const TYPE               y      //
                                           ) const noexcept {
      // Find the mirror from the lookup map
      return m_secMirrFinder.find( rich, sides, x, y );
    }

  public:
    // conditions handling

    /// Creates a condition derivation
    template <typename PARENT>
    static decltype( auto ) addConditionDerivation( PARENT* parent ) {
      // Assume parent algorithm has one and only one input of the correct type...
      return addConditionDerivation( parent, parent->template inputLocation<MirrorFinder>() );
    }

    /// Creates a condition derivation for the given key
    template <typename PARENT>
    static decltype( auto )                                     //
    addConditionDerivation( PARENT*                     parent, ///< Pointer to parent algorithm
                            LHCb::DetDesc::ConditionKey key     ///< Derived object name
    ) {
      if ( parent->msgLevel( MSG::DEBUG ) ) {
        parent->debug() << "MirrorFinder::addConditionDerivation : Key=" << key << endmsg;
      }
      return LHCb::DetDesc:: //
          addConditionDerivation<MirrorFinder( const DeRich1&, const DeRich2& )>(
              parent->conditionDerivationMgr(),                 // mamanager
              {DeRichLocations::Rich1, DeRichLocations::Rich2}, // input condition locations
              std::move( key ) );                               // output derived condition location
    }

    /// Default conditions name
    static constexpr const char* DefaultConditionKey = "RichMirrorFinder-Handler";
  };

} // namespace Rich::Utils
