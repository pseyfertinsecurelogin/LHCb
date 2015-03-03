// $Id: HybridBase.cpp,v 1.2 2007-12-11 18:37:04 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <sstream>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/System.h" 
// ============================================================================
// local
// ============================================================================
#include "LoKi/HybridBase.h"
// ============================================================================
// Python 
// ============================================================================
#include "Python.h"
// ============================================================================
/** @file 
 *  Implementation file for class LoKi::HybridTool
 *  
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2004-06-29
 */
// ============================================================================
// Standard constructor
// ============================================================================
LoKi::Hybrid::Base::Base
( const std::string& type   ,
  const std::string& name   ,
  const IInterface*  parent )
  : GaudiTool ( type , name , parent )
  //
  , m_pyInit   ( false )
  , m_showCode ( false ) 
{
  declareProperty( "ShowCode" , m_showCode , "Flag to display the prepared python code") ;
} 
// ============================================================================
// Destructor (virtual and protected) 
// ============================================================================
LoKi::Hybrid::Base::~Base() {} 
// ============================================================================
// initialization of the tool 
// ============================================================================
StatusCode LoKi::Hybrid::Base::initialize () 
{
  StatusCode sc = GaudiTool::initialize() ;
  if ( sc.isFailure() ) { return sc ; }  
  // Initialize python if it not yet done 
  if ( !Py_IsInitialized() ) 
  {
    info() << "Initialization of Python is triggered" << endreq ;
    Py_Initialize () ;
    m_pyInit = true ;
  }  
  return StatusCode::SUCCESS ;
} 
// ============================================================================
// finalization of the tool 
// ============================================================================
StatusCode LoKi::Hybrid::Base::finalize  () 
{
  // finalize python (if the owner)
  if ( Py_IsInitialized() && m_pyInit ) 
  {
    info() << "Finalization   of Python is triggered" << endreq ;
    std::string line ; 
    line += "                                                           # " + name() + "\n" ;
    line += "import atexit                                              # " + name() + "\n" ;
    line += "if atexit._exithandlers :                                  # " + name() + "\n" ;
    line += "\tprint 'ATTENTION: Clear non-empty atexit._exithandlers'  # " + name() + "\n" ;
    line += "\tprint 'len(_exithandlers)=',len(atexit._exithandlers)    # " + name() + "\n" ;
    line += "\tatexit._exithandlers=[]                                  # " + name() + "\n" ;
    line += "                                                           # " + name() + "\n" ;    
    const int result =  PyRun_SimpleString ( line.c_str() ) ;
    if ( 0 != result ) 
    {
      Warning ( "Error code from PyRun_SimpleString" , 1000 + result ).ignore() ;
      warning() << "The problematic code is \n" << line << endreq ;
      if ( PyErr_Occurred() ) { PyErr_Print() ; } 
    }
    Py_Finalize () ;
  }
  // finalize the base 
  return GaudiTool::finalize() ;
}
// ============================================================================
/* execute the python code 
 * @attention the method must be invoked only with appropriate scope!!!
 * @param code python code 
 * @return status code 
 */
