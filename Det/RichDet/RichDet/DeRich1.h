
/** @file DeRich1.h
 *
 *  Header file for detector description class : DeRich1
 *
 *  @author Antonis Papanestis   a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

#pragma once

// local
#include "RichDet/DeRich.h"

// External declarations
extern const CLID CLID_DERich1;

/** @class DeRich1 RichDet/DeRich1.h
 *
 *  This is the DeRich detector element for Rich1
 *
 *  @author Antonis Papanestis   a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

class DeRich1 : public DeRich
{

public:
  /**
   * Constructor for this class
   */
  DeRich1( const std::string &name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRich1() = default;

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
   * Specific Rich1 initialisation
   * @return Status of initialisation
   */
  StatusCode initialize() override;

private:
  /// Access the name for a given panel
  const std::string panelName( const Rich::Side panel ) const override;

  /// Update Nominal mirror parameters
  StatusCode updateMirrorParams();
};
