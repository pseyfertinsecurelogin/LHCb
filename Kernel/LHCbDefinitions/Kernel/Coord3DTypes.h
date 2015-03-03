// $Id: Coord3DTypes.h,v 1.3 2006-08-01 08:52:35 jonrob Exp $
#ifndef KERNEL_COORD3DTYPES_H 
#define KERNEL_COORD3DTYPES_H 1

// Include files

/** @namespace Gaudi Coord3DTypes.h Kernel/Coord3DTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-12-05
 */
namespace Gaudi {
  typedef ROOT::Math::Cartesian3D<double>      Cartesian3D;
  typedef ROOT::Math::Cartesian3D<float>       Cartesian3DF;
  typedef ROOT::Math::Polar3D<double>          Polar3D;
  typedef ROOT::Math::Polar3D<float>           Polar3DF;
  typedef ROOT::Math::Cylindrical3D<double>    RhoPhiZ3D;
  typedef ROOT::Math::Cylindrical3D<float>     RhoPhiZ3DF;
  typedef ROOT::Math::CylindricalEta3D<double> RhoEtaPhi3D;
  typedef ROOT::Math::CylindricalEta3D<float>  RhoEtaPhi3DF;
};
#endif // KERNEL_COORD3DTYPES_H