// ============================================================================
StatusCode LoKi::Hybrid::Base::executeCode ( const std::string& pycode ) const 
{
  // Check the proper python environment:
  if ( !Py_IsInitialized() ) { return Error("Python is not initialized yet!") ; }
  
  // play with raw Python
  
  // local scope 
  PyObject* locals  = PyEval_GetLocals  () ;
  if ( 0 == locals ) 
  {
    Warning ( "PyEval_GetLocals:  'locals'  points to NULL" , StatusCode::SUCCESS ).ignore() ; 
    if ( PyErr_Occurred() ) { PyErr_Print() ; }
  }
  // global scope 
  PyObject* globals = PyEval_GetGlobals () ;
  bool globnew = false ;
  if ( 0 == globals ) 
  { 
    Warning ( "PyEval_GetGlobals: 'globals' points to NULL" , StatusCode::SUCCESS ).ignore() ; 
    if ( PyErr_Occurred() ) { PyErr_Print() ; }
    globals  = PyDict_New() ;
    if ( PyErr_Occurred() ) { PyErr_Print() ; }
    globnew  = true ;
    PyDict_SetItemString ( globals, "__builtins__" , PyEval_GetBuiltins() ) ;
    if ( PyErr_Occurred() ) { PyErr_Print() ; }
  }
  // ===========
  // ATTENTION! 
  // ===========
  // execute Python 'code' 
  PyObject* result = PyRun_String 
    ( const_cast<char*> ( pycode.c_str() ) ,                // EXECUTE CODE 
      Py_file_input  , globals  , locals ) ;
  
  if ( PyErr_Occurred()        ) { PyErr_Print() ; }
  
  if ( 0 != globals && globnew ) { Py_XDECREF( globals ) ; }
  
  if ( 0 != result             ) { Py_XDECREF ( result ) ; }
  else 
  {
    if ( PyErr_Occurred() ) { PyErr_Print() ; }
    err () << " Error has occured in Python: the problematic code is : "
           << endreq 
           << pycode  
           << endreq ;
    return Error( " Error has occured in Python " ) ;
  }
  return StatusCode::SUCCESS ;
}
// ============================================================================
/*  prepare the actual python code from the functor:
 *  @param modules list of modules 
 *  @param actor the actual actor
 *  @param code the code for the functor 
 *  @param lines more python lines to be used 
 *  @return the valid python code 
 */
// ============================================================================
std::string LoKi::Hybrid::Base::makeCode 
( const LoKi::Hybrid::Base::Strings& modules , 
  const std::string&                 actor   ,
  const std::string&                 code    , 
  const LoKi::Hybrid::Base::Strings& lines   ,
  const std::string&                 context ) const 
{
  std::ostringstream stream ;
  // start the code:
  stream << "# " << std::string(78,'=') << std::endl ;
  stream << "# python pseudomodule, generated for the tool '" 
         << name() << "'" << std::endl ;
  stream << "# " << std::string(78,'=') << std::endl ;
  stream << "# Arguments: " << std::endl ;
  stream << "# \tcode    = " << Gaudi::Utils::toString( code    )  << std::endl ;
  stream << "# \tactor   = " << Gaudi::Utils::toString( actor   )  << std::endl ;
  stream << "# \tmodules = " << Gaudi::Utils::toString( modules )  << std::endl ;
  stream << "# \tlines   = " << Gaudi::Utils::toString( lines   )  << std::endl ;
  stream << "# \tcontext = " << Gaudi::Utils::toString( context )  << std::endl ;
  stream << "# " << std::string(78,'=') << std::endl ;
  // define imported modules:
  stream << "##        MODULES :" << std::endl ;
  for ( Strings::const_iterator imodule = modules.begin() ; 
        modules.end() != imodule ; ++imodule ) 
  {
    stream << "from " << (*imodule) << " import *" << std::endl ;
  }
  stream << "## End of MODULES  " << std::endl ;
  stream << "## The ACTOR :"              << std::endl ;
  stream << "_actor=" << actor << std::endl ;
  // put additional lines:
  stream << "##        LINES :" << std::endl ;
  for ( Strings::const_iterator iline = lines.begin() ; 
        lines.end() != iline ; ++iline ) 
  { stream << (*iline) << std::endl ; }
  stream << "## end of LINES  " << std::endl ;
  // put the context 
  stream << "##        CONTEXT :" << std::endl ;
  if ( !context.empty() ) { stream << context << std::endl ; }
  stream << "## End of CONTEXT  " << std::endl ;
  stream << "##        CODE :"    << std::endl ;  
  stream << "_code="  << code     << std::endl ;
  stream << "## End of CODE :"    << std::endl ;  
  stream << "sc=_actor.process('" << name() << "',_code)" << std::endl ;
  stream << "# " << std::string(78,'=') << std::endl ;
  stream << "# The END " << std::endl ;
  stream << "# " << std::string(78,'=') << std::endl ; 
  //
  std::string result = stream.str() ;
  if ( msgLevel ( MSG::DEBUG ) || showCode() ) 
  {
    debug() << "Generated Python code:" << std::endl 
            << result 
            << endreq ;  
  }
  ///
  return result ;
}
// ============================================================================



// ============================================================================
// The END 
// ============================================================================
