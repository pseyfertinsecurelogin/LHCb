// $Id: STCluster.h,v 1.4 2006-10-11 15:36:09 mneedham Exp $
#ifndef Tsa_STCluster_H
#define Tsa_STCluster_H 1

// Include files
#include "Kernel/STChannelID.h"
#include "Event/STLiteCluster.h"
#include "TsaKernel/Cluster.h"
#include "STDet/DeSTSector.h"

#include "GaudiKernel/boost_allocator.h"


namespace Tsa{

  // Class ID definition
  static const CLID& CLID_STCluster = 19003;

  // Namespace for locations in TDS
  namespace STClusterLocation {
    static const std::string& IT = "/Event/Rec/Tsa/ITClusters";
    static const std::string& TT = "/Event/Rec/Tsa/TTClusters";
  }


  /** @class STCluster STCluster.h 
   *
   *  Inner tracker cluster class for Tsa
   *
   *  @author Matthew Needham
   *  created Wed Jun 25 14:16:12 2003
   *
   */

  class STCluster: public Tsa::Cluster{

    public: 

    /// special constructer
    STCluster(std::auto_ptr<LHCb::Trajectory> traj,
              const double& error,    
              const LHCb::STLiteCluster& liteCluster, bool isHot = false);

   
    // special constructor
    STCluster(const DeSTSector* aSector,
              const double& error,    
              const LHCb::STLiteCluster& liteCluster, bool isHot = false);


    /// Copy constructor
    STCluster(const Tsa::STCluster& other);

    /// Destructor 
    virtual ~STCluster() ;

    STCluster* clone() const;
 
    /// Retrieve pointer to class definition structure
    virtual const CLID& clID() const; 
    static const CLID& classID(); 

    /// Retrieve  channel id
    const LHCb::STChannelID channelID() const; 

    /// Retrieve lite cluster
    LHCb::STLiteCluster cluster() const;

    /// retrieve LHCbID
    virtual LHCb::LHCbID id() const;

    /// weight
    unsigned int weight() const;

    /// same LHCbID
    virtual bool sameID(const LHCb::LHCbID testID) const; 

#ifndef _WIN32
    /// operator new
    static void* operator new ( size_t size )
    {
      return ( sizeof(STCluster) == size ?
               boost::singleton_pool<STCluster, sizeof(STCluster)>::malloc() :
               ::operator new(size) );
    }

    /// placement operator new
    /// it is needed by libstdc++ 3.2.3 (e.g. in std::vector)
    /// it is not needed in libstdc++ >= 3.4
    static void* operator new ( size_t size, void* pObj )
    {
      return ::operator new (size,pObj);
    }

    /// operator delete
    static void operator delete ( void* p )
    {
      boost::singleton_pool<STCluster, sizeof(STCluster)>::is_from(p) ?
      boost::singleton_pool<STCluster, sizeof(STCluster)>::free(p) :
      ::operator delete(p);
    }

    /// placement operator delete
    /// not sure if really needed, but it does not harm
    static void operator delete ( void* p, void* pObj )
    {
      ::operator delete (p, pObj);
    }
#endif

   private: 

    LHCb::STLiteCluster m_cluster; 
  };
};



inline Tsa::STCluster::~STCluster(){
  // destructer
}

inline Tsa::STCluster::STCluster(std::auto_ptr<LHCb::Trajectory> traj,
                         const double& error, const LHCb::STLiteCluster& liteCluster, bool isHot):
  Tsa::Cluster(traj,error, isHot), 
  m_cluster(liteCluster)                             
{
  // constructer  
}

inline Tsa::STCluster::STCluster(const DeSTSector* aSector,
                                 const double& error, const LHCb::STLiteCluster& liteCluster, bool isHot):
  Tsa::Cluster(aSector->trajectory(liteCluster.channelID(), 
                                   liteCluster.interStripFraction()),error, isHot), 
  m_cluster(liteCluster)                             
{
  // constructer  
}



/// Copy constructor
inline Tsa::STCluster::STCluster(const Tsa::STCluster& other):
Tsa::Cluster(other),
m_cluster(other.m_cluster){

}

inline const LHCb::STChannelID Tsa::STCluster::channelID() const 
{
  return m_cluster.channelID();
}

inline LHCb::STLiteCluster Tsa::STCluster::cluster() const
{
   return m_cluster;
}

//Defintion of keyed container for STCluster
namespace Tsa{
  //typedef KeyedContainer<Tsa::STCluster, Containers::HashMap> STClusters;
  typedef ObjectVector<Tsa::STCluster> STClusters;
};
#endif   ///TsaEvent_STCluster_H

