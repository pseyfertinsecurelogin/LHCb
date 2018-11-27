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
#ifndef EVENT_CALODATAFUNCTOR_H
#define EVENT_CALODATAFUNCTOR_H 1
// ============================================================================
// STD and STL
// ============================================================================
#include <functional>
#include <cmath>
#include <type_traits>
#include <iterator>
// ============================================================================
// MathCore
// ============================================================================
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/Point3DTypes.h"
// ============================================================================
// Event
// ============================================================================
#include "Kernel/CaloCellID.h"
#include "Event/CaloClusterEntry.h"
#include "Event/CaloDigit.h"
#include "Event/CaloDigitStatus.h"
#include "Event/CaloCluster.h"
#include "Event/CaloHypo.h"

template <class TYPE>
class SmartRef  ;     // GaudiKernel

/** @namespace CaloDataFunctor CaloDataFunctor.h Event/CaloDataFunctor.h
 *
 *  collection of some  useful functors, which could be used for
 *  manipulation with CaloDigit, MCCaloDigit, MCCaloHit,
 *  MCCaloSensPlaneHit, CaloCluster and CaloParticle objects
 *
 *  @author  Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date    26/11/1999
 */
namespace LHCb
{
  namespace CaloDataFunctor
  {
    // ==========================================================================
    /** @class CellID
     *
     *  The simple structure that "extracts" CaloCellID code from the class
     *  The 'generic implemntation relies on existence of TYPE::cellID() method
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2004-10-22
     */
    // ==========================================================================
    struct CellID_t
    {
      template<class TYPE, typename = typename std::enable_if<!std::is_pointer<TYPE>::value>::type>
      LHCb::CaloCellID operator() ( const TYPE& obj ) const
      { return obj.cellID() ; }

      template<class TYPE>
      LHCb::CaloCellID operator() ( const TYPE* obj ) const
      { return obj ? (*this)(*obj) : LHCb::CaloCellID() ; }

      template<class TYPE>
      LHCb::CaloCellID operator() ( const SmartRef<TYPE> & ref ) const
      { return (*this)(ref.target()); }

      LHCb::CaloCellID operator() ( const LHCb::CaloCellID& id ) const
      { return id ; } ;

      LHCb::CaloCellID operator() ( const LHCb::CaloCluster& cluster ) const
      { return cluster.seed() ; }

      LHCb::CaloCellID operator() ( const LHCb::CaloHypo& hypo ) const
      { return !hypo.clusters().empty() ? (*this)( hypo.clusters().front() )
                                        : LHCb::CaloCellID() ; }

    };
    inline constexpr CellID_t CellID{};
    // ==========================================================================

    // ==========================================================================
    /** @class Over_E_Threshold CaloDataFunctor.h Event/CaloDataFuctor.h
     *
     *  Comparison of the energy of the object with given threshold
     *
     *  Example:
     *  select all digits with are over the threshold:
     *  @code
     *  CaloDigits* digits = ... ;
     *  Over_E_Threshold cmp( 1 * GeV );
     *  CaloDigits::const_iterator it =
     *  std::stable_partition( digits->begin() ,
     *                         digits->end()   ,
     *                         cmp             );
     *  @endcode
     *  Here for all digits with exceed the threshold are placed
     *  before @p it
     *
     *  @see MCCaloHit
     *  @see MCCaloDigit
     *  @see CaloDigit
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    class Over_E_Threshold
    {
    public:
      /** constructor (explicit)
       *  @param threshold   threshold on energy of the object
       */
      explicit Over_E_Threshold( double threshold = 0.0 )
        : m_threshold( threshold ) {};
      /** compare the energy of the object with threshold value
       *  @param  obj  object
       *  @return result of comparison with threshold
       */
      template <class TYPE>
      inline bool operator() ( const TYPE& obj  ) const
      { return obj && obj->e() > m_threshold ; }

      inline bool operator() ( const LHCb::CaloClusterEntry& obj  ) const
      {
        return obj.digit() && ( obj.digit()->e() * obj.fraction() > m_threshold );
      };
    private:
      double m_threshold{0}; ///< the actual threshold value for the energy
    };
    // ==========================================================================


