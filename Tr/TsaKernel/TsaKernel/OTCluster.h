// $Id: OTCluster.h,v 1.7 2006-10-13 08:44:36 mneedham Exp $
#ifndef Tsa_OTCluster_H
#define Tsa_OTCluster_H 1

// Include files
#include "Kernel/OTChannelID.h"
#include "Event/OTTime.h"
#include "TsaKernel/Cluster.h"
#include "GaudiKernel/boost_allocator.h"

class DeOTDetector;
class DeOTModule;

namespace Tsa{

  // Class ID definition
  static const CLID& CLID_OTCluster = 19003;

  // Namespace for locations in TDS
  namespace OTClusterLocation {
    static const std::string& Default = "/Event/Rec/Tsa/OTClusters";
  }


  /** @class OTCluster OTCluster.h 
   *
   *  Inner tracker cluster class for Tsa
   *
   *  @author Matthew Needham
   *  created Wed Jun 25 14:16:12 2003
   *
   */

  class OTCluster: public Tsa::Cluster{

    public: 

    /// special constructer
    OTCluster(std::auto_ptr<LHCb::Trajectory> traj, 
              const double& error,   
              double driftRadius,
              DeOTDetector* detector, 
              LHCb::OTTime* time, 
              bool isHot = false);

   /// specical constructor
   OTCluster( DeOTModule* aModule,  
              const double& error,   
              DeOTDetector* detector, 
              LHCb::OTTime* time, 
	      double fudgeDist, 
              bool isHot = false);

    /// Copy constructor
    OTCluster(const Tsa::OTCluster& other);

    /// Destructor 
    virtual ~OTCluster() ;

    /// clone
    OTCluster* clone() const;

    /// Retrieve pointer to class definition structure
    virtual const CLID& clID() const; 
    static const CLID& classID(); 

    double driftRadius() const;

    // set the driftTime
    void setDriftRadius(const double x, const double y);

    // set the driftRadius
    void setDriftRadius(const double radius);

    double driftRadius(const double x, const double y) const;

     /// drift time
    double driftTime() const;

    /// Retrieve  channel id
    const LHCb::OTChannelID channelID() const; 

    /// Retrieve  channel id - compatible with OTFunctors..
    const LHCb::OTChannelID channel() const; 

    /// Retrieve lite cluster
    LHCb::OTTime* cluster() const;

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
      return ( sizeof(OTCluster) == size ?
               boost::singleton_pool<OTCluster, sizeof(OTCluster)>::malloc() :
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
      boost::singleton_pool<OTCluster, sizeof(OTCluster)>::is_from(p) ?
      boost::singleton_pool<OTCluster, sizeof(OTCluster)>::free(p) :
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

    double m_driftRadius;
    DeOTDetector* m_det;
    LHCb::OTTime* m_cluster; 

  };
};

#include "OTDet/DeOTDetector.h"
#include "OTDet/DeOTModule.h"

inline Tsa::OTCluster::OTCluster(
		     std::auto_ptr<LHCb::Trajectory> traj,
                     const double& error,
                     double driftRadius,   
                     DeOTDetector* detector, 
		     LHCb::OTTime* time, 
                     bool isHot):
Tsa::Cluster(traj, error, isHot),
m_driftRadius(driftRadius),
m_det(detector),
m_cluster(time)
{
  // constructer
}


inline Tsa::OTCluster::OTCluster(
		     DeOTModule* aModule,
                     const double& error,   
                     DeOTDetector* detector, 
		     LHCb::OTTime* time, 
                     double fudgeDist,
                     bool isHot):
Tsa::Cluster(aModule->trajectory(time->channel()), error, isHot),
m_det(detector),
m_cluster(time)
{
  // constructer
  const double timeAlongWire =
          ((0.5*length())+fudgeDist)*m_det->propagationDelay();
  m_driftRadius =  
         m_det->driftDistance(m_cluster->calibratedTime()-timeAlongWire);
}


inline const LHCb::OTChannelID Tsa::OTCluster::channelID() const 
{
  return m_cluster->channel();
}

inline const LHCb::OTChannelID Tsa::OTCluster::channel() const 
{
  return channelID();
}

inline LHCb::OTTime* Tsa::OTCluster::cluster() const
{
   return m_cluster;
}

inline double Tsa::OTCluster::driftTime() const {
  return m_cluster->calibratedTime();
}


inline double Tsa::OTCluster::driftRadius() const{
  return m_driftRadius;
}


inline void Tsa::OTCluster::setDriftRadius(const double x,
				    	   const double y) {
  m_driftRadius = driftRadius(x,y);  
}

inline void Tsa::OTCluster::setDriftRadius(const double radius) {
  m_driftRadius = radius;  
}

//Defintion of keyed container for OTCluster
namespace Tsa{
  typedef ObjectVector<Tsa::OTCluster> OTClusters;
};
#endif   ///TsaEvent_OTCluster_H

