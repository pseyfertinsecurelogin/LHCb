
//-----------------------------------------------------------------------------
/** @file IRichMirrorSegFinderLookUpTable.h
 *
 *  Header file for tool interface : Rich::IMirrorSegFinderLookUpTable
 *
 *  @author Antonis Papanestis
 *  @date   2003-11-04
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <cstdint>
#include <limits>
#include <algorithm>
#include <type_traits>

// Utils
#include "RichUtils/RichSIMDTypes.h"

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/SystemOfUnits.h"

// from LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"

// from MathCore
#include "GaudiKernel/Point3DTypes.h"

// RichDet
#include "RichDet/DeRichSphMirror.h"

namespace Rich
{
 namespace Future
  {

    //-----------------------------------------------------------------------------
    /** @class IMirrorSegFinderLookUpTable RichInterfaces/IRichMirrorSegFinderLookUpTable.h
     *
     *  Interface to a tool to find the appropriate mirror segment for a given reflection point.
     *  This interface is specific to the implementation using a look up table, and
     *  as such provides the most efficient access.
     *
     *  @author Chris Jones
     *  @date   2017-01-19
     */
    //-----------------------------------------------------------------------------
    
    class IMirrorSegFinderLookUpTable : public virtual IAlgTool
    {
      
    public:
      
      /// Interface ID
      DeclareInterfaceID( IMirrorSegFinderLookUpTable, 1, 0 );
      
    protected: // helper classes
      
      /// Type for list of mirrors
      using Mirrors = std::vector<const DeRichSphMirror*>;

      /// SIMD Type for array of Mirrors
      template< typename TYPE >
      using SIMDMirrors = Rich::SIMD::STDArray< const DeRichSphMirror *, TYPE >;
      
      //-----------------------------------------------------------------------------
      /** @class LookupTableFinder RichMirrorSegFinderLookUpTable.h
       *
       *  Helper object implementing a lookup table based mirror search
       *
       *  @author Chris Jones
       *  @date   2015-02-01
       */
      //-----------------------------------------------------------------------------
      template < typename FPTYPE, uint32_t NXBINS, uint32_t NYBINS, typename SIMDTYPE = float >
      class LookupTableFinder
      {
      public:  
        /// Constructor from extra size
        explicit LookupTableFinder( const FPTYPE eSize ) : m_eSize(eSize) { }
        /// Default constructor not allowed
        LookupTableFinder() = delete;
      public:
        /// Reset this object
        void reset()
        {
          mirrors.clear();
          m_lookupTable.clear();
          m_minX     = m_minY     =  9e9;
          m_maxX     = m_maxY     = -9e9;
          m_incX     = m_incY     =  0.0;
          m_minXSIMD = m_minYSIMD =  9e9;
          m_maxXSIMD = m_maxYSIMD = -9e9;
          m_incXSIMD = m_incYSIMD =  0.0;
        }
      public:
        /// Initialise this object
        void init()
        {
          // sanity check
          if ( std::numeric_limits<uint8_t>::max() < mirrors.size() )
          {
            throw GaudiException( "Too many mirrors", 
                                  "MirrorSegFinderLookUpTable::LookupTableFinder",
                                  StatusCode::FAILURE );
          }

          // sort mirrors by distance from beam line (proxy for most used)
          std::sort( mirrors.begin(), mirrors.end(),
                     []( const auto m1, const auto m2 )
                     { return ( ( std::pow(m1->mirrorCentre().X(),2) + 
                                  std::pow(m1->mirrorCentre().Y(),2) ) <
                                ( std::pow(m2->mirrorCentre().X(),2) + 
                                  std::pow(m2->mirrorCentre().Y(),2) ) ); } );

          // Loop over the mirrors to find the (x,y) range
          for ( const auto* M : mirrors )
          {
            const auto & mC = M->mirrorCentre();
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
          m_minXSIMD = SIMD::FP<SIMDTYPE>(m_minX);
          m_maxXSIMD = SIMD::FP<SIMDTYPE>(m_maxX);
          m_minYSIMD = SIMD::FP<SIMDTYPE>(m_minY);
          m_maxYSIMD = SIMD::FP<SIMDTYPE>(m_maxY);
          m_incXSIMD = SIMD::FP<SIMDTYPE>(m_incX);
          m_incYSIMD = SIMD::FP<SIMDTYPE>(m_incY);

          // initialise the look up table
          for ( auto iX = 0u; iX < nXBins(); ++iX )
          {
            for ( auto iY = 0u; iY < nYBins(); ++iY )
            {
              // find the mirror for this bin
              const auto m = closestXY( binX(iX), binY(iY) );
              // get the index for this mirror
              const auto im = std::find( mirrors.begin(), mirrors.end(), m );
              // Find the nearest mirror segment to the centre of this bin
              // and save it in the lookup table
              m_lookupTable.set( iX, iY, im - mirrors.begin() );
            }
          }

        }
        /// Find the mirror for a given position (Scalar)
        template< typename POINT,
                  typename std::enable_if<std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
        inline const DeRichSphMirror* find( const POINT& p ) const noexcept
        {
          return find( p.x(), p.y() );
        }
        /// Find the mirror for a given position (SIMD)
        template< typename POINT,
                  typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
        inline SIMDMirrors<typename POINT::Scalar> find( const POINT& p ) const noexcept
        {
          return find( p.x(), p.y() );
        }
        /// Find the mirror for a given position (x,y) (Scalar)
        template< typename TYPE,
                  typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline const DeRichSphMirror* find( const TYPE x, const TYPE y ) const noexcept
        {
          return mirrors[ m_lookupTable.get( xIndex(x), yIndex(y) ) ];
        }
        /// Find the mirror for a given position (x,y) (SIMD)
        template< typename TYPE,
                  typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline SIMDMirrors<TYPE> find( const TYPE x, const TYPE y ) const noexcept
        {
          // Get the mirror indices
          const auto xi = xIndex(x);
          const auto yi = yIndex(y);
          SIMDMirrors<TYPE> mirrs;
          // Eventually might want to see if VectorClass lookup could be used here ?
          for ( std::size_t i = 0; i < TYPE::Size; ++i )
          {
            mirrs[i] = mirrors[ m_lookupTable.get( xi[i], yi[i] ) ];
          }
          return mirrs;
        }
      private:
        /// Get the mirror closest to a given (x,y) point
        const DeRichSphMirror* closestXY( const FPTYPE x, const FPTYPE y ) const
        {
          const DeRichSphMirror* minM = nullptr;  
          // Loop over all the mirrors to find the closest
          FPTYPE minDist(9e30);
          for ( const auto* M : mirrors )
          {
            const auto dist = distance2( x, y, M );
            if ( dist < minDist ) { minM = M; minDist = dist; }
          }
          return minM;
        }
        /// Returns the 2D (x,y) distance^2 between a given point and mirror centre
        static inline double distance2( const FPTYPE x, 
                                        const FPTYPE y,
                                        const DeRichSphMirror* M )
        {
          return ( std::pow( x - M->mirrorCentre().X() , 2 ) + 
                   std::pow( y - M->mirrorCentre().Y() , 2 ) );
        }
      private:
        /// Type for 2D lookup storage
        using Mirror2DArray = std::array< std::array< uint8_t, NYBINS >, NXBINS >;
        /** @class LookupTable RichMirrorSegFinderLookUpTable.h
         *  2D (x,y) Lookup table for RICH mirrors */
        class LookupTable final : public Mirror2DArray
        {
        public:
          /// Constructor
          LookupTable( ) = default;
          /// Access the mirror for a given set of indices (Scalar)
          inline uint8_t get( const uint32_t ix,
                              const uint32_t iy ) const noexcept
          {
            return ((*this)[ix])[iy];
          }
          /// Access the mirror for a given set of indices (SIMD)
          inline SIMD::FP<uint8_t> get( const SIMD::FP<uint32_t> ix,
                                        const SIMD::FP<uint32_t> iy ) const noexcept
          {
            // Make '1D' indices from X and Y
            //const auto i = ( ix * SIMD::FP<uint32_t>(NXBINS) ) + iy;
            // Eventually might want to see if VectorClass lookup could be used here ?
            SIMD::FP<uint8_t> indices;
            for ( std::size_t i = 0; i < SIMD::FP<uint32_t>::Size; ++i )
            {
              indices[i] = ((*this)[ ix[i] ])[ iy[i] ];
            }
            return indices;
          }
          /// Set the mirror for a given bin
          void set( const uint32_t ix,
                    const uint32_t iy,
                    const uint8_t im )
          {
            ((*this)[ix])[iy] = im;
          }
          /// Clear the table
          void clear() { for ( auto & i : *this ) { i.fill(0); } }
        };
      private:
        /// Get the number of bins in X (Scalar)
        constexpr uint32_t nXBins() const noexcept { return NXBINS;  }
        /// Get the number of bins in Y (Scalar)
        constexpr uint32_t nYBins() const noexcept { return NYBINS;  }
        /// Get the minimum range in X (Scalar)
        FPTYPE               minX() const noexcept { return m_minX; }
        /// Get the maximum range in X (Scalar)
        FPTYPE               maxX() const noexcept { return m_maxX; }
        /// Get the minimum range in Y (Scalar)
        FPTYPE               minY() const noexcept { return m_minY; }
        /// Get the maximum range in Y (Scalar)
        FPTYPE               maxY() const noexcept { return m_maxY; }
      private:
        /// Get x for a given index value
        inline FPTYPE binX( const uint32_t i ) const noexcept
        {
          return minX() + (i/m_incX) + (0.5/m_incX);
        }
        /// Get y for a given index value
        inline FPTYPE binY( const uint32_t i ) const noexcept
        {
          return m_minY + (i/m_incY) + (0.5/m_incY);
        }
      private:
        /// Get the x index (Scalar)
        template< typename TYPE,
                  typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline uint32_t xIndex( const TYPE x ) const noexcept
        {
          return ( x < minX() ? 0            :
                   x > maxX() ? nXBins() - 1 :
                   (uint32_t)((x-minX())*m_incX) );
        }
        /// Get the x index (SIMD)
        template< typename TYPE,
                  typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline SIMD::FP<uint32_t> xIndex( TYPE x ) const noexcept
        {
          // Check for over and under flows
          x( x < m_minXSIMD ) = m_minXSIMD;
          x( x > m_maxXSIMD ) = m_maxXSIMD;
          // form indices
          return Vc::simd_cast< SIMD::FP<uint32_t> >( ( x - m_minXSIMD ) * m_incXSIMD );
        }
        /// Get the y index (Scalar)
        template< typename TYPE,
                  typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline uint32_t yIndex( const TYPE y ) const noexcept
        {
          return ( y < minY() ? 0            :
                   y > maxY() ? nYBins() - 1 :
                   (uint32_t)((y-minY())*m_incY) );
        }
        /// Get the y index (SIMD)
        template< typename TYPE,
                  typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline SIMD::FP<uint32_t> yIndex( TYPE y ) const noexcept
        {
          // Check for over and under flows
          y( y < m_minYSIMD ) = m_minYSIMD;
          y( y > m_maxYSIMD ) = m_maxYSIMD;
          // form indices
          return Vc::simd_cast< SIMD::FP<uint32_t> >( ( y - m_minYSIMD ) * m_incYSIMD );
        }
      private:
        SIMD::FP<SIMDTYPE> m_minXSIMD; ///< Minimum X (SIMD)
        SIMD::FP<SIMDTYPE> m_maxXSIMD; ///< Maximum X (SIMD)
        SIMD::FP<SIMDTYPE> m_minYSIMD; ///< Minimum Y (SIMD)
        SIMD::FP<SIMDTYPE> m_maxYSIMD; ///< Maximum Y (SIMD)
        SIMD::FP<SIMDTYPE> m_incXSIMD; ///< 1 / Increment in X
        SIMD::FP<SIMDTYPE> m_incYSIMD; ///< 1 / Increment in Y
      private:
        FPTYPE m_minX{ 9e9};         ///< Minimum X
        FPTYPE m_maxX{-9e9};         ///< Maximum X
        FPTYPE m_minY{ 9e9};         ///< Minimum Y
        FPTYPE m_maxY{-9e9};         ///< Maximum Y
        FPTYPE m_incX{0};            ///< 1 / Increment in X
        FPTYPE m_incY{0};            ///< 1 / Increment in Y
        LookupTable m_lookupTable;   ///< The lookup table
      public:
        /// Vector of all mirrors in this panel
        Mirrors mirrors;
      private:
        /// Extra size to take into account mirror extents
        FPTYPE m_eSize{0};  
      };

      /// Specialisation for RICH1
      template < typename FPTYPE, uint32_t NXBINS, uint32_t NYBINS, typename SIMDTYPE = float >
      class R1LookupTableFinder final : public LookupTableFinder<FPTYPE,NXBINS,NYBINS,SIMDTYPE>
      {
      public:
        R1LookupTableFinder() : LookupTableFinder<FPTYPE,NXBINS,NYBINS,SIMDTYPE>(100*Gaudi::Units::mm) { }
      };

      /// Specialisation for RICH2
      template < typename FPTYPE, uint32_t NXBINS, uint32_t NYBINS, typename SIMDTYPE = float >
      class R2LookupTableFinder final : public LookupTableFinder<FPTYPE,NXBINS,NYBINS,SIMDTYPE>
      {
      public:
        R2LookupTableFinder() : LookupTableFinder<FPTYPE,NXBINS,NYBINS,SIMDTYPE>(2500*Gaudi::Units::mm) { }
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
      class TwoSegmentXFinder final
      {
      public:
        /// Reset this object
        void reset()
        {
          mirrors.clear();
        }
        /// Initialise this object
        void init()
        {
          // Must have 2 mirrors in -ve and +ve x positions
          if ( 2 != mirrors.size() || ( mirrors[0]->mirrorCentre().X() * 
                                        mirrors[1]->mirrorCentre().X() > 0 ) )
          {
            throw GaudiException( "Problem with mirror segments", 
                                  "MirrorSegFinderLookUpTable::TwoSegmentXFinder",
                                  StatusCode::FAILURE );
          }
          // sort the mirrors so the -ve X one is in [0] and +ve in [1]
          std::sort( mirrors.begin(), mirrors.end(),
                     []( const auto m1, const auto m2 )
                     { return ( m1->mirrorCentre().X() <
                                m2->mirrorCentre().X() ); } );
        }
        /// Find the mirror for a given position (Scalar)
        template< typename POINT, 
                  typename std::enable_if<std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
        inline const DeRichSphMirror* find( const POINT& p ) const noexcept
        {
          return find( p.x() );
        }
        /// Find the mirror for a given position (SIMD)
        template< typename POINT, 
                  typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
        inline SIMDMirrors<typename POINT::Scalar> find( const POINT& p ) const noexcept
        {
          return find( p.x() );
        }
        /// Find the mirror for a given position (x,y) {Scalar)
        template< typename TYPE, 
                  typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline const DeRichSphMirror* find( const TYPE x, const TYPE /* y */ ) const noexcept
        {
          return find( x );
        }     
        /// Find the mirror for a given position (x,y) {SIMD)
        template< typename TYPE, 
                  typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline SIMDMirrors<TYPE> find( const TYPE x, const TYPE /* y */ ) const noexcept
        {
          return find( x );
        }     
        /// Find the mirror for a given position (x,y) {Scalar)
        template< typename TYPE, 
                  typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline const DeRichSphMirror* find( const TYPE x ) const noexcept
        {
          return ( x < 0 ? mirrors[0] : mirrors[1] );
        }     
        /// Find the mirror for a given position (x,y) {SIMD)
        template< typename TYPE, 
                  typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline SIMDMirrors<TYPE> find( const TYPE x ) const noexcept
        {
          // revert to scalar loop here... Is there a way to avoid this ?
          SIMDMirrors<TYPE> mirrs;
          for ( std::size_t i = 0; i < TYPE::Size; ++i )
          { mirrs[i] = ( x[i] < 0 ? mirrors[0] : mirrors[1] ); }
          return mirrs;
        }     
      public:
        /// Vector of all mirrors in this panel
        Mirrors mirrors;
      };

      /// Overall RICH mirror finder
      template < typename R1FINDER, typename R2FINDER >
      class RichMirrorFinder
      {
      public:
        /// Reset the finders
        void reset() 
        {
          m_r1Finder[Rich::top]   .reset();
          m_r1Finder[Rich::bottom].reset();
          m_r2Finder[Rich::left]  .reset();
          m_r2Finder[Rich::right] .reset();
        }
        /// Validity check
        bool isOK() const
        {
          return ( !m_r1Finder[Rich::top]   .mirrors.empty() &&
                   !m_r1Finder[Rich::bottom].mirrors.empty() &&
                   !m_r2Finder[Rich::left]  .mirrors.empty() &&
                   !m_r2Finder[Rich::right] .mirrors.empty() );
        }
        /// Add a mirror to the given RICH and side
        void addMirror( const Rich::DetectorType rich,
                        const Rich::Side side,
                        const DeRichSphMirror * m )
        {
          if ( Rich::Rich1 == rich )
          { m_r1Finder[side].mirrors.push_back( m ); }
          else
          { m_r2Finder[side].mirrors.push_back( m ); }
        }
        /// Initialise
        void init( )
        {
          m_r1Finder[Rich::top]   .init();
          m_r1Finder[Rich::bottom].init(); 
          m_r2Finder[Rich::left]  .init();
          m_r2Finder[Rich::right] .init(); 
        }
        /// Find the mirror for the given RICH, panel and point (Scalar)
        template< typename POINT,
                  typename std::enable_if<std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
        inline const DeRichSphMirror* find( const Rich::DetectorType rich,
                                            const Rich::Side side,
                                            const POINT& p ) const noexcept
        {
          return ( Rich::Rich1 == rich ?
                   m_r1Finder[side].find(p) : m_r2Finder[side].find(p) );
        }
        /// Find the mirror for the given RICH, panel and point (SIMD)
        template< typename POINT,
                  typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
        inline SIMDMirrors<typename POINT::Scalar> find( const Rich::DetectorType rich,
                                                         const Rich::Side side,
                                                         const POINT& p ) const noexcept
        {
          return ( Rich::Rich1 == rich ?
                   m_r1Finder[side].find(p) : m_r2Finder[side].find(p) );
        }
        /// Find the mirror for the given RICH, panel and point (x,y) (Scalar)
        template< typename TYPE,
                  typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline const DeRichSphMirror* find( const Rich::DetectorType rich,
                                            const Rich::Side side,
                                            const TYPE x, const TYPE y ) const noexcept
        {
          return ( Rich::Rich1 == rich ?
                   m_r1Finder[side].find(x,y) : m_r2Finder[side].find(x,y) );
        }
        /// Find the mirror for the given RICH, panel and point (x,y) (SIMD)
        template< typename TYPE,
                  typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
        inline SIMDMirrors<TYPE> find( const Rich::DetectorType rich,
                                       const Rich::Side side,
                                       const TYPE x, const TYPE y ) const noexcept
        {
          return ( Rich::Rich1 == rich ?
                   m_r1Finder[side].find(x,y) : m_r2Finder[side].find(x,y) );
        }
        /// Get the list of mirrors
        const Mirrors& mirrors( const Rich::DetectorType rich,
                                const Rich::Side side ) const noexcept
        {
          return ( Rich::Rich1 == rich ?
                   m_r1Finder[side].mirrors : m_r2Finder[side].mirrors );
        }
      private:
        /// RICH1 finder
        PanelArray<R1FINDER> m_r1Finder;
        /// RICH2 finder
        PanelArray<R2FINDER> m_r2Finder;
      };

    protected:

      // Finder types for each RICH and mirror type
      using R1Primary   = TwoSegmentXFinder;
      using R2Primary   = R2LookupTableFinder<double,400,400>;
      using R1Secondary = R1LookupTableFinder<double,200,100>;
      using R2Secondary = R2LookupTableFinder<double,400,400>;
      
      /// Primary mirrors
      RichMirrorFinder<R1Primary,R2Primary> m_sphMirrFinder;
      
      /// Secondary mirrors
      RichMirrorFinder<R1Secondary,R2Secondary> m_secMirrFinder;

    public:
      
      /** Locates the spherical mirror Segment given a reflection point,
       *  RICH identifier and panel
       *
       *  Scalar implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the spherical mirror
       *
       *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
       */
      template< typename POINT,
                typename std::enable_if<std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
      inline const DeRichSphMirror*
      findSphMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const POINT& reflPoint ) const noexcept
      { 
        // Find the mirror from the lookup map
        return m_sphMirrFinder.find( rich, side, reflPoint );
      }

      /** Locates the spherical mirror Segment given a reflection point,
       *  RICH identifier and panel
       *
       *  SIMD implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the spherical mirror
       *
       *  @return Array of DeRichSphMirror objects for the associated mirror segments
       */
      template< typename POINT,
                typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
      inline SIMDMirrors<typename POINT::Scalar>
      findSphMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const POINT& reflPoint ) const noexcept
      { 
        //std::cout << "Point " << reflPoint << std::endl;
        // Find the mirror from the lookup map
        return m_sphMirrFinder.find( rich, side, reflPoint );
      }
      
      /** Locates the secondary (spherical) mirror Segment given a reflection point,
       *  RICH identifier and panel
       *
       *  Scalar implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the secondary mirror
       *
       *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
       */
      template< typename POINT,
                typename std::enable_if<std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
      inline const DeRichSphMirror*
      findSecMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const POINT& reflPoint ) const noexcept
      {
        // Find the mirror from the lookup map
        return m_secMirrFinder.find( rich, side, reflPoint );
      }

      /** Locates the secondary (spherical) mirror Segment given a reflection point,
       *  RICH identifier and panel
       *
       *  SIMD implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the secondary mirror
       *
       *  @return Array of DeRichSphMirror objects for the associated mirror segments
       */
      template< typename POINT,
                typename std::enable_if<!std::is_arithmetic<typename POINT::Scalar>::value>::type * = nullptr >
      inline SIMDMirrors<typename POINT::Scalar>
      findSecMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const POINT& reflPoint ) const noexcept
      {
        // Find the mirror from the lookup map
        return m_secMirrFinder.find( rich, side, reflPoint );
      }

    public:

      /** Locates the spherical mirror Segment given a reflection point,
       *  RICH identifier and panel  
       *
       *  Scalar implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the spherical mirror
       *
       *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
       */
      template< typename TYPE,
                typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
      inline const DeRichSphMirror*
      findSphMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const TYPE x, const TYPE y ) const noexcept
      { 
        // Find the mirror from the lookup map
        return m_sphMirrFinder.find( rich, side, x, y );
      }

      /** Locates the spherical mirror Segment given a reflection point,
       *  RICH identifier and panel  
       *
       *  SIMD implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the spherical mirror
       *
       *  @return Array of DeRichSphMirror objects for the associated mirror segments
       */
      template< typename TYPE,
                typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
      inline SIMDMirrors<TYPE>
      findSphMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const TYPE x, const TYPE y ) const noexcept
      { 
        // Find the mirror from the lookup map
        return m_sphMirrFinder.find( rich, side, x, y );
      }
      
      /** Locates the secondary (spherical) mirror Segment given a reflection point,
       *  RICH identifier and panel
       *
       *  Scalar implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the secondary mirror
       *
       *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
       */
      template< typename TYPE,
                typename std::enable_if<std::is_arithmetic<TYPE>::value>::type * = nullptr >
      inline const DeRichSphMirror*
      findSecMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const TYPE x, const TYPE y ) const noexcept
      {
        // Find the mirror from the lookup map
        return m_secMirrFinder.find( rich, side, x, y );
      }

      /** Locates the secondary (spherical) mirror Segment given a reflection point,
       *  RICH identifier and panel
       *
       *  SIMD implementation
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the secondary mirror
       *
       *  @return Array of DeRichSphMirror objects for the associated mirror segments
       */
      template< typename TYPE,
                typename std::enable_if<!std::is_arithmetic<TYPE>::value>::type * = nullptr >
      inline SIMDMirrors<TYPE>
      findSecMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const TYPE x, const TYPE y ) const noexcept
      {
        // Find the mirror from the lookup map
        return m_secMirrFinder.find( rich, side, x, y );
      }
      
    };

 }
}
