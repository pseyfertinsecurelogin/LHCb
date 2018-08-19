
/** @file DeRich2.h
 *
 *  Header file for detector description class : DeRich2
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

#pragma once

// local
#include "RichDet/DeRich.h"

// External declarations
extern const CLID CLID_DERich2;

/** @class DeRich2 RichDet/DeRich2.h
 *
 * This is the DeRich detector element for Rich2
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 */

class DeRich2 : public DeRich
{

public:
  /**
   * Constructor for this class
   */
  DeRich2( const std::string &name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRich2() = default;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID &clID() const override { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID &classID();

  /**
   * Specific Rich2 initialisation
   * @return Status of initialisation
   */
  StatusCode initialize() override;

private:
  /// Access the name for a given panel
  const std::string panelName( const Rich::Side panel ) const override;

  /// Update Nominal mirror parameters
  StatusCode updateMirrorParams();
};
