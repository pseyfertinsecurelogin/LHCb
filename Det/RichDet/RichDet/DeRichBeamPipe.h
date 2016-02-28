
/** @file DeRichBeamPipe.h
 *
 *  Header file for detector description class : DeRichBeamPipe
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2006-11-27
 */

#ifndef RICHDET_DERICHBEAMPIPE_H
#define RICHDET_DERICHBEAMPIPE_H 1

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ISolid.h"
#include "DetDesc/SolidCons.h"

// Local
#include "RichDet/DeRichBase.h"

// LHCbKernel

// External declarations
extern const CLID CLID_DERichBeamPipe;

/** @class DeRichBeamPipe RichDet/DeRichBeamPipe.h
 *
 * Class for generic description of the BeamPipe (equivalent) in the Rich
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 * @date   2006-11-27
 */
class DeRichBeamPipe : public DeRichBase
{

public:

  /// Enum describing the various possible types of intersection
  enum BeamPipeIntersectionType 
    {
      NoIntersection = 0, ///< Did not intersect the beam pipe at all
      FrontAndBackFace,   ///< Entered via the front face and left via the back face (i.e. totally inside the beampipe)
      FrontFaceAndCone,   ///< Entered via the front face and left via the cone surface
      BackFaceAndCone,    ///< Entered via the cone surafece and left via the backface
      ConeOnly            ///< Entered via the cone surafece and left via the cone surface
    };

  /**
   * Constructor for this class
   */
  DeRichBeamPipe(const std::string & name = "");

  /**
   * Default destructor
   */
  virtual ~DeRichBeamPipe();

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID();

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  virtual StatusCode initialize();

  /**
   * Finds the entry and exit points of the beam pipe equivalent (central
   * tube in Rich2). For boolean solids
   * this is the first and last intersection point.
   *
   * @param[in]  position   The start point to ray trace from
   * @param[in]  direction  The vector to ray trace from the start point
   * @param[out] entryPoint The entry point to the beam-pipe volume
   * @param[out] exitPoint  The exit point from the beam-pipe volume
   *
   * @return Enum describing the status of the intersection
   */
  BeamPipeIntersectionType intersectionPoints( const Gaudi::XYZPoint& position,
                                               const Gaudi::XYZVector& direction,
                                               Gaudi::XYZPoint& entryPoint,
                                               Gaudi::XYZPoint& exitPoint ) const;

  /** Test if a given direction intersects the beam-pipe volume at all.
   *  Faster than intersectionPoints since it does not compute the intersection points
   *  in global coordinates.
   *  @param[in]  position    The start point to ray trace from
   *  @param[in]  direction   The vector to ray trace from the start point
   *  @return boolean indicating if the beam pipe was intersected or not
   *  @retval true  The beam pipe was intersected
   *  @retval false The beam pipe was NOT intersected
   */
  inline bool testForIntersection( const Gaudi::XYZPoint&  position,
                                   const Gaudi::XYZVector& direction ) const
  {
    ISolid::Ticks ticks;
    return ( 0 != m_localCone->intersectionTicks ( geometry()->toLocal(position),
                                                   geometry()->toLocalMatrix()*direction,
                                                   ticks ) );
  }

  /**
   * Convert the enum to text for easy reading
   */
  static std::string text(const DeRichBeamPipe::BeamPipeIntersectionType& type);

private: // data

  const ISolid* m_solid = nullptr;  ///< solid used for the beam pipe

  /// A copy of the beam pipe cone that is solid (not hollow)
  SolidCons* m_localCone = nullptr;

  double m_zHalfLength{0};  ///< Half length of the cone along z

};

//==============================================================================

inline std::ostream& operator << ( std::ostream& s,
                                   const  DeRichBeamPipe::BeamPipeIntersectionType& type )
{
  return s << DeRichBeamPipe::text( type );
}

#endif    // RICHDET_DERICHBEAMPIPE_H
