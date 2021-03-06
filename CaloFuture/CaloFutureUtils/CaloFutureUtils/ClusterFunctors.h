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
#ifndef CALOFUTUREUTILS_CLUSTERFUNCTORS_H
#define CALOFUTUREUTILS_CLUSTERFUNCTORS_H 1
// ============================================================================
// Include files
// CaloDet
#include "CaloDet/DeCalorimeter.h"
#include "CaloFutureUtils/CaloFutureDataFunctor.h"
#include "Event/CaloCluster.h"

class DeCalorimeter;

/** @namespace ClusterFunctors ClusterFunctors.h CaloFutureUtils/ClusterFunctors.h
 *
 *  collection of useful functors for dealing with
 *  CaloCluster objects
 *
 *  @author Ivan Belyaev
 *  @date   04/07/2001
 */
namespace LHCb {
  namespace ClusterFunctors {

    using iterator       = LHCb::CaloCluster::Entries::iterator;
    using const_iterator = LHCb::CaloCluster::Entries::const_iterator;
    typedef std::pair<iterator, iterator>             iterator_pair;
    typedef std::pair<const_iterator, const_iterator> const_iterator_pair;

    /** @fn void throwException( const std::string& message )
     *  throw the exception
     *  @exception CaloException
     *  @param message exception message
     *  @return status code (fictive)
     */
    [[noreturn]] void throwException( const std::string& message );

    /** Calculate the "energy" of the cluster
     *  as a sum of energies of its digits, weighted with energy fractions
     *  @param   cl  pointer to cluster
     *  @return      "energy" of cluster
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   xx/xx/xxxx
     */
    double energy( const LHCb::CaloCluster* cl );

    /** Calculate the "energy" of the cluster
     *  as a sum of energies of its digits, weighted with energy fractions
     *
     *  "IT" could be either iterator or const_iterator
     *
     *  @param    begin iterator to first element of sequence
     *  @param    end   iterator to last+1 element of sequance
     *  @return      "energy" of sequence
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     */
    template <class IT>
    double energy( IT begin, IT end ) {
      return CaloDataFunctor::clusterEnergy( begin, end );
    }

    /** Calculate the "energy", X and Y position of the cluster
     *  as a sum of energies/x/y of its digits, weighted with energy fractions
     *  @param   cl  pointer to cluster object
     *  @param   de  pointer to DeCalorimeter object
     *  @param   e   energy
     *  @param   x   x-position
     *  @param   y   y-position
     *  @return    status code
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     */
    StatusCode calculateEXY( const LHCb::CaloCluster* cl, const DeCalorimeter* de, double& e, double& x, double& y );

    /** Calculate the "energy", X and Y position of the cluster
     *  as a sum of energies/x/y of its digits, weighted with energy fractions
     *
     *  "IT" could be either iterator or const_iterator
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
     */
    template <class IT, class DE>
    StatusCode calculateEXY( IT begin, IT end, DE de, double& e, double& x, double& y ) {
      return CaloDataFunctor::calculateClusterEXY( begin, end, de, e, x, y );
    }

    /** Useful function to determine, if clusters have at least one common cell.
     *
     *  For invalid arguments return "false"
     *
     *  @param   cl1   pointer to first  cluster
     *  @param   cl2   pointer to second cluster
     *  @return "true" if clusters have at least 1 common cell
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   xx/xx/xxxx
     */
    bool overlapped( const LHCb::CaloCluster* cl1, const LHCb::CaloCluster* cl2 );

    /** Useful function to find first common digit from two sequences.
     *  It returns the pair of
     *  iterators (first one for first sequence and
     *  the secons one for second sequences).
     *
     *  "IT" could be either iterator or const_iterator
     *
     *  @param  begin1 iterator pointing to 1st    element of 1st sequence
     *  @param  end1   iterator pointing to last+1 element of 1st sequence
     *  @param  begin2 iterator pointing to 1st    element of 2nd sequence
     *  @param  end2   iterator pointing to last+1 element of 2nd sequence
     *  @return pair of iterators
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   xx/xx/xxxx
     */
    template <class IT>
    std::pair<IT, IT> commonDigit( IT begin1, IT end1, IT begin2, IT end2 ) {
      return CaloDataFunctor::clusterCommonDigit( begin1, end1, begin2, end2 );
    }

