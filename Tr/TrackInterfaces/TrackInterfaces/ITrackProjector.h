#ifndef TRACKINTERFACES_ITRACKPROJECTOR_H 
#define TRACKINTERFACES_ITRACKPROJECTOR_H 1

// Include files
// -------------
// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Geometry definitions
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/Point3DTypes.h"

// From TrackEvent
#include "Event/TrackTypes.h"

// Forward declarations
namespace LHCb {
  class State;
  class StateVector;
  class Measurement;
  class FitNode;
};

static const InterfaceID IID_ITrackProjector ( "ITrackProjector", 2, 0 );

/** @class ITrackProjector ITrackProjector.h
 *  
 *  Interface for tracking projector tools
 *
 *  @author Jose Hernando
 *  @author Eduardo Rodrigues
 *  @date   2005-03-15
 */
class ITrackProjector : virtual public IAlgTool {
public:
  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_ITrackProjector; }

  /// Project the reference vector
  virtual StatusCode project( const LHCb::StateVector& state,
                              const LHCb::Measurement& meas ) = 0;

  /// Project a state onto a measurement.
  virtual StatusCode project( const LHCb::State& state,
                              const LHCb::Measurement& meas ) = 0;
  
  /// Project the state vector in this fitnode and update projection matrix and reference residual
  virtual StatusCode projectReference( LHCb::FitNode& node ) const = 0;
  
  /// Retrieve the projection matrix H of the (last) projection
  virtual const Gaudi::TrackProjectionMatrix& projectionMatrix() const = 0;

  /// Retrieve the chi squared of the (last) projection
  virtual double chi2() const = 0;

  /// Retrieve the residual of the (last) projection
  virtual double residual() const = 0;

  /// Retrieve the error on the residual of the (last) projection
  virtual double errResidual() const = 0;

  /// Retrieve the error on the measure of the (last) projection
  virtual double errMeasure() const = 0;

  /// Retrieve the derivative of the residual wrt. the alignment parameters
  /// of the measurement. The details of the alignment transformation are 
  /// defined in AlignTraj.
  typedef Gaudi::Matrix1x6 Derivatives;
  virtual Derivatives alignmentDerivatives(const LHCb::StateVector& state, const LHCb::Measurement& meas, 
                                           const Gaudi::XYZPoint& pivot) const = 0;
};

//==============================================================================
//   end of class
//==============================================================================

#endif // TRACKINTERFACES_ITRACKPROJECTOR_H
