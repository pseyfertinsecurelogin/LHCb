#ifndef MULTIPLEMAGNETICFIELDSVC_H
#define MULTIPLEMAGNETICFIELDSVC_H 1

#include <Math/Vector3Dfwd.h>
#include <string>
// Include files
#include <vector>

#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/extends.h"
#include "Kernel/ILHCbMagnetSvc.h"

class ISvcLocator;
namespace LHCb {
class MagneticFieldGrid;
}  // namespace LHCb

/** @class MultipleMagneticFieldSvc MultipleMagneticFieldSvc.h
 *  A service for wrapping multiple MagneticFieldSvc
 *
 *  @author Magnus Lieng
 *  @date   2008-07-03
 */

class MultipleMagneticFieldSvc : public extends<Service, ILHCbMagnetSvc> {

public:
  /// Standard Constructor.
  /// @param  name   String with service name
  /// @param  svc    Pointer to service locator interface
  MultipleMagneticFieldSvc( const std::string& name, ISvcLocator* svc );

  /// Virtual destructor.
  ~MultipleMagneticFieldSvc() override;


  /// Initialise the service (Inherited Service overrides)
  StatusCode initialize() override;

  /// Finalise the service (Inherited Service overrides)
  StatusCode finalize() override;


  using ILHCbMagnetSvc::fieldVector; // avoid hiding compiler  warning...

  /** Implementation of ILHCbMagneticFieldSvc interface.
   * @param[in]  xyz Point at which magnetic field vector will be given
   * @return fvec Magnectic field vector.
   */
  ROOT::Math::XYZVector fieldVector( const Gaudi::XYZPoint&  xyz ) const override;

  // These should be incorporated in a better way...
  bool   useRealMap()  const override;  ///< True is using real map
  double scaleFactor() const;           ///< accessor to m_scaleFactor
  int polarity() const;                 ///< Polarity (only if they are all the same!)
  bool isDown() const override;         ///< sign of the polarity
  double signedRelativeCurrent() const override; ///< includes scale factor for polarity and current
  const LHCb::MagneticFieldGrid* fieldGrid() const override { return 0 ; }


private:

  std::vector<std::string> m_names;
  std::vector<SmartIF<ILHCbMagnetSvc>> m_magneticFieldSvcs; ///< Magnetic field services.
};

#endif  // MULTIPLEMAGNETICFIELDSVC_H
