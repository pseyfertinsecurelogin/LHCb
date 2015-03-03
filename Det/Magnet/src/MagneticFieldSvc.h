// $Id: MagneticFieldSvc.h,v 1.10 2006-01-20 16:24:11 cattanem Exp $
#ifndef MAGNETICFIELDSVC_H
#define MAGNETICFIELDSVC_H 1

// Include files
#include "GaudiKernel/Service.h"
#include "GaudiKernel/IMagneticFieldSvc.h"
#include "Kernel/Vector3DTypes.h"
#include "Kernel/Point3DTypes.h"

// Forward declarations
template <class TYPE> class SvcFactory;


/** @class MagneticFieldSvc MagneticFieldSvc.h
 *  A service for finding the magnetic field vector at a given
 *  point in space. Based on original implementation by I.Last and
 *  G.Gracia via Gaudi_gufld (modified from SICBMC)
 *
 *  @author Edgar De Oliveira
 *  @date   2002-05-21
 */

class MagneticFieldSvc : public Service,
                         virtual public IMagneticFieldSvc {

protected:
  
  /// Standard Constructor.
  /// @param  name   String with service name
  /// @param  svc    Pointer to service locator interface
  MagneticFieldSvc( const std::string& name, ISvcLocator* svc );
  
  /// Virtual destructor.
  virtual ~MagneticFieldSvc();

public:

  /// Initialise the service (Inherited Service overrides) 
  virtual StatusCode initialize();
  
  /// Finalise the service.
  virtual StatusCode finalize();
  
  /** Query the available interfaces.
   * @param riid Requested interface ID
   * @param ppvInterface Pointer to requested interface
   * @return StatusCode indicating SUCCESS or FAILURE.
   */
  virtual StatusCode queryInterface( const InterfaceID& riid, 
                                     void** ppvInterface      );
  
  /// Service type.
  virtual const InterfaceID& type() const { return IID_IMagneticFieldSvc; };

  /** IMagneticFieldSvc interface.
   * @param[in]  xyz Point at which magnetic field vector will be given
   * @param[out] fvec Magnectic field vector.
   * @return StatusCode SUCCESS if calculation was performed.
   */
  virtual StatusCode fieldVector( const Gaudi::XYZPoint&  xyz, 
                                        Gaudi::XYZVector& fvec ) const;

private:

  /// Allow SvcFactory to instantiate the service.
  friend class SvcFactory<MagneticFieldSvc>;

  /// Fills Q, the field vector
  void fieldGrid( const Gaudi::XYZPoint& xyz, Gaudi::XYZVector& fvec ) const;

  StatusCode parseFile( );       ///< Reads the field map from file 
  
  std::string m_filename;        ///< Magnetic field file name
  std::vector<double> m_Q;       ///< Field vector
  double m_Dxyz[3];              ///< Steps in x, y and z
  int    m_Nxyz[3];              ///< Number of steps in x, y and z
  double m_max_FL[3];    
  double m_min_FL[3];

  double m_zOffSet;              ///< The z offset

};

#endif  // MAGNETICFIELDSVC_H
