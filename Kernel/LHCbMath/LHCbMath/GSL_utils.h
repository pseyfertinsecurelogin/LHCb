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
#ifndef LHCBMATH_GSL_UTILS_H
#define LHCBMATH_GSL_UTILS_H 1
// ============================================================================
// STL
// ============================================================================
#include <ostream>
// ============================================================================
// Gaudi
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// GSL
// ============================================================================
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
// =============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace Utils
  {
    // ========================================================================
    /** print GSL-vector to the stream
     *  @param v the vector
     *  @param s the stream
     *  @return the stream
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2012-05-28
     */
    GAUDI_API
    std::ostream& toStream
    ( const gsl_vector&  v ,
      std::ostream&      s ) ;
    // ========================================================================
    /** print GSL-matrix to the stream
     *  @param m the matrix
     *  @param s the stream
     *  @return the stream
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2012-05-28
     */
    GAUDI_API
    std::ostream& toStream
    ( const gsl_matrix&  m ,
      std::ostream&      s ) ;
    // ========================================================================
  } //                                            end of namespace Gaudi::Utils
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
/// print operator
inline
std::ostream& operator<<( std::ostream& s , const gsl_vector& v )
{ return Gaudi::Utils::toStream ( v , s ) ; }
// ============================================================================
/// print operator
inline
std::ostream& operator<<( std::ostream& s , const gsl_matrix& m )
{ return Gaudi::Utils::toStream ( m , s ) ; }
// ============================================================================
// The END
// ============================================================================
#endif // LHCBMATH_GSL_UTILS_H
