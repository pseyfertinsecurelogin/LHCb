
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
      
      //-----------------------------------------------------------------------------
      /** @class LookupTableFinder RichMirrorSegFinderLookUpTable.h
       *
       *  Helper object implementing a lookup table based mirror search
       *
       *  @author Chris Jones
       *  @date   2015-02-01
       */
      //-----------------------------------------------------------------------------
      template < unsigned int NXBINS, unsigned int NYBINS >
      class LookupTableFinder
      {
      public:  
        /// Constructor from extra size
        explicit LookupTableFinder( const double eSize ) : m_eSize(eSize) { }
        /// Default constructor not allowed
        LookupTableFinder() = delete;
      public:
        /// Reset this object
        void reset()
        {
          mirrors.clear();
          m_lookupTable.clear();
          m_minX = m_minY =  9e9;
          m_maxX = m_maxY = -9e9;
          m_incX = m_incY =  0.0;
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
          m_incX = (double)nXBins() / ( m_maxX - m_minX );
          m_incY = (double)nYBins() / ( m_maxY - m_minY );

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
        /// Find the mirror for a given position
        template< typename POINT >
        inline const DeRichSphMirror* find( const POINT& p ) const noexcept
        {
          return mirrors[ m_lookupTable.get( xIndex(p.x()), yIndex(p.y()) ) ];
        }
        /// Find the mirror for a given position (x,y)
        template< typename TYPE >
        inline const DeRichSphMirror* find( const TYPE x, const TYPE y ) const noexcept
        {
          return mirrors[ m_lookupTable.get( xIndex(x), yIndex(y) ) ];
        }
      private:
        /// Get the mirror closest to a given (x,y) point
        const DeRichSphMirror* closestXY( const double x, const double y ) const
        {
          const DeRichSphMirror* minM = nullptr;  
          // Loop over all the mirrors to find the closest
          double minDist(9e99);
          for ( const auto* M : mirrors )
          {
            const auto dist = distance2( x, y, M );
            if ( dist < minDist ) { minM = M; minDist = dist; }
          }
          return minM;
        }
        /// Returns the 2D (x,y) distance^2 between a given point and mirror centre
        static inline double distance2( const double x, 
                                        const double y,
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
          /// Access the mirror for a given set of indices
          inline uint8_t get( const unsigned int ix,
                              const unsigned int iy ) const noexcept
          {
            return ((*this)[ix])[iy];
          }
          /// Set the mirror for a given bin
          void set( const unsigned int ix,
                    const unsigned int iy,
                    const uint8_t im )
          {
            ((*this)[ix])[iy] = im;
          }
          /// Clear the table
          void clear() { for ( auto & i : *this ) { i.fill(0); } }
        };
      private:
        /// Get the number of bins in X
        constexpr unsigned int nXBins() const noexcept { return NXBINS;  }
        /// Get the number of bins in Y
        constexpr unsigned int nYBins() const noexcept { return NYBINS;  }
        /// Get the minimum range in X
        double                   minX() const noexcept { return m_minX; }
        /// Get the maximum range in X
        double                   maxX() const noexcept { return m_maxX; }
        /// Get the minimum range in Y
        double                   minY() const noexcept { return m_minY; }
        /// Get the maximum range in Y
        double                   maxY() const noexcept { return m_maxY; }
      private:
        /// Get x for a given index value
        inline double binX( const unsigned int i ) const noexcept
        {
          return minX() + (i/m_incX) + (0.5/m_incX);
        }
        /// Get y for a given index value
        inline double binY( const unsigned int i ) const noexcept
        {
          return m_minY + (i/m_incY) + (0.5/m_incY);
        }
        /// Get the x index
        template< typename TYPE >
        inline unsigned int xIndex( const TYPE x ) const noexcept
        {
          return ( x < minX() ? 0            :
                   x > maxX() ? nXBins() - 1 :
                   (unsigned int)((x-minX())*m_incX) );
        }
        /// Get the y index
        template< typename TYPE >
        inline unsigned int yIndex( const TYPE y ) const noexcept
        {
          return ( y < minY() ? 0            :
                   y > maxY() ? nYBins() - 1 :
                   (unsigned int)((y-minY())*m_incY) );
        }
      private:
        double m_minX{ 9e9};       ///< Minimum X
        double m_maxX{-9e9};       ///< Maximum X
        double m_minY{ 9e9};       ///< Minimum Y
        double m_maxY{-9e9};       ///< Maximum Y
        double m_incX{0};          ///< 1 / Increment in X
        double m_incY{0};          ///< 1 / Increment in Y
        LookupTable m_lookupTable; ///< The lookup table
      public:
        /// Vector of all mirrors in this panel
        Mirrors mirrors;
      private:
        /// Extra size to take into account mirror extents
        double m_eSize{0};  
      };

      /// Specialisation for RICH1
      template < unsigned int NXBINS, unsigned int NYBINS >
      class R1LookupTableFinder final : public LookupTableFinder<NXBINS,NYBINS>
      {
      public:
        R1LookupTableFinder() : LookupTableFinder<NXBINS,NYBINS>(100*Gaudi::Units::mm) { }
      };

      /// Specialisation for RICH2
      template < unsigned int NXBINS, unsigned int NYBINS >
      class R2LookupTableFinder final : public LookupTableFinder<NXBINS,NYBINS>
      {
      public:
        R2LookupTableFinder() : LookupTableFinder<NXBINS,NYBINS>(2500*Gaudi::Units::mm) { }
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
        /// Find the mirror for a given position
        template< typename POINT >
        inline const DeRichSphMirror* find( const POINT& p ) const noexcept
        {
          return ( p.x() < 0 ? mirrors[0] : mirrors[1] );
        }
        /// Find the mirror for a given position (x,y)
        template< typename TYPE >
        inline const DeRichSphMirror* find( const TYPE x, const TYPE /* y */ ) const noexcept
        {
          return ( x < 0 ? mirrors[0] : mirrors[1] );
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
        /// Find the mirror for the given RICH, panel and point
        template< typename POINT >
        inline const DeRichSphMirror* find( const Rich::DetectorType rich,
                                            const Rich::Side side,
                                            const POINT& p ) const noexcept
        {
          return ( Rich::Rich1 == rich ?
                   m_r1Finder[side].find(p) : m_r2Finder[side].find(p) );
        }
        /// Find the mirror for the given RICH, panel and point (x,y)
        template< typename TYPE >
        inline const DeRichSphMirror* find( const Rich::DetectorType rich,
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
      using R2Primary   = R2LookupTableFinder<400,400>;
      using R1Secondary = R1LookupTableFinder<200,100>;
      using R2Secondary = R2LookupTableFinder<400,400>;
      
      /// Primary mirrors
      RichMirrorFinder<R1Primary,R2Primary> m_sphMirrFinder;
      
      /// Secondary mirrors
      RichMirrorFinder<R1Secondary,R2Secondary> m_secMirrFinder;

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
      template< typename POINT >
      inline const DeRichSphMirror*
      findSphMirror( const Rich::DetectorType rich,
                     const Rich::Side side,
                     const POINT& reflPoint ) const noexcept
      { 
        // Find the mirror from the lookup map
        return m_sphMirrFinder.find( rich, side, reflPoint );
      }
      
      /** Locates the secondary (spherical) mirror Segment given a reflection point,
       *  RICH identifier and panel
       *
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the secondary mirror
       *
       *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
       */
      template< typename POINT >
      inline const DeRichSphMirror*
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
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the spherical mirror
       *
       *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
       */
      template< typename TYPE >
      inline const DeRichSphMirror*
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
       *  @param rich       The RICH detector
       *  @param side       The RICH HPD panel side
       *  @param reflPoint  The reflection point on the secondary mirror
       *
       *  @return Const pointer to DeRichSphMirror object for the associated mirror segment
       */
      template< typename TYPE >
      inline const DeRichSphMirror*
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
