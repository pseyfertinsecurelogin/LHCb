// ============================================================================
// Include files 
// ============================================================================
// GaudiKernel
// ============================================================================
// GaudiKernel
// ============================================================================
#include <ostream>

#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
// ============================================================================
// LoKi
// ============================================================================
// IWYU issue 441
#include "LoKi/Colors.h" // IWYU pragma: keep
// ============================================================================
/** @file
 *
 *  Helper file with functions to provide colorized output 
 * 
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 */
// ============================================================================
/*  set the colors for output stream (not actiev for WIN32)
 *  @param  stream  reference for stream 
 *  @param  fg      color for foreground 
 *  @param  fg      color for foreground 
 *  @return true if color are changed 
 */
// ============================================================================
bool LoKi::Colors::setColor
( std::ostream&     stream , 
  const MSG::Color& fg     , 
  const MSG::Color& bg     )
{
#ifndef WIN32    
  int fc =  30 +        fg ;
  int bc =  40 +        bg ;
  stream << "\033[" << bc  << "m\033[1;" <<  fc << "m";
  return true  ;
#else 
  return false ;
#endif 
}
// ============================================================================
/* reset the colors for output stream (not active for WIN32)
 *  @param  stream  reference for stream 
 *  @return true if color are changed 
 */
bool LoKi::Colors::resetColor ( std::ostream&     stream ) 
{
#ifndef WIN32 
  stream << "\033[0m" ;
  return true ;
#else 
  return false ;
#endif
}
// ============================================================================
/* reset the colors for output stream (not active for WIN32)
 *  @param  stream  reference for stream 
 *  @return true if color are changed 
 */
// ============================================================================
bool LoKi::Colors::resetColor ( MsgStream&        stream ) 
{
  stream.resetColor() ;
  return true ;
}
// ============================================================================
/*  set the colors for output stream 
 *  @param  stream  reference for stream 
 *  @param  fg      color for foreground 
 *  @param  fg      color for foreground 
 *  @return true if color are changed 
 */
// ============================================================================
bool LoKi::Colors::setColor 
( MsgStream&        stream ,
  const MSG::Color& fg     , 
  const MSG::Color& bg     )
{
  stream.setColor ( fg , bg );
  return true ;
}
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
