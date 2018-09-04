// ============================================================================
#ifndef UTKERNEL_IUTCHANNELIDSELECTOR_H 
#define UTKERNEL_IUTCHANNELIDSELECTOR_H 1
// Include files
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// forward declaration
namespace LHCb{
  class  UTChannelID;    
}

/** @class IUTChannelID IUTChannelID.h
 *  
 *  An abstract interface for cluster "Selector"/"Preselector" 
 *  tools, which allow a fast and efficient selection/preselection 
 *  of clusters 
 *
 *
 *  @author M Needham
 *  @date   03/02/2009
 */

struct IUTChannelIDSelector : extend_interfaces<IAlgTool>
{
  
  /** static interface identification
   *  @see IInterface
   *  @see IID_IUTChannelIDSelector
   *  @return the unique interface identifier
   */
  DeclareInterfaceID( IUTChannelIDSelector , 2 , 0 );
  
  
  /** "select"/"preselect" method 
   *  @param  cluster pointer to calo cluster object to be selected 
   *  @return true if cluster is selected
   */
  virtual bool select     ( const LHCb::UTChannelID& chan ) const = 0 ;
  
  /** "select"/"preselect" method (functor interface)
   *  @param  cluster pointer to calo cluster object to be selected 
   *  @return true if cluster is selected
   */
  virtual bool operator() ( const LHCb::UTChannelID& chan ) const = 0 ;
  
};
  
// ============================================================================
#endif // UT_IUTCHANNELIDSELECTOR_H
// ============================================================================
