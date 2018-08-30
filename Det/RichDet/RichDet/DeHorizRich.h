
/** @file DeHorizRich.h
 *
 *  Header file for detector description class : DeHorizRich
 *
 *  @author Antonis Papanestis antonis.papanestis@stfc.ac.uk
 *  @date   2012-10-18
 */

#pragma once

// local
#include "RichDet/DeRich.h"

// External declarations
extern const CLID CLID_DEHorizRich;

/** @class DeHorizRich RichDet/DeHorizRich.h
 *
 * This is the DeRich detector element for HorizRich
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 */

class DeHorizRich : public DeRich
{

public:

  /**
   * Constructor for this class
   */
  DeHorizRich( const std::string &name = "" );

  /**
   * Default destructor
   */
  virtual ~DeHorizRich() = default;

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
   * Specific HorizRich initialisation
   * @return Status of initialisation
   */
  StatusCode initialize() override;

private:

  /// Access the name for a given panel
  const std::string panelName( const Rich::Side panel ) const override;
};
