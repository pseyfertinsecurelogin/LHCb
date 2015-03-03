// $Id: Cluster.h,v 1.6 2007-01-16 12:42:17 cattanem Exp $
#ifndef TsaCluster_H
#define TsaCluster_H 1

// Include files
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/Point3DTypes.h"
#include "Kernel/LHCbID.h"

#include <vector>
#include <algorithm>

/** @class TsaCluster TsaCluster.h 
 *
 *  cluster class for Tsa
 *
 *  @author Matthew Needham
 *  created Wed Jun 25 14:16:12 2003
 *
 */


#include <memory>
#include "Kernel/Trajectory.h"

namespace LHCb{
  class Trajectory;  
}

namespace Tsa{
  class Cluster: public ContainedObject
  {
  public: 

    /// special constructer
    Cluster(std::auto_ptr<LHCb::Trajectory> traj, const double& error, bool isHot = false);

    /// Copy constructor
    Cluster(const Tsa::Cluster& other);

    /// Destructor 
    virtual ~Cluster();

    /// clone
    virtual Cluster* clone() const = 0;

    /// error
    double error() const;

    /// trajectory
    const LHCb::Trajectory& trajectory() const; 

    /// used count
    unsigned int pass() const;

    /// update count
    void updatePass(const unsigned int pass);

    /// on track
    bool onTrack() const;

    /// update 
    void setOnTrack(const bool isOnTrack);

    /// test if inside y range of strip 
    bool isInsideY(const double y, const double tol) const;

    /// test if inside x range of strip 
    bool isInsideX(const double x, const double tol) const;

    /// start of trajectory
    Gaudi::XYZPoint beginPoint() const;

    /// midpoint of trajectory
    Gaudi::XYZPoint midPoint() const; 

    /// end of trajectory 
    Gaudi::XYZPoint endPoint() const;

    // short cut direction of the tracjectory - assume const
    Gaudi::XYZVector direction() const;

    // x of middle of trajectory
    double xMid() const;


    //y of middle of trajectory
    double yMid() const;

    // z of middle of trajectory
    double zMid() const;


    /// return the weight - IT is worth twice OT
    virtual unsigned int weight() const = 0;

    /// return the LHCbID
    virtual LHCb::LHCbID id() const = 0;
   
    /// min y
    double yMin() const;

    /// max y  
    double yMax() const;

    /// check if in a hot area
    bool isHot() const;

    /// check the length
    double length() const;

    /// same LHCbID
    virtual bool sameID(const LHCb::LHCbID testID) const = 0; 

  private: 
 
    std::auto_ptr<LHCb::Trajectory> m_trajectory;
    //  Gaudi::XYZPoint m_point;
    double m_error; ///< measured val error
    bool m_isHot;
    unsigned int m_pass;
    bool m_onTrack;
    double m_yMin;
    double m_yMax;

  };
};


#include "Kernel/Trajectory.h"
#include <algorithm>

inline Tsa::Cluster::Cluster(std::auto_ptr<LHCb::Trajectory> traj, const double& error, bool isHot): 
  ContainedObject(),
  m_trajectory(traj),                               
  m_error(error), 
  m_isHot(isHot),
  m_pass(0),
  m_onTrack(false) 
{
  // constructer  
  // m_point = beginPoint() + 0.5*(endPoint() - beginPoint());

  //cache yMin and yMax
  double yMin = beginPoint().y();
  double yMax = endPoint().y();

  if (yMin > yMax) std::swap(yMin, yMax);

  m_yMin = yMin;
  m_yMax = yMax;

}

inline const LHCb::Trajectory& Tsa::Cluster::trajectory() const{
  return *(m_trajectory.get());
}

inline Gaudi::XYZPoint Tsa::Cluster::beginPoint() const{
  return m_trajectory->beginPoint();
}

inline Gaudi::XYZPoint Tsa::Cluster::endPoint() const{
  return m_trajectory->endPoint();
}

inline Gaudi::XYZPoint Tsa::Cluster::midPoint() const{
  //return m_point;
  return beginPoint() + 0.5*(endPoint() - beginPoint());
}

inline double Tsa::Cluster::xMid() const {
  return midPoint().x();
}

inline double Tsa::Cluster::yMid() const {
  return midPoint().y();
}

inline double Tsa::Cluster::zMid() const {
  return midPoint().z();
}

inline Gaudi::XYZVector Tsa::Cluster::direction() const{
  return m_trajectory->direction(0.);
}

inline double Tsa::Cluster::yMin() const{
  return m_yMin;
}

inline double Tsa::Cluster::yMax() const{
  return m_yMax;
}

inline double Tsa::Cluster::length() const{
  return m_trajectory->length();
}

inline bool Tsa::Cluster::isHot() const{
  return m_isHot;
}

inline double Tsa::Cluster::error() const 
{
  return m_error;
}

inline unsigned int Tsa::Cluster::pass() const{
  return m_pass;
}

inline void Tsa::Cluster::updatePass(const unsigned int pass){
  m_pass = pass;
}

inline bool Tsa::Cluster::onTrack() const{
  return m_onTrack;
}

inline void Tsa::Cluster::setOnTrack(const bool isOnTrack){
  m_onTrack = isOnTrack;
}

inline bool Tsa::Cluster::isInsideY(const double y, 
                                 const double tol) const{
  return ((y<(yMax()+tol))&&(y>(yMin()-tol)));
}

inline bool Tsa::Cluster::isInsideX(const double x, 
                                    const double tol) const{

 // get x range
 double xMin = beginPoint().x();
 double xMax = endPoint().x();
 if (xMin > xMax) std::swap(xMin, xMax);
 return ((x<(xMax+tol))&&(x>(xMin-tol)));
}

//Defintion of keyed container for TsaCluster
namespace Tsa{
 typedef ObjectVector<Tsa::Cluster> Clusters;
}
#endif   /// TsaCluster_H

