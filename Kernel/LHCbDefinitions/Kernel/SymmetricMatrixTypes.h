#ifndef KERNEL_SYMMETRICMATRIXTYPES_H 
#define KERNEL_SYMMETRICMATRIXTYPES_H 1

// Include files
#include "Math/SMatrix.h"
/** @namespace Gaudi SymmetricMatrixTypes.h Kernel/SymmetricMatrixTypes.h
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2005-12-20
 *
 * Temporary typedefs for symmetric matrices pointing to standard 
 * square matrices.
 * @todo Replace with symmetric matrices when implemented inSMatrix
 */
namespace Gaudi {

  typedef ROOT::Math::SMatrix<double, 1, 1, 
                              ROOT::Math::MatRepSym<double,1> > SymMatrix1x1;
  typedef ROOT::Math::SMatrix<double, 2, 2,
                              ROOT::Math::MatRepSym<double,2> > SymMatrix2x2;
  typedef ROOT::Math::SMatrix<double, 3, 3,
                              ROOT::Math::MatRepSym<double,3> > SymMatrix3x3;
  typedef ROOT::Math::SMatrix<double, 4, 4,
                              ROOT::Math::MatRepSym<double,4> > SymMatrix4x4;
  typedef ROOT::Math::SMatrix<double, 5, 5,
                              ROOT::Math::MatRepSym<double,5> > SymMatrix5x5;
  typedef ROOT::Math::SMatrix<double, 6, 6,
                              ROOT::Math::MatRepSym<double,6> > SymMatrix6x6;
  typedef ROOT::Math::SMatrix<double, 7, 7,
                              ROOT::Math::MatRepSym<double,7> > SymMatrix7x7;
  typedef ROOT::Math::SMatrix<double, 8, 8,
                              ROOT::Math::MatRepSym<double,8> > SymMatrix8x8;
  typedef ROOT::Math::SMatrix<double, 9, 9,
                              ROOT::Math::MatRepSym<double,9> > SymMatrix9x9;


  typedef ROOT::Math::SMatrix<float, 1, 1, 
                              ROOT::Math::MatRepSym<float,1> > SymMatrix1x1F;
  typedef ROOT::Math::SMatrix<float, 2, 2,
                              ROOT::Math::MatRepSym<float,2> > SymMatrix2x2F;
  typedef ROOT::Math::SMatrix<float, 3, 3,
                              ROOT::Math::MatRepSym<float,3> > SymMatrix3x3F;
  typedef ROOT::Math::SMatrix<float, 4, 4,
                              ROOT::Math::MatRepSym<float,4> > SymMatrix4x4F;
  typedef ROOT::Math::SMatrix<float, 5, 5,
                              ROOT::Math::MatRepSym<float,5> > SymMatrix5x5F;
  typedef ROOT::Math::SMatrix<float, 6, 6,
                              ROOT::Math::MatRepSym<float,6> > SymMatrix6x6F;
  typedef ROOT::Math::SMatrix<float, 7, 7,
                              ROOT::Math::MatRepSym<float,7> > SymMatrix7x7F;
  typedef ROOT::Math::SMatrix<float, 8, 8,
                              ROOT::Math::MatRepSym<float,8> > SymMatrix8x8F;
  typedef ROOT::Math::SMatrix<float, 9, 9,
                              ROOT::Math::MatRepSym<float,9> > SymMatrix9x9F;



};
#endif // KERNEL_SYMMETRICMATRIXTYPES_H
