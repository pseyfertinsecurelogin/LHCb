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
#ifndef LHCBMATH_EQUALTO_H
#define LHCBMATH_EQUALTO_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Vector4DTypes.h"
// ============================================================================
// Root
// ============================================================================
#include "Math/SMatrix.h"
#include "Math/SVector.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/LHCbMath.h"
// ============================================================================
namespace LHCb {
  // ==========================================================================
  namespace Math {
    // ========================================================================
    /** specialisation for 4D-vectors
     *  @see Gaudi::LorentzVector
     *  @see LHCb::Math::mULPS_double
     *  @see Gaudi::Math::Lomont
     *  @see Gaudi::Math::Lomont<double>
     */
    template <>
    struct Equal_To<Gaudi::LorentzVector> {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_double
       */
      Equal_To( const unsigned int eps = mULPS_double ) : m_cmp( eps ) {}
      // ======================================================================
      /// comparison:
      inline bool operator()( const Gaudi::LorentzVector& v1, const Gaudi::LorentzVector& v2 ) const {
        return &v1 == &v2 || ( m_cmp( v1.Px(), v2.Px() ) && m_cmp( v1.Py(), v2.Py() ) && m_cmp( v1.Pz(), v2.Pz() ) &&
                               m_cmp( v1.E(), v2.E() ) );
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the evaluator
      Equal_To<double> m_cmp; // the evaluator
      // ======================================================================
    };
    // ========================================================================
    /** specialisation for 3D-vectors
     *  @see Gaudi::XYZVector
     *  @see LHCb::Math::mULPS_double
     *  @see Gaudi::Math::Lomont
     *  @see Gaudi::Math::Lomont<double>
     */
    template <>
    struct Equal_To<Gaudi::XYZVector> {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_double
       */
      Equal_To( const unsigned int eps = mULPS_double ) : m_cmp( eps ) {}
      // ======================================================================
      /// comparison:
      inline bool operator()( const Gaudi::XYZVector& v1, const Gaudi::XYZVector& v2 ) const {
        return &v1 == &v2 || ( m_cmp( v1.X(), v2.X() ) && m_cmp( v1.Y(), v2.Y() ) && m_cmp( v1.Z(), v2.Z() ) );
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the evaluator
      Equal_To<double> m_cmp; // the evaluator
      // ======================================================================
    };
    // ========================================================================
    /** specialisation for 3D-points
     *  @see Gaudi::XYZVector
     *  @see LHCb::Math::mULPS_double
     *  @see Gaudi::Math::Lomont
     *  @see Gaudi::Math::Lomont<double>
     */
    template <>
    struct Equal_To<Gaudi::XYZPoint> {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_double
       */
      Equal_To( const unsigned int eps = mULPS_double ) : m_cmp( eps ) {}
      // ======================================================================
      /// comparison:
      inline bool operator()( const Gaudi::XYZPoint& v1, const Gaudi::XYZPoint& v2 ) const {
        return &v1 == &v2 || ( m_cmp( v1.X(), v2.X() ) && m_cmp( v1.Y(), v2.Y() ) && m_cmp( v1.Z(), v2.Z() ) );
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the evaluator
      Equal_To<double> m_cmp; // the evaluator
      // ======================================================================
    };
    // ========================================================================
    /// specialisation for linear algebra vectors
    template <class T, unsigned int D>
    struct Equal_To<ROOT::Math::SVector<T, D>> {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_double
       */
      Equal_To( const unsigned int eps = mULPS_double ) : m_cmp( eps ) {}
      // ======================================================================
      /// comparison:
      inline bool operator()( const ROOT::Math::SVector<T, D>& v1, const ROOT::Math::SVector<T, D>& v2 ) const {
        return &v1 == &v2 || std::equal( v1.begin(), v1.end(), v2.begin(), m_cmp );
      }
      // ======================================================================
      /// comparison with another vector type
      template <class T2>
      inline bool operator()( const ROOT::Math::SVector<T, D>& v1, const ROOT::Math::SVector<T2, D>& v2 ) const {
        return std::equal( v1.begin(), v1.end(), v2.begin(), m_cmp );
      }
      /// comparison with another vector type
      template <class T2>
      inline bool operator()( const ROOT::Math::SVector<T2, D>& v1, const ROOT::Math::SVector<T, D>& v2 ) const {
        return std::equal( v1.begin(), v1.end(), v2.begin(), m_cmp );
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the evaluator
      Equal_To<T> m_cmp; // the evaluator
      // ======================================================================
    };
    // ========================================================================
    /// specialisation for matrices
    template <class T, unsigned int D1, unsigned int D2, class R>
    struct Equal_To<ROOT::Math::SMatrix<T, D1, D2, R>> {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_double
       */
      Equal_To( const unsigned int eps = mULPS_double ) : m_cmp( eps ) {}
      // ======================================================================
      /// comparison:
      inline bool operator()( const ROOT::Math::SMatrix<T, D1, D2, R>& v1,
                              const ROOT::Math::SMatrix<T, D1, D2, R>& v2 ) const {
        return &v1 == &v2 || std::equal( v1.begin(), v1.end(), v2.begin(), m_cmp );
      }
      // ======================================================================
      /// compare with another matrix type (e.g. symmetric...)
      template <class T2, class R2>
      inline bool operator()( const ROOT::Math::SMatrix<T, D1, D2, R>&   v1,
                              const ROOT::Math::SMatrix<T2, D1, D2, R2>& v2 ) const {
        for ( unsigned int i = 0; i < D1; ++i ) {
          for ( unsigned int j = 0; j < D2; ++j ) {
            if ( !m_cmp( v1( i, j ), v2( i, j ) ) ) { return false; }
          }
        } // RETURN
        return true;
      }
      // ======================================================================
      /// compare with another matrix type (e.g. symmetric...)
      template <class T2, class R2>
      inline bool operator()( const ROOT::Math::SMatrix<T2, D1, D2, R2>& v1,
                              const ROOT::Math::SMatrix<T, D1, D2, R>&   v2 ) const {
        for ( unsigned int i = 0; i < D1; ++i ) {
          for ( unsigned int j = 0; j < D2; ++j ) {
            if ( !m_cmp( v1( i, j ), v2( i, j ) ) ) { return false; }
          }
        } // RETURN
        return true;
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the evaluator
      Equal_To<T> m_cmp; // the evaluator
      // ======================================================================
    };
    // ========================================================================
  } // namespace Math
  // ==========================================================================
} // namespace LHCb
// ============================================================================
// The END
// ============================================================================
#endif // LHCBMATH_EQUALTO_H