    /** Useful function to locate the digit within the sequence
     *
     *  "IT" could be either iterator or const_iterator
     *
     *  @param begin iterator pointing to the 1st    element of sequence
     *  @param end   iterator pointing to teh last+1 element of sequence
     *  @param digit pointer to CaloDigit
     *  @return location of digit within the sequence
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     */
    template <class IT>
    IT locateDigit( IT begin, IT end, const LHCb::CaloDigit* digit ) {
      return CaloDataFunctor::clusterLocateDigit( begin, end, digit );
    }

    /** Locate the digit with given status from sequence of digits
     *
     *  "IT" could be either iterator or const_iterator
     *
     *  @param begin  iterator pointing to the 1st    element of sequence
     *  @param end    iterator pointing to teh last+1 element of sequence
     *  @param st     status
     *  @return location of digit within the sequence
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     */
    template <class IT>
    IT locateDigit( IT begin, IT end, const LHCb::CaloDigitStatus::Status& st ) {
      return CaloDataFunctor::clusterLocateDigit( begin, end, st );
    }

    /** The simple class/function to get the index of area in Calo
     *  "calo-area" of cluster is defined as "calo-area" index of seed cell
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    class ClusterArea final {
    public:
      /** the only one essential method
       *  @exception CaloException for invalid cluster
       *  @param cluster pointer to CaloFutureCluster object
       *  @return index of calorimeter area for given cluster
       */
      unsigned int operator()( const LHCb::CaloCluster* cluster ) const {
        if ( !cluster ) { Exception( " CaloCluster* points to NULL! " ); }
        // find seed cell
        auto seed = locateDigit( cluster->entries().begin(), cluster->entries().end(), CaloDigitStatus::SeedCell );
        if ( cluster->entries().end() == seed ) { Exception( " 'SeedCell' is not found!" ); }
        const LHCb::CaloDigit* digit = seed->digit();
        if ( !digit ) { Exception( " CaloDigit* points to NULL for seed!" ); }
        // get the area
        return digit->cellID().area();
      };

    protected:
      /** throw the exception
       *  @see CaloException
       *  @exception CaloException
       *  @param     message exception message
       *  @return    status code (fictive)
       */
      [[noreturn]] void Exception( const std::string& message ) const { throwException( "ClusterArea() " + message ); };
    };

    /** The simple class/function to get the index of area in Calo
     *  "calo-area" of cluster is defined as "calo-area" index of seed cell
     *
     *  @see ClusterArea
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    inline unsigned int clusterArea( const LHCb::CaloCluster* cluster ) {
      ClusterArea evaluator;
      return evaluator( cluster );
    }

    /** @class ClusterCaloFuture
     *
     *  The simple class/function to get the index of calorimeter.
     *  Index of calorimeter for clusers
     *  is defined as "calo" index of the seed cell
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    class ClusterCaloFuture final {
    public:
      /** the only one essential method
       *  @exception CaloException for invalid cluster
       *  @param cluster pointer to CaloCluster object
       *  @return index of calorimeter area for given cluster
       */
      unsigned int operator()( const LHCb::CaloCluster* cluster ) const {
        if ( !cluster ) { Exception( " CaloCluster* points to NULL! " ); }
        // find seed cell
        auto seed =
            locateDigit( cluster->entries().begin(), cluster->entries().end(), LHCb::CaloDigitStatus::SeedCell );
        if ( cluster->entries().end() == seed ) { Exception( " 'SeedCell' is not found!" ); }
        const LHCb::CaloDigit* digit = seed->digit();
        if ( !digit ) { Exception( " CaloDigit* points to NULL for seed!" ); }
        // get the area
        return digit->cellID().calo();
      };

