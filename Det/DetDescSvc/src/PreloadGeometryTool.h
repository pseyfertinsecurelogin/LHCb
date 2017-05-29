#ifndef PRELOADGEOMETRYTOOL_H
#define PRELOADGEOMETRYTOOL_H 1

// STL
#include <memory>
#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/IGenericTool.h"            // Interface
#include "GaudiKernel/extends.h"

class IGenericTool;
class IInterface;

/** @class PreloadGeometryTool PreloadGeometryTool.h
 *  Tool to traverse the full geometry tree and load it
 *
 *  @author Marco Cattaneo
 *  @date   2006-07-26
 */
struct PreloadGeometryTool : extends<GaudiTool,IGenericTool>
{
  /// Standard constructor
  PreloadGeometryTool( const std::string& type,
                       const std::string& name,
                       const IInterface* parent );

  void execute() override; ///< Load the geometry tree

};

#endif // PRELOADGEOMETRYTOOL_H
