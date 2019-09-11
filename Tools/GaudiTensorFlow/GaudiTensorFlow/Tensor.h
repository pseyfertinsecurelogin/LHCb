/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef _GAUDITENSORFLOW_H_
#define _GAUDITENSORFLOW_H_

// STL
#include <cstring> //for std::memcpy
#include <sstream>

// Boost
#include "boost/assert.hpp"
#include "boost/multi_array.hpp"

// TensorFlow
#include "tensorflow/c/c_api.h"

// Local
#include "GaudiTensorFlow/SmartPointers.h"
#include "GaudiTensorFlow/TensorAbs.h"

#include <iostream>

namespace GaudiTensorFlow {
  //============================================================================
  //  TensorFlow internal type numbering
  //============================================================================
  template <typename T>
  TF_DataType getTensorFlowDataType();

  template <>
  TF_DataType getTensorFlowDataType<float>() {
    return TF_FLOAT;
  }
  template <>
  TF_DataType getTensorFlowDataType<double>() {
    return TF_DOUBLE;
  }
  template <>
  TF_DataType getTensorFlowDataType<int>() {
    return TF_INT32;
  }
  template <>
  TF_DataType getTensorFlowDataType<unsigned char>() {
    return TF_UINT8;
  }
  template <>
  TF_DataType getTensorFlowDataType<short>() {
    return TF_INT16;
  }
  template <>
  TF_DataType getTensorFlowDataType<char>() {
    return TF_INT8;
  }
  template <>
  TF_DataType getTensorFlowDataType<long>() {
    return TF_INT64;
  }
  template <>
  TF_DataType getTensorFlowDataType<bool>() {
    return TF_BOOL;
  }
  template <>
  TF_DataType getTensorFlowDataType<unsigned int>() {
    return TF_UINT32;
  }
  template <>
  TF_DataType getTensorFlowDataType<unsigned long>() {
    return TF_UINT64;
  }

  template <typename TYPE, int RANK>
  //============================================================================
  //  Tensor class
  //    templated, state-less, STL-compatible interface to boost::multi_array
  //    implementing TensorAbs abstract class.
  //============================================================================
  class Tensor : public TensorAbs, public boost::multi_array<TYPE, RANK> {
  public:
    //========================================================================
    //  Reusing multi_array interface
    //========================================================================
    using boost::multi_array<TYPE, RANK>::multi_array;
    using typename boost::multi_array<TYPE, RANK>::index;
    typedef boost::array<typename boost::multi_array<TYPE, RANK>::index, RANK> indices;

    Tensor( std::initializer_list<size_t> shape ) : Tensor( convert_to_indices( shape ) ) {}

    //========================================================================
    //  Extending interface to support STL containers
    //========================================================================
    TYPE operator()( std::initializer_list<size_t> indices ) const {
      return boost::multi_array<TYPE, RANK>::operator()( convert_to_indices( indices ) );
    }

    template <typename INDEX_CONTAINER>
    TYPE operator()( INDEX_CONTAINER indices ) const {
      return boost::multi_array<TYPE, RANK>::operator()( convert_to_indices( indices ) );
    }

    TYPE& operator()( std::initializer_list<size_t> indices ) {
      return boost::multi_array<TYPE, RANK>::operator()( convert_to_indices( indices ) );
    }

    template <typename INDEX_CONTAINER>
    TYPE& operator()( INDEX_CONTAINER indices ) {
      return boost::multi_array<TYPE, RANK>::operator()( convert_to_indices( indices ) );
    }

    //========================================================================
    //  Implementing interface defined in TensorAbs
    //========================================================================
    std::vector<size_t> getShape() const override {
      std::vector<size_t> ret( RANK );
      auto                shape = boost::multi_array<TYPE, RANK>::shape();
      std::copy( shape, shape + RANK, ret.begin() );
      return ret;
    }

    //========================================================================
    SmartTensor get_TF_Tensor() override {
      std::array<int64_t, RANK> tf_shape;
      auto                      shape = boost::multi_array<TYPE, RANK>::shape();
      std::copy( shape, shape + RANK, tf_shape.begin() );

      TYPE* data = boost::multi_array<TYPE, RANK>::data();

      SmartTensor ret( TF_AllocateTensor( getTfType(), tf_shape.data(), RANK, getAllocatedNumberOfBytes() ) );

      std::memcpy( TF_TensorData( ret.get() ), data, getAllocatedNumberOfBytes() );

      return ret;
    }

    //========================================================================
    void importData( void* input ) override {
      auto p = static_cast<TYPE*>( input );
      std::memcpy( static_cast<void*>( boost::multi_array<TYPE, RANK>::data() ), static_cast<void*>( p ),
                   getAllocatedNumberOfBytes() );
    }

