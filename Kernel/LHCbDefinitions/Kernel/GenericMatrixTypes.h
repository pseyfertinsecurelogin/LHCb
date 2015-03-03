// $Id: GenericMatrixTypes.h,v 1.6 2006-09-26 15:40:35 jpalac Exp $
#ifndef KERNEL_GENERICMATRIXTYPES_H 
#define KERNEL_GENERICMATRIXTYPES_H 1

// Include files
#include "Math/SMatrix.h"
/** @namespace Gaudi GenericMatrixTypes.h Kernel/GenericMatrixTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-12-08
 */
namespace Gaudi {

  typedef ROOT::Math::SMatrix<double, 1, 1> Matrix1x1;
  typedef ROOT::Math::SMatrix<double, 2, 2> Matrix2x2;
  typedef ROOT::Math::SMatrix<double, 3, 3> Matrix3x3;
  typedef ROOT::Math::SMatrix<double, 4, 4> Matrix4x4;
  typedef ROOT::Math::SMatrix<double, 5, 5> Matrix5x5;
  typedef ROOT::Math::SMatrix<double, 6, 6> Matrix6x6;
  typedef ROOT::Math::SMatrix<double, 7, 7> Matrix7x7;
  typedef ROOT::Math::SMatrix<double, 8, 8> Matrix8x8;
  typedef ROOT::Math::SMatrix<double, 9, 9> Matrix9x9;

  typedef ROOT::Math::SMatrix<double, 1, 5> Matrix1x5;
  typedef ROOT::Math::SMatrix<double, 4, 3> Matrix4x3;
  typedef ROOT::Math::SMatrix<double, 3, 4> Matrix3x4;
  typedef ROOT::Math::SMatrix<double, 2, 3> Matrix2x3;
  typedef ROOT::Math::SMatrix<double, 3, 2> Matrix3x2;

  typedef ROOT::Math::SMatrix<float, 1, 1> Matrix1x1F;
  typedef ROOT::Math::SMatrix<float, 2, 2> Matrix2x2F;
  typedef ROOT::Math::SMatrix<float, 3, 3> Matrix3x3F;
  typedef ROOT::Math::SMatrix<float, 4, 4> Matrix4x4F;
  typedef ROOT::Math::SMatrix<float, 5, 5> Matrix5x5F;
  typedef ROOT::Math::SMatrix<float, 6, 6> Matrix6x6F;
  typedef ROOT::Math::SMatrix<float, 7, 7> Matrix7x7F;
  typedef ROOT::Math::SMatrix<float, 8, 8> Matrix8x8F;
  typedef ROOT::Math::SMatrix<float, 9, 9> Matrix9x9F;

  typedef ROOT::Math::SMatrix<float, 1, 5> Matrix1x5F;
  typedef ROOT::Math::SMatrix<float, 4, 3> Matrix4x3F;
  typedef ROOT::Math::SMatrix<float, 3, 4> Matrix3x4F;
  typedef ROOT::Math::SMatrix<float, 2, 3> Matrix2x3F;
  typedef ROOT::Math::SMatrix<float, 3, 2> Matrix3x2F;

};
#endif // KERNEL_GENERICMATRIXTYPES_H
