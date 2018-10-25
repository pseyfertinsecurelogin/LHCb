/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include <array>
#include "VectorConfiguration.h"
#include "Math/SVector.h"

#include "LHCbMath/MatrixManip.h"
#include "Event/StateVector.h"

namespace LHCb {

namespace Vector {

namespace Mem {

namespace View {

/**
 * @brief      A generic Matrix SOA view.
 *             It requires a valid basePointer to function properly.
 *
 * @tparam     PRECISION  Precision of the matrix.
 * @tparam     N          Number of elements of the matrix.
 */
template<class PRECISION, std::size_t N>
struct Matrix {
  constexpr static unsigned size () { return N; }
  
  PRECISION* m_basePointer = nullptr;
  Matrix () = default;
  Matrix (PRECISION* basePointer) : m_basePointer(basePointer) {}
  void setBasePointer (const Matrix<PRECISION, N>& v) { m_basePointer = v.m_basePointer; }
  void setBasePointer (PRECISION* basePointer) { m_basePointer = basePointer; }
  PRECISION& operator[] (const unsigned i) { return m_basePointer[i * VectorConfiguration::width<PRECISION>()]; }
  PRECISION operator[] (const unsigned i) const { return m_basePointer[i * VectorConfiguration::width<PRECISION>()]; }

  /**
   * @brief      Copies v into its state
   *             Assumes m_basePointer is well defined
   */
  void copy (const Matrix<PRECISION, N>& m) {
    for (unsigned i=0; i<N; ++i) {
      (*this)[i] = m[i];
    }
  }

  void copy (const std::array<PRECISION, N>& v) {
    for (unsigned i=0; i<N; ++i) {
      (*this)[i] = v[i];
    }
  }

  template<class __GAUDITYPE>
  void copy (const __GAUDITYPE& gm) {
    for (unsigned i=0; i<N; ++i) {
      (*this)[i] = gm.Array()[i];
    }
  }

  void operator+= (const Matrix<PRECISION, N>& m) {
    for (unsigned i=0; i<N; ++i) {
      (*this)[i] += m[i];
    }
  }

  template<class __GAUDITYPE>
  void operator+= (const __GAUDITYPE& gm) {
    for (unsigned i=0; i<N; ++i) {
      (*this)[i] += gm.Array()[i];
    }
  }
};

template<class PRECISION>
struct TrackVector : public Matrix<PRECISION, 5> {
  TrackVector () = default;
  TrackVector (PRECISION* m_basePointer) : Matrix<PRECISION, 5>(m_basePointer) {}

  void copy (const TrackVector& v) { Matrix<PRECISION, 5>::copy(static_cast<Matrix<PRECISION, 5>>(v)); }
  void operator+= (const TrackVector& v) { Matrix<PRECISION, 5>::operator+=(static_cast<Matrix<PRECISION, 5>>(v)); }

  void copy (const Gaudi::TrackVector& v) { Matrix<PRECISION, 5>::template copy<Gaudi::TrackVector>(v); }
  void copy (const Gaudi::TrackProjectionMatrix& v) { Matrix<PRECISION, 5>::template copy<Gaudi::TrackProjectionMatrix>(v); }
  void operator+= (const Gaudi::TrackVector& v) { Matrix<PRECISION, 5>::template operator+=<Gaudi::TrackVector>(v); }
  
  operator Gaudi::TrackVector () const {
    return Gaudi::TrackVector{(*this)[0], (*this)[1], (*this)[2], (*this)[3], (*this)[4]};
  }

  operator Gaudi::TrackProjectionMatrix () const {
    Gaudi::TrackProjectionMatrix pm;
    for (unsigned i=0; i<5; ++i) {
      pm.Array()[i] = (*this)[i];
    }
    return pm;
  }
};

template<class PRECISION>
struct TrackSymMatrix : public Matrix<PRECISION, 15> {
  TrackSymMatrix () = default;
  TrackSymMatrix (PRECISION* m_basePointer) : Matrix<PRECISION, 15>(m_basePointer) {}

  void copy (const std::array<PRECISION, 15>& v) { Matrix<PRECISION, 15>::copy(v); }
  void copy (const TrackSymMatrix& v) { Matrix<PRECISION, 15>::copy(static_cast<Matrix<PRECISION, 15>>(v)); }
  void operator+= (const TrackSymMatrix& v) { Matrix<PRECISION, 15>::operator+=(static_cast<Matrix<PRECISION, 15>>(v)); }
  
  void copy (const Gaudi::TrackSymMatrix& v) { Matrix<PRECISION, 15>::template copy<Gaudi::TrackSymMatrix>(v); }
  void operator+= (const Gaudi::TrackSymMatrix& v) { Matrix<PRECISION, 15>::template operator+=<Gaudi::TrackSymMatrix>(v); }
  
  PRECISION& operator() (const unsigned row, const unsigned col) {
    return row>col ?
      this->m_basePointer[(row*(row+1)/2 + col) * VectorConfiguration::width<PRECISION>()] :
      this->m_basePointer[(col*(col+1)/2 + row) * VectorConfiguration::width<PRECISION>()];
  }

  PRECISION operator() (const unsigned row, const unsigned col) const {
    return row>col ?
      this->m_basePointer[(row*(row+1)/2 + col) * VectorConfiguration::width<PRECISION>()] :
      this->m_basePointer[(col*(col+1)/2 + row) * VectorConfiguration::width<PRECISION>()];
  }
  
  operator Gaudi::TrackSymMatrix () const {
    Gaudi::TrackSymMatrix t;
    for (unsigned i=0; i<15; ++i) {
      t.Array()[i] = (*this)[i];
    }
    return t;

    // TODO - Find a suitable constructor in place
    // return Gaudi::TrackSymMatrix(
    //   ROOT::Math::SVector<PRECISION, 15>{
    //     this->operator[](0), this->operator[](1), this->operator[](2), this->operator[](3), this->operator[](4),
    //     this->operator[](5), this->operator[](6), this->operator[](7), this->operator[](8), this->operator[](9),
    //     this->operator[](10), this->operator[](11), this->operator[](12), this->operator[](13), this->operator[](14)
    //   }
    // );
  }
};

// Some operators
template<class PRECISION>
Gaudi::TrackVector operator- (const Gaudi::TrackVector& mA, const TrackVector<PRECISION>& mB) {
  const auto* A = mA.Array();
  return Gaudi::TrackVector {
    A[0]-mB[0],
    A[1]-mB[1],
    A[2]-mB[2],
    A[3]-mB[3],
    A[4]-mB[4]
  };
}

template<class PRECISION, std::size_t N>
std::ostream& operator<< (std::ostream& s, const Matrix<PRECISION, N>& v) {
  for (unsigned i=0; i<N; ++i) {
    s << v[i];
    if (i != N-1) s << " ";
  }
  return s;
}

template<class PRECISION>
std::ostream& operator<< (std::ostream& s, const TrackVector<PRECISION>& v) {
  if (v.m_basePointer != nullptr) {
    for (unsigned i=0; i<5; ++i) {
      s << v[i];
      if (i!=4) s << " ";
    }
  } else {
    s << "uninitialized";
  }
  return s;
}

template<class PRECISION>
std::ostream& operator<< (std::ostream& s, const TrackSymMatrix<PRECISION>& v) {
  if (v.m_basePointer != nullptr) {
    for (unsigned i=0; i<15; ++i) {
      s << v[i];
      if (i!=14) s << " ";
    }
  } else {
    s << "uninitialized";
  }
  return s;
}

}

}

}

}
