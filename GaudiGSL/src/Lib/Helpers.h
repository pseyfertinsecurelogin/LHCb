/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef GAUDIMATH_HELPERS_H
#define GAUDIMATH_HELPERS_H 1
// ============================================================================
// Include files
// ============================================================================

/** @file
 *
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2003-09-01
 */

// ============================================================================
namespace Genfun {
  namespace GaudiMathImplementation {
    /** @class GSL_Helper
     *  the simple structure  to be used
     *  for adaption interface <tt>Genfun::AbsFunction</tt>
     *  to <tt>gsl_function</tt> structure
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GSL_Helper final {
    public:
      /// constructor
      GSL_Helper( const Genfun::AbsFunction& function, Genfun::Argument& argument, const size_t index );
      GSL_Helper& operator=( GSL_Helper& ) = delete;

    public:
      const Genfun::AbsFunction& m_function;
      Genfun::Argument&          m_argument;
      size_t                     m_index;
    };

    double GSL_Adaptor( double x, void* params );

  } // end of namespace GaudiMathImplementation
} // end of namespace Genfun
// ============================================================================

// ============================================================================
// The END
// ============================================================================
#endif // GAUDIMATH_HELPERS_H
