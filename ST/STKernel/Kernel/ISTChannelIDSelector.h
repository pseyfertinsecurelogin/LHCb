// ============================================================================
#ifndef STKERNEL_ISTCHANNELIDSELECTOR_H 
#define STKERNEL_ISTCHANNELIDSELECTOR_H 1
// Include files
// STD & STL 
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// forward declaration
namespace LHCb{
  class  STChannelID;    
}

/** @class ISTChannelID ISTChannelID.h
 *  
 *  An abstract interface for cluster "Selector"/"Preselector" 
 *  tools, which allow a fast and efficient selection/preselection 
 *  of clusters 
 *
 *
 *  @author M Needham
 *  @date   03/02/2009
 */

struct ISTChannelIDSelector : extend_interfaces<IAlgTool>
{
  
  /** static interface identification
   *  @see IInterface
   *  @see IID_ISTChannelIDSelector
   *  @return the unique interface identifier
   */
  DeclareInterfaceID( ISTChannelIDSelector , 2 , 0 );
  
  
  /** "select"/"preselect" method 
   *  @param  cluster pointer to calo cluster object to be selected 
   *  @return true if cluster is selected
   */
  virtual bool select     ( const LHCb::STChannelID& chan ) const = 0 ;
  
  /** "select"/"preselect" method (functor interface)
   *  @param  cluster pointer to calo cluster object to be selected 
   *  @return true if cluster is selected
   */
  virtual bool operator() ( const LHCb::STChannelID& chan ) const = 0 ;
  
};
  
// ============================================================================
#endif // ST_ISTCHANNELIDSELECTOR_H
// ============================================================================