    //========================================================================
    TF_DataType getTfType() const override { return getTensorFlowDataType<TYPE>(); }

    //========================================================================
    size_t getAllocatedNumberOfBytes() const override {
      return boost::multi_array<TYPE, RANK>::num_elements() * sizeof( TYPE );
    }

    //========================================================================
    //  dump  (representation of the tensor as a string)
    //========================================================================
    std::string dump( const std::string& indent_string = " ", std::vector<size_t> idx = {} ) const {
      const auto        nDim = RANK - idx.size();
      std::stringstream ret;

      if ( nDim == 0 ) {
        indices index_array;
        std::copy( idx.begin(), idx.end(), index_array.begin() );
        ret << boost::multi_array<TYPE, RANK>::operator()( index_array );
        return ret.str();
      }

      ret << "[";

      const auto len = boost::multi_array<TYPE, RANK>::shape()[idx.size()];
      idx.push_back( 0 );
      for ( size_t i = 0; i < len; ++i ) {
        idx[idx.size() - 1] = i;
        if ( i && nDim == 1 ) ret << " ";
        ret << dump( indent_string + " ", idx );
      }
      ret << "]";

      std::string retstr = ret.str();
      for ( size_t iChar = 1; iChar < retstr.size(); ++iChar )
        if ( retstr[iChar - 1] == ']' && retstr[iChar] == '[' )
          retstr.insert( iChar, std::string( "\n" ) + indent_string );

      return retstr;
    }

    //========================================================================
    //  Declaration of stream operator
    //========================================================================
    template <typename T2, int TFTYPE2>
    friend std::ostream& operator<<( std::ostream& os, GaudiTensorFlow::Tensor<T2, TFTYPE2> const& m );

  private:
    //========================================================================
    //  convert_to_indices (private)
    //========================================================================
    template <typename T>
    static indices convert_to_indices( T args ) {
      static_assert(
          std::is_same<typename T::value_type, size_t>::value || std::is_same<typename T::value_type, short>::value ||
          std::is_same<typename T::value_type, int>::value || std::is_same<typename T::value_type, long>::value ||
          std::is_same<typename T::value_type, int64_t>::value );
      BOOST_ASSERT_MSG( args.size() == RANK, "Incorrect number of indices." );
      indices ret;
      std::copy( args.begin(), args.end(), ret.begin() );
      return ret;
    }
  };

  //============================================================================
  // Service functions
  //============================================================================
  template <typename T, int RANK>
  std::ostream& operator<<( std::ostream& os, GaudiTensorFlow::Tensor<T, RANK> const& m ) {
    return os << m.dump();
  }

  //============================================================================
  template <typename TYPE>
  Tensor<TYPE, 0> make_tensor( TYPE t ) {
    auto ret    = Tensor<TYPE, 0>();
    *ret.data() = t;
    return ret;
  }

  //============================================================================
  template <typename TYPE>
  Tensor<TYPE, 1> make_tensor( std::vector<TYPE> t ) {
    auto ret = Tensor<TYPE, 1>( {t.size()} );
    ret.importData( static_cast<void*>( t.data() ) );
    return ret;
  }

  //============================================================================
  template <typename TYPE>
  Tensor<TYPE, 2> make_tensor( const std::vector<std::vector<TYPE>>& v ) {
    for ( const auto& col : v )
      if ( v[0].size() != col.size() ) throw std::range_error( "Inconsistent length of columns" );

    Tensor<TYPE, 2> ret( {v.size(), v[0].size()} );
    for ( size_t iRow = 0; iRow < v.size(); ++iRow )
      for ( size_t iCol = 0; iCol < v[0].size(); ++iCol ) ret[iRow][iCol] = v[iRow][iCol];

    return ret;
  }

  //============================================================================
  template <typename TYPE>
  Tensor<TYPE, 2> zip_tensor( const std::vector<std::vector<TYPE>>& v ) {
    for ( const auto& col : v )
      if ( v[0].size() != col.size() ) throw std::range_error( "Inconsistent length of columns" );

    Tensor<TYPE, 2> ret( {v[0].size(), v.size()} );

    for ( size_t iRow = 0; iRow < v[0].size(); ++iRow )
      for ( size_t iCol = 0; iCol < v.size(); ++iCol ) ret[iRow][iCol] = v[iCol][iRow];

    return ret;
  }

} // namespace GaudiTensorFlow

#endif // _GAUDITENSORFLOW_TENSORMULTIARRAY_H_
