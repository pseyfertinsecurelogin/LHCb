#ifndef MAGFIELDGRID_H
#define MAGFIELDGRID_H

// STD
#include <vector>
#include <array>
#include <cmath>

// Gaudi
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/GenericMatrixTypes.h"

// VectorClass
#include "VectorClass/vectorclass.h"

class MagneticFieldGridReader ;

namespace LHCb
{

  /** @class MagneticFieldGrid MagneticFieldGrid.h
   *  Simple class to hold a magnetic field grid.
   *
   *  @author Marco Cattaneo, Wouter Hulsbergen
   *  @date   2008-07-26
   */
  class MagneticFieldGrid final
  {

  public:

    /// typedefs etc
    typedef Gaudi::XYZVector FieldVector ;
    typedef Gaudi::Matrix3x3 FieldGradient ;

    /// declare the field reader as friend so that we don't have to make a constructor
    friend class ::MagneticFieldGridReader;

    /// Return the field vector fvec at the point xyz by interpolation
    /// on the grid.
    FieldVector fieldVector( const Gaudi::XYZPoint& xyz ) const;

    /// Return the field vector fvec at the point xyz by interpolation
    /// on the grid.
    FieldGradient fieldGradient( const Gaudi::XYZPoint& xyz ) const;

    /// Return the field vector fvec at the point xyz by choosing the
    /// closest point on the grid.
    FieldVector fieldVectorClosestPoint( const Gaudi::XYZPoint& xyz ) const ;

    /// Return the magnetic field scale factor
    double scaleFactor() const noexcept { return m_scaleFactor ; }

    /// Update the scale factor
    void setScaleFactor( const double& s )
    {
      m_scaleFactor = s ;
      m_scaleFactor_V = Vec4f( s, s, s, 0. );
    }

  private:

    double m_scaleFactor = 1; ///< The scale factor

    Vec4f m_scaleFactor_V = { 1,1,1,0 };       ///< Vector version of the scale factor
    std::vector<Vec4f> m_Q_V;                  ///< Vectorised Field map
    Vec4f   m_min_FL_V = { 0.,0.,0.,0. };      ///< Offset in x, y and z
    Vec4f   m_Dxyz_V = { 0.,0.,0.,0. };        ///< Steps in x, y and z
    Vec4f   m_invDxyz_V = { 0.,0.,0.,0. };     ///< Inverse of steps in x, y and z (cached for speed)
    std::array<unsigned,3> m_Nxyz_V = {0,0,0}; ///< Number of steps in x, y and z

  };

}

#endif // MAGFIELDGRID_H
