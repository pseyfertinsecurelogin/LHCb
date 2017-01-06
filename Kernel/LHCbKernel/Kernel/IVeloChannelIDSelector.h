#ifndef IVELOCHANNELIDSELECTOR_H
#define IVELOCHANNELIDSELECTOR_H 1

// Include files
// -------------
// From STL
#include <string>

// From Gaudi
#include "GaudiKernel/IAlgTool.h"

// Forward declarations
namespace LHCb {
  class VeloChannelID;
}



/** @class IVeloChannelIDSelector IVeloChannelIDSelector.h Kernel/IVeloChannelIDSelector.h
 *
 *  Abstract interface for Velo channelID selection tools
 *  which allow a fast and efficient selection/preselection of Velo channelIDs
 *  (simply copied from the ST implementation)
 *
 *  @author Eduardo Rodrigues
 *  @date   2011-06-28
 */
struct IVeloChannelIDSelector : extend_interfaces<IAlgTool>
{

   // Return the interface ID
   DeclareInterfaceID(IVeloChannelIDSelector, 2, 0 );

   /** "select"/"preselect" method
    *  @param  channelID pointer to object to be selected
    *  @return true if channelID is selected
    */
   virtual bool select( const LHCb::VeloChannelID& cluster ) const = 0 ;

   /** "select"/"preselect" method (functor interface)
    *  @param  channelID pointer to object to be selected
    *  @return true if channelID is selected
    */
   virtual bool operator() ( const LHCb::VeloChannelID& cluster ) const = 0 ;

};
#endif // IVELOCHANNELIDSELECTOR_H