    // ==========================================================================
    /** @class EnergyTransverse CaloDataFunctor.h Event/CaloDataFucntor.h
     *
     *  Calculate the transverse energy for the object
     *
     *  "TYPE" is required to have valid comparison with 0 and
     *  implemented "->e()" methods and valid CellID, e.g. pointer
     *  or smart reference to CaloDigit, MCCaloDigit, CaloCluster
     *  objects.
     *
     *  "DETECTOR"  is required to have implemented methods,
     *  e.g. pointer or smart pointer to DeCalorimeter object:
     *
     *   -  double cellCenter( const CellID& ) const;
     *
     *   since the correct definition of "z" is a quite delicate task,
     *   functor allows to modify the z-value from Detector
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template<class DETECTOR >
    class EnergyTransverse
    {
    public:
      /** constructor
       *  @param detector  "DETECTOR" object
       *  @param deltaZ    z-correction to position of shower maximum
       */
      EnergyTransverse
      ( DETECTOR detector     ,
        double   deltaZ   = 0 )
        : m_det ( detector )
        , m_dz  ( deltaZ   ) {}

      /** calculate the transverse energy of the object
       *  @param  obj   object
       *  @return the transverse energy of the object
       */
      template <typename TYPE>
      inline double operator () ( const TYPE& obj ) const
      {
        return ( obj ?
                 obj->e() * std::sin( m_det->cellCenter(CellID(obj)).Theta() ) :
                 0.0 );
      }
    private:
      mutable DETECTOR m_det  ;  ///< detector element
      double  m_dz{0}         ;  ///< dz correction
    };
    // ==========================================================================

    // ==========================================================================
    /** @class Over_Et_Threshold CaloDataFunctor.h Event/CaloDataFucntor.h
     *
     *  Comparison of the transverse energy of the object with
     *  given threshold value
     *
     *  "DETECTOR"  is required to have implemented methods,
     *    e.g. pointer or smart pointer to DeCalorimeter object:
     *
     *   - double cellCenter( const CellID& ) const;
     *
     *  since the correct definition of "z" is a quite delicate task,
     *  functor allows to modify the z-value from Detector
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template < class DETECTOR >
    class Over_Et_Threshold
    {
      /// typedef for Et estimator
      typedef EnergyTransverse<DETECTOR>  ET;
    public:
      /** constructor
       *  @param Detector         "DETECTOR" object
       *  @param Threshold        transverse energy threshold
       *  @param DeltaZ           z-correction to position of shower maximum
       */
      Over_Et_Threshold ( DETECTOR Detector,
                          double   Threshold = 0 ,
                          double   DeltaZ = 0    )
        : m_et       ( Detector  , DeltaZ )
        , m_threshold( Threshold          ) {}
      /** compare the threshold energy of the object with threshold value
       *  @param  obj  object
       *  @return result of comparison with threshold
       */
      template <typename TYPE>
      inline bool operator() ( const TYPE& obj  ) const
      { return obj && ( m_et( obj ) > m_threshold ) ; }
    private:
      ET        m_et            ;      ///< e_t estimator
      double    m_threshold{0}  ;      ///< threshold value for energy
    };
    // ==========================================================================

    // ==========================================================================
    /** @class Less_by_Energy CaloDataFunctor.h Event/CaloDataFunctor.h
     *
     *  Comparison of the energy of one object with the energy of
     *  another object. Types of objects could be different!
     *
     *  "TYPE1","TYPE2" are required to have valid comparison with 0,
     *  and implemented "->e()" method, e.g. pointer or smart reference
     *  to CaloDigit, MCCaloDigit, or CaloCluster object.
     *
     *  Example:
     *  sort container of MCCaloDigits in ascending order
     *  @code
     *  MCCaloDigits* digits = ... ;
     *  std::stable_sort( digits->begin() ,
     *                    digits->end()   ,
     *                    Less_by_Energy             );
     *  @endcode
     *
     *  For "inversion" of comparison criteria use Inverse
     *  @see Inverse
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    struct Less_by_Energy_t
    {
      /** compare the energy of one object with the energy of
       *  another object
       *  @param obj1   first  object
       *  @param obj2   second object
       *  @return  result of energy comparison
       */
      template <class TYPE1,class TYPE2 = TYPE1 >
      inline bool operator() ( const TYPE1& obj1 , const TYPE2& obj2 ) const
      {
        return !obj1 || ( obj2 && obj1->e() < obj2->e() ) ;
      }
      inline bool operator()
        ( const LHCb::CaloClusterEntry& obj1 ,
          const LHCb::CaloClusterEntry& obj2 ) const
      {
        return
          ( !obj1.digit() ) ? true  :
          ( !obj2.digit() ) ? false :
          ( obj1.digit()->e() * obj1.fraction() ) <
          ( obj2.digit()->e() * obj2.fraction() ) ;
      }
      ///
    };
    inline constexpr Less_by_Energy_t Less_by_Energy {};
    // ==========================================================================

    // ==========================================================================

    // ==========================================================================
    /** @class Less_by_TransverseEnergy CaloDataFunctor.h Event/CaloDataFunctor.h
     *
     *  Comparison of the transverse energy of one object with
     *  the transverse energy of another object
     *
     *  "DETECTOR"  is required to have implemented methods,
     *   e.g. pointer or smart pointer to DeCalorimeter object:
     *
     *   - double cellCenter( const CellID& ) const;
     *
     *  since the correct definition of "z" is a quite delicate task,
     *  functor allows to modify the z-value from Detector
     *
     *  Example:
     *  sort container of CaloDigits in ascending order of
     *  transverse energy
     *  @code
     *  CaloDigits* digits = ... ;
     *  Less_by_TrnasverseEnergy<Detector*> cmp( detector );
     *  std::stable_sort( digits->begin() ,
     *                    digits->end()   ,
     *                    cmp             );
     *  @endcode
     *
     *  For "inversion" of comparison criteria use Inverse
     *  @see Inverse
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template < class DETECTOR >
    class Less_by_TransverseEnergy
    {
      /// useful typedef for Et estimator
      typedef   EnergyTransverse< DETECTOR >       ET  ;
    public:
      /** constructor
       *  @param Detector "DETECTOR" object
       *  @param DeltaZ   z-correction to position of shower maximum
       */
      Less_by_TransverseEnergy
      ( DETECTOR Detector     ,
        double   DeltaZ   = 0 )
        : m_et ( Detector , DeltaZ ) {}
      /** compare the transverse energy of one object with the
       *  transverse energy of  another object
       *  @param obj1   first  object
       *  @param obj2   second object
       *  @return  result of energy comparison
       */
      template <typename TYPE, typename TYPE2 >
      inline bool operator() ( const TYPE&  obj1 ,
                               const TYPE2& obj2 ) const
      {
        return !obj1 || ( obj2 && ( m_et( obj1 ) < m_et( obj2 ) ) );
      }
    private:

      ET  m_et  ;      ///< e_t estimator
    };
    // ==========================================================================

    // ==========================================================================
    /** @class Accumulate_Energy CaloDataFunctor.h Event/CaloDataFunctor.h
     *
     *  Accumulate the energy of the objects
     *
     *  "TYPE" is required to have valid comparison with 0, and
     *  implemented "->e()" method, e.g. pointer or smart reference to
     *  CaloDigit,MCCaloDigit or CaloCluster objects
     *
     *  Example:
     *  accumulate the total energy of MCCaloHits
     *  @code
     *  MCCaloHits* hits = ... ;
     *  Accumulate_Energy<const MCCaloHit*> sum;
     *  const double totalEnergy =
     *   std::accumulate( hits->begin() , hits->end() , 0.0 , sum );
     *  @endcode
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    struct Accumulate_Energy_t
    {
    public:
      /** accumulate the energy of the objkect
       *  @param   Energy  accumulated energy
       *  @param   object  object
       *  @return  accumulated energy
       */
      template <class TYPE>
      inline double  operator() ( double&     Energy  ,
                                  const TYPE& object     ) const
      { return ( !object ) ? Energy : Energy += object->e() ; }
    };
    inline constexpr Accumulate_Energy_t Accumulate_Energy {};
    // ==========================================================================

    // ==========================================================================
    /** @class Accumulate_TransverseEnergy CaloDataFunctor.h
     *
     *  Accumulate the transverse energy of the object
     *
     *  "TYPE" is required to have valid comparison with 0, and
     *  implemented "->e()" method and valid
     *  EnergyTtransverse<TYPE,DETECTOR> structure, e.g.
     *  pointer or smart reference to CaloDigit or MCCaloDigit objects.
     *
     *  "DETECTOR" is required to have an implemented methods
     *  e.g. pointer or smart pointer to DeCalorimeter object:
     *
     *   - double cellCenter( const CellID& ) const;
     *
     *  Since the correct definition of "z" is a quite delicate task,
     *  implemented functor allows to apply correction to the the z-value
     *
     *  Example:
     *  accumulate the total transverse energy of CaloDigits
     *  @code
     *  CaloDigits* digits = ... ;
     *  Accumulate_TransverseEnergy<const CaloDigit*,Detector*> sum( detector );
     *  const double totalEnergy =
     *   std::accumulate( digits->begin() , digits->end() , 0.0 , sum );
     *  @endcode
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template <class DETECTOR>
    class Accumulate_TransverseEnergy
    {
      /// useful typedef for Et estimator
      typedef   EnergyTransverse<DETECTOR>        ET;
    public:
      /** constructor
       *  @param  Detector  "DETECTOR" object
       *  @param  DeltaZ     additional Z-correction to be applied
       */
      Accumulate_TransverseEnergy ( DETECTOR Detector     ,
                                    double   DeltaZ   = 0 )
        : m_et ( Detector , DeltaZ  ) { }
      /** accumulate the transverse energy of the objects
       *  @param   Energy  accumulated transverce energy
       *  @param   obj     object
       *  @return  accumulated transverse energy
       */
      template <typename TYPE>
      inline double  operator() ( double& Energy  , const TYPE& obj ) const
      { return ( !obj ) ? Energy : Energy += m_et( obj )  ; }
    private:
      ET        m_et ;    ///< e_t estimator
    };
    // ==========================================================================

    // ==========================================================================
    /** @class IsCaloCellID CaloDataFunctor.h Event/CaloDataFunctor.h
     *
     *  compare calorimtery cell identifier of the object with
     *  given value (equality test). Coudl be used for location
     *  of objects with given cellID  within the containers.
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    class IsCaloCellID
    {
    public:
      /** constructor
       *  @param CellID  cell identifier to be compared with
       */
      explicit IsCaloCellID ( const LHCb::CaloCellID& CellID )
        : m_cellID ( CellID ) { }
      /** compare cell identifier of the object with given value
       *  @param obj  object
       *  @return  result of comparison (equality test)
       */
      template <class TYPE>
      inline bool operator() ( const TYPE& obj ) const
      { return obj &&  CellID( obj )  == m_cellID ; }
    private:
      LHCb::CaloCellID  m_cellID ; ///< reference CaloCellID
    };
    // ==========================================================================

    // ==========================================================================
    /** The special functor for CaloCluster class
     *  Calculate the "energy" of the cluster as a sum of
     *  energies of its digits, weighted with energy fractions
     *  "IT" could be either iterator or const_iterator
     *
     *  @param    begin iterator to first element of sequence
     *  @param    end   iterator to last+1 element of sequance
     *  @return      "energy" of sequence
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template <class IT>
    inline  double clusterEnergy( IT begin , IT end )
    {
      double energy = 0 ;
      for ( ; begin != end ; ++begin )
      {
        // get the digit
        const LHCb::CaloDigit * digit = begin->digit()  ;
        /// skip nulls
        if( ! digit ) { continue ; }
        /// check the status and skip useless digits
        if( !( begin->status() & LHCb::CaloDigitStatus::UseForEnergy ) )
        { continue ; }
        // accumulate the energy
        energy += digit->e() * begin->fraction() ;
      }
      return energy ;
    }
    // ==========================================================================

    // ==========================================================================
    /** The special functor for CaloCluster class.
     *  Calculate the "energy", X and Y position
     *  of the cluster as a sum of
     *  energies/x/y of its digits,
     *  weighted with energy fractions
     *  "IT" could be either iterator or const_iterator
     *
     *  Error Codes:
     *
     *   - 200   invalid pointer to detector element
     *   - 201   empty input sequence
     *   - 201   no selected digits
     *   - 202   accumulated energy = 0
     *
     *  @param   begin iterator to first element of sequence
     *  @param   end   iterator to last+1 element of sequance
     *  @param   de  pointer to DeCalorimeter object
     *  @param   e   energy
     *  @param   x   x-position
     *  @param   y   y-position
     *  @return    status code
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template <class IT, class DE>
    inline StatusCode
    calculateClusterEXY( IT begin  ,
                         IT end    ,
                         DE de     ,
                         double& e ,
                         double& x ,
                         double& y  ){
      // reset initial parameters
      e = 0 ;  x = 0 ; y = 0 ;
      // use counter
      unsigned num  = 0 ;
      // energy for position
      double   epos = 0 ;
      // no detector
      if( 0 == de                 ) { return StatusCode( 200 )   ; }
      // empty sequence
      if( 0 == de || begin == end ) { return StatusCode( 201 )   ; }
      // explicit loop over all entries
      for( ; begin != end ; ++begin ){
        // extract the digit
        const LHCb::CaloDigit*       digit  = begin->digit() ;
        // skip nulls
        if( 0 == digit                           ) { continue ; }
        double eDigit = digit->e() * begin->fraction() ;
        if( ( begin->status() & LHCb::CaloDigitStatus::UseForEnergy   ) !=0 ){
          e += eDigit    ;        // accumulate digit energy
        }
        if( ( begin->status() & LHCb::CaloDigitStatus::UseForPosition ) != 0 ) {
          epos += eDigit ;        // accumulate digit energy for position
          ++num ;
          const Gaudi::XYZPoint pos = de->cellCenter( digit->cellID() );
          x += eDigit * pos.x() ;
          y += eDigit * pos.y() ;
        }
      }
      // at least one useful digit ?
      if( 0 == num               ) { return StatusCode( 202 ) ; }
      // accumulated energy is NULL!
      if( 0 == epos              ) { return StatusCode( 203 ) ; }
      // rescale x and y
      if( 0 != epos ) { x /= epos ; }
      else            { x  = 0    ; }
      if( 0 != epos ) { y /= epos ; }
      else            { y  = 0    ; }
      //
      return StatusCode::SUCCESS ;
    }
    // ==========================================================================


    // ==========================================================================
    /** The special fucntor to CaloCluster class.
     *  useful function to find first common digit
     *  from two sequences. It returns the pair of
     *  iterators (first one for first sequence and
     *  the secons one for second sequences).
     *  "IT" could be either iterator or const_iterator
     *
     *  @param  begin1 iterator pointing to 1st    element of 1st sequence
     *  @param  end1   iterator pointing to last+1 element of 1st sequence
     *  @param  begin2 iterator pointing to 1st    element of 2nd sequence
     *  @param  end2   iterator pointing to last+1 element of 2nd sequence
     *  @return pair of iterators
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template<class IT>
    inline std::pair<IT,IT>
    clusterCommonDigit( IT begin1 ,
                        IT end1   ,
                        IT begin2 ,
                        IT end2   )
    {
      // check arguments
      if( begin2 == end2 ||  begin1 == end1 )
      { return std::pair<IT,IT>( end1 , end2 ); }
      // loop over the sequences
      for( IT it1 = begin1 ; end1 != it1 ; ++it1 )
      {
        if( !(it1->digit() ) ) { continue ;  }  // skip NULLS !
        for( IT it2 = begin2 ; end2 != it2 ; ++it2 )
        {
          if( it1->digit() == it2->digit() )  // the same digit!
          { return { it1 , it2 }; }
        }
      }
      return { end1 , end2 };
    }
    // ==========================================================================

    // ==========================================================================
    /** The special function for CaloCluster class
     *  The useful function to locate the digit within the sequence
     *
     *  "IT" could be either iterator or const_iterator
     *
     *  @param begin iterator pointing to the 1st    element of sequence
     *  @param end   iterator pointing to teh last+1 element of sequence
     *  @param digit pointer to CaloDigit
     *  @return location of digit within the sequence
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template<class IT>
    inline IT
    clusterLocateDigit( IT begin               ,
                        IT end                 ,
                        const LHCb::CaloDigit* digit )
    {
      return std::find_if( begin, end,
                           [&](const auto& arg) { return arg->digit() == digit; } );
    }
    // ==========================================================================

    // ==========================================================================
    /** The special function for CaloCluster class
     *  The useful function to locate the digit with given status
     *  from sequence of digits
     *
     *  "IT" could be either iterator or const_iterator
     *
     *  @param begin  iterator pointing to the 1st    element of sequence
     *  @param end    iterator pointing to the last+1 element of sequence
     *  @param st     status
     *  @return location of digit within the sequence
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 26/11/1999
     */
    template <class IT>
    inline IT
    clusterLocateDigit ( IT begin                          ,
                         IT end                            ,
                         const LHCb::CaloDigitStatus::Status& st )
    {
      return std::find_if( begin, end, [&](const auto& arg) { return  st & arg.status(); } );
    }
    // ==========================================================================


    // ==========================================================================
    /** The special functions to create "inverse" comparison criteria
     *
     *  @see inverse
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 12 May 2002
     */
    template <class Compare>
    struct Inverse
    {
      /// constructor
      Inverse( const Compare& cmp ) : m_cmp ( cmp ) {};
      /** the only one essential method
       *  @param arg1 first  argument ("second" for underlying comparison)
       *  @param arg2 second argument ("first"  for underlying comparison)
       *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
       *  @date 12 May 2002
       */
      template <typename Arg1, typename Arg2>
      auto operator() ( Arg1&& arg1 , Arg2&& arg2 ) const
      { return m_cmp( std::forward<Arg2>(arg2) , std::forward<Arg1>(arg1) ) ;}
    private:
      // underlying comparison criteria
      Compare  m_cmp;
    };
    // ==========================================================================

    // ==========================================================================
    /** helpful function (in STL spirit) for creation of "inverse"
     *  comparison criteria:
     *
     *  @see Inverse
     *  Example:
     *  @code
     *  CaloClusters* clusters = ... ;
     *  Less_by_Energy cmp;
     *  // sort clusters in ascending order:
     *  std::stable_sort( clusters->begin() ,
     *                    clusters->end()   ,          cmp   ) ;
     *  // sort clusters in descensing order
     *  std::stable_sort( clusters->begin() ,
     *                    clusters->end()   , inverse( cmp ) ) ;
     *  @endcode
     */
    template <class Compare>
    inline Inverse<Compare> inverse( const Compare& cmp )
    { return Inverse<Compare>( cmp ); }
    // ==========================================================================

    // ==========================================================================
    /** @class DigitFromCalo
     *  simple utility to count digits from certain calorimeter
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date 31/03/2002
     */
    class DigitFromCalo
    {
    public:
      DigitFromCalo() = delete;
      /** constructor
       *  @param calo  calorimeter name
       */
      explicit DigitFromCalo( const std::string& calo )
        : m_calo( CaloCellCode::CaloNumFromName( calo ) ) {}
      /** constructor
       *  @param calo  calorimeter index
       */
      explicit DigitFromCalo( const int  calo )
        : m_calo(                                calo   ) {}
      /** the only essential method
       *  @param digit pointer to CaloDigit object
       *  @return true if digit belongs to the predefined calorimeter
       */
      inline bool operator() ( const LHCb::CaloDigit* digit ) const
      {
        return digit && static_cast<int>(digit->cellID().calo()) == m_calo ;
      };
      /** change the currect calorimeter
       *  @param calo new calorimeter name
       */
      inline void setCalo( const std::string& calo )
      { setCalo( CaloCellCode::CaloNumFromName( calo ) ); };
      /** change the currect calorimeter
       *  @param calo new calorimeter index
       */
      inline void setCalo( const int  calo ) { m_calo = calo ; };
    private:
      int m_calo{0} ;

    };
    // ==========================================================================

    // ==========================================================================
    /** @class Calo
     *  simple class to extract the Calorimeter index (SPD/PRS/ECAL/HCAL)
     *  from the objects
     *  The TYPE is reuired to have the valid CellID<TYPE> structure
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-10-22
     */
    struct Calo_t
    {
      template <class TYPE>
      int operator() ( const TYPE& obj ) const
      { return CellID( obj ).calo() ; }
    };
    inline constexpr Calo_t Calo{};
    // ==========================================================================

    // ==========================================================================
    /** @class IsFromCalo
     *  simple class (predicate) to determine if the object
     *  'is from the given calrimeter'
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2004-10-22
     */
    class IsFromCalo
    {
    public:
      IsFromCalo() = delete;
      /// constructor from calorimeter index
      IsFromCalo  ( int index ) : m_index ( index ) {}
      /// constructor from calorimeter name
      IsFromCalo  ( const std::string& name ) : IsFromCalo( CaloCellCode::CaloNumFromName ( name ) ) {}
      template <class TYPE>
      bool operator() ( const TYPE& obj ) const
      { return Calo( obj ) == m_index ; }
    private:
      int        m_index{0} ;
    };
    // ==========================================================================

    // ==========================================================================
    /** @class CaloArea
     *  simple class to extract the Calorimeter area
     *  from the objects
     *  The TYPE is reuired to have the valid CellID<TYPE> structure
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-10-22
     */
    struct CaloArea_t
    {
      template <class TYPE>
      int operator() ( const TYPE& obj ) const { return CellID( obj ).area() ; }
    };
    inline constexpr CaloArea_t CaloArea{};
    // ==========================================================================

    // ==========================================================================
    /** @class IsFromArea
     *  simple class (predicate) to determine if the object
     *  'is from the given area'
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2004-10-22
     */
    class IsFromArea
    {
    public:
      IsFromArea() = delete;
      explicit IsFromArea  ( int index ) : m_index ( index ) {}
      template <class TYPE>
      bool operator() ( const TYPE& obj ) const
      { return CaloArea( obj ) == m_index ; }
    private:
      int            m_index{0} ;
    };
    // ==========================================================================


    // ==========================================================================
    /** @class CaloRow
     *  simple class to extract the Calorimeter Row
     *  from the objects
     *  The TYPE is reuired to have the valid CellID<TYPE> structure
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-10-22
     */
    struct CaloRow_t
    {
      template <class TYPE>
      int operator() ( const TYPE& obj ) const
      { return CellID( obj ).row() ; }

    };
    inline constexpr CaloRow_t CaloRow{};
    // ==========================================================================

    // ==========================================================================
    /** @class IsFromRow
     *  simple class to determine if the object 'is from the given row'
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2004-10-22
     */
    class IsFromRow
    {
    public:
      IsFromRow() = delete;
      explicit IsFromRow   ( int index ) : m_index ( index ) {}
      template <class TYPE> bool operator() ( const TYPE& obj ) const
      { return CaloRow( obj ) == m_index ; }
    private:
      int            m_index{0} ;
    };
    // ==========================================================================

    // ==========================================================================
    /** @class CaloColumn
     *  simple class to extract the Calorimeter column
     *  from the objects
     *  The TYPE is reuired to have the valid CellID<TYPE> structure
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-10-22
     */
    struct CaloColumn_t
    {
      template <class TYPE>
      int operator() ( const TYPE& obj ) const
      { return CellID( obj ).col() ; }
    };
    inline constexpr CaloColumn_t CaloColumn{};
    // ==========================================================================

    // ==========================================================================
    /** @class IsFromColumn
     *  simple class to determine if the object 'is from the given column'
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2004-10-22
     */
    class IsFromColumn
    {
    public:
      IsFromColumn() = delete;
      IsFromColumn  ( int index ) : m_index   ( index ) {}
      template <class TYPE> bool operator() ( const TYPE& obj ) const
      { return CaloClumn( obj ) == m_index ; }
    private:
      int              m_index{0} ;
    };
    // ==========================================================================

  } // end of namespace CaloDataFunctor
} // end of namespace LHCb
// ============================================================================
// The End
// ============================================================================
#endif // EVENT_CALODATAFUNCTOR_H
// ============================================================================