    protected:
      /** throw the exception
       *  @see CaloException
       *  @exception CaloException
       *  @param     message exception message
       *  @return    status code (fictive)
       */
      [[noreturn]] void Exception( const std::string& message ) const {
        throwException( "ClusterCaloFuture() " + message );
      };
    };

    /** The simple class/function to get the index of calorimeter.
     *  Index of calorimeter for clusers
     *  is defined as "calo" index of the seed cell
     *
     *  @see ClusterArea
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    inline unsigned int clusterCaloFuture( const LHCb::CaloCluster* cluster ) {
      ClusterCaloFuture evaluator;
      return evaluator( cluster );
    }

    /** @class  ClusterFromCaloFuture
     *
     *  simple predicate/functor to select cluster from given calorimeter
     *
     *  @see ClusterCaloFuture
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    class ClusterFromCaloFuture final {
    public:
      /** constructor
       *  @see         CaloCellCode
       *  @exception   CaloException for invalid calorimeter name
       *  @param calo  name of calorimeter (full or abbreviated)
       */
      ClusterFromCaloFuture( const std::string& calo )
          : m_calo( CaloCellCode::CaloNumFromName( calo ) ), m_evaluator() {
        if ( m_calo < 0 ) { Exception( "Wrong Calo Name='" + calo + "'" ); }
      };
      /** the only one essential method
       *  @see ClusterCaloFuture
       *  @exception CaloException fro ClusterCaloFuture class
       *  @param cluster pointer to CaloFutureCluster object
       *  @return true if cluster belongs to tehselected calorimter
       */
      bool operator()( const LHCb::CaloCluster* cluster ) const { return m_evaluator( cluster ) == m_calo; };
      /** set new calorimeter name
       *  @exception   CaloException for invalid calorimeter name
       *  @param calo  name of calorimeter (full or abbreviated)
       */
      void setCaloFuture( const std::string& calo ) {
        m_calo = CaloCellCode::CaloNumFromName( calo );
        if ( m_calo < 0 ) { Exception( "Wrong CaloFuture Name='" + calo + "'" ); }
      }

    protected:
      /** throw the exception
       *  @see CaloException
       *  @exception CaloException
       *  @param     message exception message
       *  @return    status code (fictive)
       */
      [[noreturn]] void Exception( const std::string& message ) const {
        throwException( "ClusterFromCaloFuture() " + message );
      };

    private:
      CaloCellCode::CaloIndex m_calo = CaloCellCode::CaloIndex( -1 );
      ClusterCaloFuture       m_evaluator;
    };

    /** @class  ClusterFromArea
     *
     *  simple predicate/functor to select cluster from given
     *  area in calorimeter
     *
     *  @see ClusterArea
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    class ClusterFromArea final {
    public:
      /** constructor
       *  @param area area index
       */
      ClusterFromArea( unsigned int area ) : m_area( area ) {}
      /** the only one essential method
       *  @see ClusterArea
       *  @exception CaloException from ClusterArea class
       *  @param cluster pointer to CaloCluster object
       *  @return true if cluster belongs to the selected area in calorimter
       */
      bool operator()( const LHCb::CaloCluster* cluster ) const { return m_evaluator( cluster ) == m_area; };

    private:
      unsigned int m_area{};
      ClusterArea  m_evaluator{};
    };

    /** @class OnTheBoundary
     *
     *  Simple utility to locate clusters, placed on the boundaries
     *  between different calorimeter zones
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    class OnTheBoundary final {
    public:
      /** the only one essential method
       *  @param cluster pointer to CaloCluster object
       *  @return true if cluster is on the boundary,
       *               for empty clusters "false" is returned
       *  @exception CaloException for invalid clusters
       */
      inline bool operator()( const LHCb::CaloCluster* cluster ) const {
        if ( !cluster ) { Exception( "CaloCluster* points to NULL!" ); }
        const LHCb::CaloCluster::Entries& entries = cluster->entries();
        if ( entries.size() <= 1 ) { return false; } // RETURN !!!
        auto seed = locateDigit( entries.begin(), entries.end(), LHCb::CaloDigitStatus::SeedCell );
        if ( entries.end() == seed ) { Exception( "'SeedCell' is not found!" ); }
        const LHCb::CaloDigit* sd = seed->digit();
        if ( !sd ) { Exception( "CaloDigit* for 'SeedCell' is  NULL!" ); }
        const unsigned int seedArea = sd->cellID().area();
        for ( auto entry = entries.begin(); entries.end() != entry; ++entry ) {
          const LHCb::CaloDigit* digit = entry->digit();
          if ( !digit ) { continue; }
          if ( seedArea != digit->cellID().area() ) { return true; }
        } // end of loop over all cluyster entries
        //
        return false;
      };

    protected:
      /** throw the exception
       *  @see CaloException
       *  @exception CaloException
       *  @param     message exception message
       *  @return    status code (fictive)
       */
      [[noreturn]] void Exception( const std::string& message ) const {
        throwException( "OnTheBoundary() " + message );
      };
    };

    /** Simple utility to locate clusters, placed on the boundaries
     *  between different calorimeter zones
     *
     *  @see OnTheBoundary
     *  @param cluster pointer to CaloCluster object
     *  @return true if cluster is on the boundary,
     *               for empty clusters "false" is returned
     *  @exception CaloException for invalid clusters
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    inline bool onTheBoundary( const LHCb::CaloCluster* cluster ) {
      OnTheBoundary evaluator;
      return evaluator( cluster );
    }

    /** @class ZPosition
     *
     *  The simple function to get the cluster z-posiiton as a z-position of
     *  "Seed Cell"
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   02/12/2001
     */
    class ZPosition final {
    public:
      /** the explicit constructor
       *  @param detector source of calorimeter detector information
       */
      ZPosition( const DeCalorimeter* detector ) : m_detector( detector ){};
      /** the only one essential method
       *  @exception CaloException if detector is not valid
       *  @exception CaloException if cluster is NULL
       *  @exception CaloException if cluster is Empty
       *  @exception CaloException if cluster has no SEED cell
       *  @exception CaloException if SEED digit is NULL
       *  @param cluster pointer to CaloCluster object
       *  @return z-position
       */
      double operator()( const LHCb::CaloCluster* cluster ) const {
        if ( !m_detector ) { Exception( " DeCalorimeter*     points to NULL! " ); }
        if ( !cluster ) { Exception( " const CaloCluster* points to NULL! " ); }
        if ( cluster->entries().empty() ) { Exception( " CaloCluster is empty! " ); }
        auto iseed = locateDigit( cluster->entries().begin(), cluster->entries().end(), CaloDigitStatus::SeedCell );
        if ( cluster->entries().end() == iseed ) { Exception( " The Seed Cell is not found! " ); }
        ///
        const LHCb::CaloDigit* seed = iseed->digit();
        if ( !seed ) { Exception( " The Seed Digit points to NULL! " ); }
        //
        return m_detector->cellCenter( seed->cellID() ).z();
      };

    protected:
      /** throw the exception
       *  @see CaloException
       *  @exception CaloException
       *  @param     message exception message
       *  @return    status code (fictive)
       */
      [[noreturn]] void Exception( const std::string& message ) const { throwException( "ZPosition() " + message ); };

    private:
      const DeCalorimeter* m_detector; ///< source of geometry information
    };

    /** Helpful function to tag the sub cluster according to the
     *  fractionc evaluated by "evaluator"
     *
     *  Error codes
     *
     *          - 225 : CaloCluster* points to NULL
     *          - 226 : Entry with status 'SeedCell' is not found
     *          - 227 : Entry with status 'SeedCell' is invalid
     *
     *
     *  @param cluster pointer to CaloCluster object
     *  @param evaluator  evaluator object
     *  @param modify  flag for modification of energy fractions
     *  @param tag     tag to be set for cells with modified fractions
     *
     *  @author Vanya Belyaev Ivan Belyaev
     *  @date   01/04/2002
     */

    template <class EVALUATOR>
    inline StatusCode tagTheSubCluster( LHCb::CaloCluster* cluster, const EVALUATOR& evaluator, const bool modify,
                                        const LHCb::CaloDigitStatus::Status& status,
                                        const LHCb::CaloDigitStatus::Status& tag ) {
      // check the arguments
      if ( !cluster ) { return StatusCode( 225 ); }
      // get all entries
      LHCb::CaloCluster::Entries& entries = cluster->entries();
      // find seed digit
      auto seedEntry = locateDigit( entries.begin(), entries.end(), CaloDigitStatus::SeedCell );
      // check the seed
      if ( entries.end() == seedEntry ) { return StatusCode( 226 ); }
      const LHCb::CaloDigit* seed = seedEntry->digit();
      if ( !seed ) { return StatusCode( 227 ); }
      // loop over all entries

      for ( auto entry = entries.begin(); entries.end() != entry; ++entry ) {
        // reset existing statuses
        // std::cout << entry->digit()->cellID() << " initial status " << entry->status() << std::endl;
        entry->removeStatus( tag );
        entry->removeStatus( status );
        // std::cout << " removing status " << tag << " & " << status << " -> " << entry->status() << std::endl;

        // skip invalid digits
        const LHCb::CaloDigit* digit = entry->digit();
        if ( !digit ) { continue; }
        // evaluate the fraction
        const double fraction = evaluator( seed->cellID(), digit->cellID() );
        if ( 0 >= fraction ) { continue; }
        // update statuses
        entry->addStatus( status );
        // std::cout << " New status " << entry->status() << std::endl;

        if ( !modify ) { continue; }
        // modify the fractions
        entry->setFraction( entry->fraction() * fraction );
        entry->addStatus( tag );
      }
      ///
      return StatusCode::SUCCESS;
    }

    /** Helpful function to untag the sub cluster according to the
     *  fraction evaluated by "evaluator"
     *
     *  Error codes
     *
     *          - 225 : CaloCluster* points to NULL
     *          - 226 : Entry with status 'SeedCell' is not found
     *          - 227 : Entry with status 'SeedCell' is invalid
     *
     *
     *  @param cluster pointer to CaloCluster object
     *  @param evaluator  evaluator object
     *  @param tag     tag to be set for cells with modified fractions
     *
     *  @see CaloCuster
     *  @see tagTheSubCluster
     *
     *  @author Vanya Belyaev Ivan Belyaev
     *  @date   01/04/2002
     */

    template <class EVALUATOR>
    inline StatusCode untagTheSubCluster( LHCb::CaloCluster* cluster, const EVALUATOR& evaluator,
                                          const LHCb::CaloDigitStatus::Status& tag ) {
      // check the arguments
      if ( !cluster ) { return StatusCode( 225 ); }
      // get all entries
      LHCb::CaloCluster::Entries& entries = cluster->entries();
      // find seed digit
      auto seedEntry = locateDigit( entries.begin(), entries.end(), LHCb::CaloDigitStatus::SeedCell );
      // check the seed
      if ( entries.end() == seedEntry ) { return StatusCode( 226 ); }
      const LHCb::CaloDigit* seed = seedEntry->digit();
      if ( !seed ) { return StatusCode( 227 ); }
      // loop over all entries
      for ( auto entry = entries.begin(); entries.end() != entry; ++entry ) {
        // reset existing statuses
        entry->addStatus( LHCb::CaloDigitStatus::UseForEnergy );
        entry->addStatus( LHCb::CaloDigitStatus::UseForPosition );
        entry->addStatus( LHCb::CaloDigitStatus::UseForCovariance );
        // tagged ?
        if ( !( tag | entry->status() ) ) { continue; } // CONTINUE
        // skip invalid digits
        const LHCb::CaloDigit* digit = entry->digit();
        if ( !digit ) { continue; } // CONTINUE
        // evaluate the fraction
        const double fraction = evaluator( seed->cellID(), digit->cellID() );
        if ( 0 >= fraction ) { continue; } // CONTINUE
        // modify the fractions
        entry->setFraction( entry->fraction() / fraction );
        entry->removeStatus( tag );
      }
      ///
      return StatusCode::SUCCESS;
    }

  } // namespace ClusterFunctors
} // namespace LHCb

// ============================================================================
#endif ///< CALOFUTUREUTILS_CLUSTERFUNCTORS_H
