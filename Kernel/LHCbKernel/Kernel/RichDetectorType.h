
//-----------------------------------------------------------------------------
/** @file RichDetectorType.h
 *
 *  Header file for RICH particle ID enumeration : RichDetectorType
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   08/07/2004
 */
//-----------------------------------------------------------------------------

#ifndef LHCBKERNEL_RICHDETECTORTYPE_H
#define LHCBKERNEL_RICHDETECTORTYPE_H 1

// std include
#include <string>
#include <vector>
#include <iostream>

//  General namespace for RICH specific definitions documented in RichSide.h
namespace Rich
{

  /// Number of RICH detectors
  static const unsigned int NRiches = 2;

  /** @enum Rich::DetectorType
   *
   *  RICH Detector types
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   08/07/2004
   */
  enum DetectorType
    {
      InvalidDetector = -1, ///< Unspecified Detector
      Rich1           = 0,  ///< RICH1 detector
      Rich2           = 1   ///< RICH2 detector
    };

  /** Text conversion for DetectorType enumeration
   *
   *  @param detector RICH detector enumeration
   *  @return Detector type as a string
   */
  std::string text( const Rich::DetectorType detector );

  /// Std Vector typedef
  typedef std::vector<Rich::DetectorType> DetectorVector;

  /// Shortcut to a list of Detectors
  typedef DetectorVector Detectors;

  /// Access a vector of valid detector types
  const Detectors & detectors();

  /// Implement textual ostream << method for Rich::DetectorType enumeration
  inline std::ostream& operator << ( std::ostream & s,
                                     const Rich::DetectorType & detector )
  {
    return s << Rich::text( detector );
  }

}

#endif // LHCBKERNEL_RICHDETECTORTYPE_H
