// ============================================================================
// Include files
// ============================================================================
// Python
// ============================================================================
#include "Python.h"
// ============================================================================
// STD & STL
// ============================================================================
#include <sstream>
#include <fstream>
#include <memory>
#include <set>
#include <map>
#include <numeric>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/System.h"
#include "GaudiKernel/Time.h"
#include "GaudiKernel/IAlgContextSvc.h"
// ============================================================================
// GaudiAlgs
// ============================================================================
#include "GaudiAlg/GetAlgs.h"
// ============================================================================
// DaVinciInterfaces
// ============================================================================
#include "Kernel/GetIDVAlgorithm.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/HybridBase.h"
#include "LoKi/ILoKiSvc.h"
#include "LoKi/CacheFactory.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/algorithm/string/erase.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
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
namespace
{
  struct DecRef { void operator()(PyObject* p) { Py_XDECREF(p); } };
  using PyObjPtr = std::unique_ptr<PyObject,DecRef>;
  // ==========================================================================
  /// replace all new lines with "newline+comments"
  std::string addComment ( std::string str )
  {
    static const std::string comm = "\n##! " ;
    std::string::size_type pos = str.find ( '\n' ) ;
    while ( std::string::npos != pos )
    {
      str.replace ( pos , 1 , comm );
      pos = str.find ( '\n' , pos + 2 ) ;
    }
    return str ;
  }
  // ==========================================================================
  /// prepare the actual code: trim and remove the paired quotes
  std::string trimCode ( std::string code )
  {
    // trim leading and trailing blanks:
    boost::trim ( code ) ;
    if ( code.empty() )                { return code ; }
    // check for paired quotes:
    auto ifront = code.begin () + 1 ;
    auto iback  = code.end   () - 1 ;
    //
    if ( *ifront != *iback ) { return code ; }
    //
    if ( '\'' != *ifront && '"' != *ifront ) { return code ; }
    //
    return trimCode ( std::string ( ifront , iback ) ) ;
  }
  // ==========================================================================
  /// Helper class to protect calls to Python code with the GIL.
  struct PyGILGuard 
  {
    PyGILGuard  ():  gstate( PyGILState_Ensure() ) {}
    ~PyGILGuard () { PyGILState_Release(gstate); }
    PyGILState_STATE gstate;
  };
  // ==========================================================================
}
// ============================================================================
// Standard constructor
// ============================================================================
LoKi::Hybrid::Base::Base
( const std::string& type   ,
  const std::string& name   ,
  const IInterface*  parent )
  : GaudiTool ( type , name , parent )
    //
  , m_cpplines   ( { "#include \"GaudiKernel/Kernel.h\""            ,
                     "#include \"GaudiKernel/System.h\""            ,
                     "#include \"GaudiKernel/SystemOfUnits.h\""     ,
                     "#include \"GaudiKernel/PhysicalConstants.h\"" ,
                     "#include \"LoKi/Functors.h\""                 ,
                     "#include \"LoKi/Context.h\""                  ,
                     "#include \"LoKi/CacheFactory.h\""             ,
                     "#include \"LoKi/FunctorCache.h\""             , } )
{
  declareProperty ( "ShowCode" ,
                    m_showCode = false,
                    "Flag to display the prepared python code") ;
  //
  // use Environment variable
  //
  declareProperty ( "MakeCpp"  ,
                    m_makeCpp    = "UNKNOWN" != System::getEnv("LOKI_GENERATE_CPPCODE"),
                    "Generate C++ code for created functors ") ;
  //
  declareProperty ( "UsePython"  ,
                    m_use_python = "UNKNOWN" == System::getEnv("LOKI_DISABLE_PYTHON"),
                    "Use Python as factory for LoKi-functors ") ;
  //
  declareProperty ( "UseCache"  ,
                    m_use_cache  = "UNKNOWN" == System::getEnv("LOKI_DISABLE_CACHE" ),
                    "Use C++ cache for LoKi-functors ") ;
  //
  // make reasonable default name
  //
  m_cppname = this->name() ;
  if ( boost::algorithm::starts_with(  m_cppname,  "ToolSvc." ) ) { m_cppname.erase ( 0 , 8 ) ; }
  std::replace_if( m_cppname.begin(), m_cppname.end(),
                   [](const char& c) { return c=='.' || c== ' '; }, '_');
  boost::algorithm::replace_all( m_cppname, "::","__");
  m_cppname.insert ( 0 , "FUNCTORS_" ) ;
  //
  declareProperty ( "CppFileName",
                    m_cppname    ,
                    "File name for C++ code for created functors ") ;
  //
  declareProperty ( "CppLines" ,
                    m_cpplines ,
                    "C++ (header) lines to be included") ;
}
// ============================================================================
// initialization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::Base::initialize ()
{
  const StatusCode sc = GaudiTool::initialize() ;
  if ( sc.isFailure() ) { return sc ; }
  // force the loading/initialization of  LoKi Service
  svc<LoKi::ILoKiSvc>( "LoKiSvc" , true ) ;
  // Messages
  if ( !m_use_python ) Print( "Python Functors are DISABLED"    , sc , MSG::ALWAYS ).ignore() ;
  if ( !m_use_cache  ) Print( "C++ Cache Functors are DISABLED" , sc , MSG::ALWAYS ).ignore() ;
  // return
  // m_showCode = true ;
  return ( m_use_python || m_use_cache ? sc : Error( "No Functors enabled" ) );
}
// ============================================================================
// finalization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::Base::finalize  ()
{
  //
  // finalize python (if the owner)
  if ( Py_IsInitialized() && m_pyInit )
  {
    info() << "Finalization   of Python is triggered" << endmsg ;
    std::string line ;
    line += "                                                           # " + name() + "\n" ;
    line += "import atexit                                              # " + name() + "\n" ;
    line += "if atexit._exithandlers :                                  # " + name() + "\n" ;
    line += "\tprint 'ATTENTION: Clear non-empty atexit._exithandlers'  # " + name() + "\n" ;
    line += "\tprint 'len(_exithandlers)=',len(atexit._exithandlers)    # " + name() + "\n" ;
    line += "\tatexit._exithandlers=[]                                  # " + name() + "\n" ;
    line += "                                                           # " + name() + "\n" ;
    {
      PyGILGuard guard{};
      const int result =  PyRun_SimpleString ( line.c_str() ) ;
      if ( 0 != result )
      {
        Warning ( "Error code from PyRun_SimpleString" , StatusCode{static_cast<unsigned long>( 1000 + result )} ).ignore() ;
        warning() << "The problematic code is \n" << line << endmsg ;
        if ( PyErr_Occurred() ) { PyErr_Print() ; }
      }
    }
    Py_Finalize () ;
  }
  //
  if ( Py_IsInitialized() && m_pyInit ) { Warning ( "Python is still initialized!" ) ; }
  //
  // Write C++ code
  //
  if ( m_makeCpp ) { writeCpp () ; }
  //
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
namespace
{
  // ==========================================================================
  std::string toString ( PyObject& o )
  {
    PyGILGuard guard{};
    PyObjPtr str{ PyObject_Str ( &o ) };
    return str ? std::string{PyString_AS_STRING(str.get())} : std::string{};
  }
  // ==========================================================================
}
// ============================================================================
StatusCode LoKi::Hybrid::Base::executeCode ( const std::string& pycode ) const
{
  // Initialize python if it not yet done
  if ( !Py_IsInitialized() )
  {
    info() << "Initialization of Python is triggered" << endmsg ;
    Py_Initialize () ;
    m_pyInit = true  ;
    ++counter ("Python is initialized!") ;
  }
  // Check the proper python environment:
  if ( !Py_IsInitialized() ) { return Error("Python is not initialized yet!") ; }

  // play with raw Python, ensuring we have the GIL.
  PyGILGuard guard{};

  // local scope
  PyObject* locals  = PyEval_GetLocals  () ;
  if ( !locals )
  {
    debug() << "PyEval_GetLocals:  'locals'  points to NULL" << endmsg ;
    if ( PyErr_Occurred() ) { PyErr_Print() ; }
  }
  // global scope
  PyObject* globals = PyEval_GetGlobals () ;
  bool globnew = false ;
  if ( !globals )
  {
    debug() << "PyEval_GetGlobals: 'globals' points to NULL" << endmsg ;
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

  bool ok = true ;
  if ( !result )
  {
    ok = false ;

    PyObject* o1 = nullptr ;
    PyObject* o2 = nullptr ;
    PyObject* o3 = nullptr ;
    PyErr_Fetch              ( &o1 , &o2 , &o3 ) ;
    PyErr_NormalizeException ( &o1 , &o2 , &o3 ) ;

    if ( o1 ) { error () << " PyError Type      : " << toString(*o1) << endmsg; }
    if ( o2 ) { error () << " PyError Value     : " << toString(*o2) << endmsg; }
    if ( o3 ) { error () << " PyError Traceback : " << toString(*o3) << endmsg; }

    PyObject* pyErr = o2 ;

    // SyntaxError
    if ( PyErr_GivenExceptionMatches ( o2 , PyExc_SyntaxError    ) ||
         PyErr_GivenExceptionMatches ( o2 , PyExc_TypeError      ) ||
         PyErr_GivenExceptionMatches ( o2 , PyExc_NameError      ) ||
         PyErr_GivenExceptionMatches ( o2 , PyExc_IndexError     ) ||
         PyErr_GivenExceptionMatches ( o2 , PyExc_ImportError    ) ||
         PyErr_GivenExceptionMatches ( o2 , PyExc_AttributeError )  )
    {
      //
      PyObjPtr filename { PyObject_GetAttrString  ( pyErr , "filename"            ) };
      PyObjPtr lineno   { PyObject_GetAttrString  ( pyErr , "lineno"              ) };
      PyObjPtr offset   { PyObject_GetAttrString  ( pyErr , "offset"              ) };
      PyObjPtr text     { PyObject_GetAttrString  ( pyErr , "text"                ) };
      PyObjPtr prntfal  { PyObject_GetAttrString  ( pyErr , "print_file_and_line" ) };
      PyObjPtr msg      { PyObject_GetAttrString  ( pyErr , "msg"                 ) };
      PyObjPtr message  { PyObject_GetAttrString  ( pyErr , "message"             ) };
      //
      long        _offset = -1 ;
      long        _lineno = -1 ;
      std::string _text        ;
      std::string _msg         ;
      std::string _message     ;
      
      if ( filename ) { info () << "Filename: " << toString ( *filename ) << endmsg ; }
      if ( lineno   ) 
      {
        info () << "Lineno  : " << toString ( *lineno ) << endmsg ;
        if ( PyInt_Check ( lineno.get() ) ) { _lineno = PyInt_AsLong ( lineno.get() ) ;  }
      }
      if ( offset   ) 
      {
        info () << "offset  : " << toString ( *offset ) << endmsg ;
        if ( PyInt_Check ( offset.get() ) ) { _offset = PyInt_AsLong ( offset.get() ) ;  }
      }
      if ( text     ) { _text    = toString ( *text    ) ; info () << "text    : " << _text    << endmsg ; }
      if ( msg      ) { _msg     = toString ( *msg     ) ; info () << "msg     : " << _msg     << endmsg ; }
      if ( message  ) { _message = toString ( *message ) ; info () << "message : " << _message << endmsg ; }
      if ( prntfal  ) { info () << "prntfal : " << toString( *prntfal )  << endmsg ; }
      {
        MsgStream& stream = error ()  ;
        stream
          << " Python error in Code\n" ;
        if ( pyErr  )
        { stream << "Python error  : " << toString ( *pyErr ) << '\n' ; }
        if ( !_msg.empty()     )
        { stream << "PyErr msg     : " << _msg               << '\n' ; }
        if ( !_message.empty() )
        { stream << "PyErr message : " << _message           << '\n' ; }
        if ( 0 <= _lineno   ) { stream << " Line #" << _lineno << '\n'; }
        if ( !_text.empty() ) { stream << "       " << _text          ; }
        if ( 0 < _offset )
        { stream  << "      " << std::string(_offset,' ') << '^'      ; }
        stream<< endmsg ;
      }
      // restore for printout
      PyErr_Restore ( o1 , o2 , o3 ) ;
    } 
    else 
    {
      // restore for printout
      PyErr_Restore ( o1 , o2 , o3 ) ;
    }
    //
    error ()  << "Native Python printout to stderr"        << endmsg ;
    PyErr_Print   () ;
    error ()  << "End of native Python printout to stderr" << endmsg ;
  }

  if ( globals && globnew )      { Py_XDECREF( globals ) ; }

  if ( result )                  { Py_XDECREF ( result )      ; }
  else if ( PyErr_Occurred()   ) { PyErr_Print() ; ok = false ; }
  else                           {                 ok = false ; }

  if ( !ok )
  {
    err () << " Error has occured in Python: the problematic code is : "
           << endmsg
           << pycode
           << endmsg ;
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
  //
  std::string _code = code ;
  boost::algorithm::replace_all ( _code , "\n"  , " " ) ;
  boost::algorithm::replace_all ( _code , "\\n" , " " ) ;
  boost::algorithm::trim        ( _code ) ;
  {
    std::string::size_type pos = _code.find ("&  ") ;
    while ( std::string::npos != pos ) 
    { _code.replace( pos , 3 , "& " ) ; pos = _code.find ("&  " , pos + 1 ) ; }
  }
  {
    std::string::size_type pos = _code.find ("|  ") ;
    while ( std::string::npos != pos ) 
    { _code.replace( pos , 3 , "| " ) ; pos = _code.find ("|  " , pos + 1 ) ; }
  }
  {
    std::string::size_type pos = _code.find ("  &") ;
    while ( std::string::npos != pos ) 
    { _code.replace( pos , 3 , " &" ) ; pos = _code.find ("  &" , pos + 1 ) ; }
  }
  {
    std::string::size_type pos = _code.find ("  |") ;
    while ( std::string::npos != pos ) 
    { _code.replace( pos , 3 , " |" ) ; pos = _code.find ("  |" , pos + 1 ) ; }
  }
  //
  // trim and remove the paired quotes:
  _code = trimCode ( _code ) ;
  //
  std::ostringstream stream ;
  // start the code:
  stream   << "# "             << std::string(78,'=')                                     << '\n'
           << "# python pseudomodule, generated for the hybrid factory'" << name() << "'" << '\n' 
           << "# \tat "        << Gaudi::Time::current().format(true)                     << '\n' 
           << "# \tby "        << System::accountName()                                   << '\n'
           << "# "             << std::string(78,'=')                                     << '\n' ;
  // 
  stream   << "# Arguments:\n" 
           << "# \tcode    = " << Gaudi::Utils::toString ( _code   )  << '\n' 
           << "# \tactor   = " << Gaudi::Utils::toString ( actor   )  << '\n' 
           << "# \tmodules = " << Gaudi::Utils::toString ( modules )  << '\n' 
           << "# \tlines   = " << Gaudi::Utils::toString ( lines   )  << '\n' 
           << "# \tcontext = " << Gaudi::Utils::toString ( addComment ( "\n" + context ) )  << '\n' 
           << "# "             << std::string(78,'=')                 << '\n' ;
  //
  // define imported modules:
  //
  stream   << "##        MODULES :\n" ;
  for ( const auto& module : modules ) { stream << "from " << module << " import *\n" ; }
  stream   << "## End of MODULES\n" ;
  //
  stream   << "## The ACTOR :\n" ;
  stream   << "_actor   = " << actor << '\n' ;
  const std::string ntab ( 8 , ' ' ) ;
  stream   << "_context = _actor.context  () if hasattr ( _actor , 'context' ) else None\n" 
           << "from LoKiCore.decorators import HybridContext, hybrid_context_deco       \n" 
           << "_algo    = _context.  algo () if _context else None                      \n" 
           << "_dvalgo  = _context.dvalgo () if _context else None                      \n"
           << "with HybridContext ( algo = _algo , dvalgo = _dvalgo ) as context :      \n" 
           << ntab << "hybrid_context_deco ( locals().copy() , context )                \n" ;
  /// insert additional lines:
  if ( !lines.empty() ) 
  {
    stream << ntab << "##        LINES :\n" ;
    for ( const auto& l : lines ) 
    { 
      std::vector<std::string> _lines ;
      boost::algorithm::split ( _lines , l , boost::is_any_of("\n") ) ;
      for (  const  auto& ll : _lines ) 
      { 
        std::string lll = ll ;
        boost::algorithm::replace_all ( lll , "\\t" , "\t" ) ;
        boost::algorithm::replace_all ( lll , "\t"  , ntab ) ;
        stream << ntab << lll << '\n' ; 
      } 
    }
    stream << ntab << "hybrid_context_deco ( locals().copy() , context ) \n" 
           << ntab << "## end of LINES  " << '\n' ;
  }
  /// insert preambulo
  if ( !context.empty() ) 
  {
    std::string _context      = "\n" + context ;
    boost::algorithm::replace_all ( _context , "\\n"    , "\n" ) ;
    boost::algorithm::replace_all ( _context , "\\t"    , "\t" ) ;
    boost::algorithm::replace_all ( _context , "\t"     , ntab ) ;
    //
    std::string::size_type pos = _context.find('\n') ;
    while ( std::string::npos != pos ) 
    { _context.insert ( pos + 1 , ntab ) ; pos = _context.find ( '\n' , pos + 1 ) ; }
    //
    // boost::algorithm::replace_all ( _context , "\n"     , "\n"+ntab ) ;
    std::vector<std::string> context_ ;
    boost::algorithm::split       ( context_ , _context , boost::is_any_of("\n") ) ;
    stream << ntab << "##        PREAMBULO :\n" ;
    for (  const  auto& ll : context_ ) { stream << ll << '\n' ; }
    stream << ntab << "## End of PREAMBULO  \n" 
           << ntab << "hybrid_context_deco ( locals().copy() , context ) \n" ;
  }
  /// and finally the code 
  stream   << ntab << "##        CODE :         \n" 
           << ntab << "_code= " << _code    << '\n' 
           << ntab << "## End of CODE           \n" ;
  /// the most important line: send the created python object to C++
  stream   << ntab << "sc = _actor.process( '" << name () << "' , _code ) \n" ;
  //
  stream   << "#"  << std::string ( 78 , '=' ) <<        '\n' 
           << "##" << std::string ( 70 , ' ' ) << "The END\n" 
           << "#"  << std::string ( 78 , '=' ) <<        '\n' ;
  //
  std::string result = stream.str() ;
  if ( msgLevel ( MSG::DEBUG ) || showCode() )
  {
    info() << "Generated Python code:\n"
           << result
           << endmsg ;
  }
  ///
  return result ;
}
// ============================================================================
namespace
{
  // ==========================================================================
  typedef std::pair<std::string,std::string>  _PAIR       ;
  typedef std::map<std::string,_PAIR>         _FUNCTIONS  ;
  typedef std::map<std::string,_FUNCTIONS>    _ALLFUNCS   ;
  // ==========================================================================
  const std::vector<std::string>  s_emptylines ;
  const _ALLFUNCS                 s_emptyfuncs ;
  // ==========================================================================
  std::unique_ptr<std::ostream>
  openFile ( std::string                     namebase                 ,
             const unsigned short            findex                   ,
             const std::vector<std::string>& lines     = s_emptylines ,
             const _ALLFUNCS&                allfuncs  = s_emptyfuncs )
  {
    //
    // construct the file name
    //  1) remove trailing .cpp
    if ( boost::algorithm::ends_with( namebase, ".cpp" ) ) boost::algorithm::erase_tail( namebase, 4 );
    //  2) replace blanks  by underscore
    std::replace( namebase.begin(), namebase.end(), ' ','_' );
    //  3) construct the name
    boost::format fname ( "%s_%04d.cpp" ) ;
    fname % namebase % findex ;
    //
    auto file = std::make_unique<std::ofstream>( fname.str() );
    //
    *file << "/** The file is generated on : " << System::hostName()   << '\n'
          << " *  at : "        << Gaudi::Time::current().format(true) << '\n'
          << " *  by : "        << System::accountName()               << '\n'
          << " */\n" ;
    //
    // write the include directives
    *file << "\n// Explicitly declared include files:\n" ;
    for ( const auto& l : lines ) { *file << l << '\n' ; }
    *file << '\n' ;
    //
    std::set<std::string> morelines ;
    for ( const auto& ifunc : allfuncs ) {
      for ( auto& icode : ifunc.second ) {
        for ( const auto& s : { 
            std::make_pair("LoKiPhys",          "#include \"LoKi/LoKiPhys.h\""  ) ,
              std::make_pair("LoKiTrack",         "#include \"LoKi/LoKiTrack.h\"" ) ,
              std::make_pair("LoKiArrayFunctors", "#include \"LoKi/LoKiArrayFunctors.h\""),
              std::make_pair("LoKiProtoParticles","#include \"LoKi/LoKiProtoParticles.h\""),
              std::make_pair("LoKiHlt",           "#include \"LoKi/LoKiHlt.h\""),
              std::make_pair("LoKiNumbers",       "#include \"LoKi/LoKiNumbers.h\""),
              std::make_pair("LoKiTrigger",       "#include \"LoKi/LoKiTrigger.h\""),
              std::make_pair("LoKiCore",          "#include \"LoKi/LoKiCore.h\""),
              std::make_pair(".algorithms",       "#include \"LoKi/AlgFunctors.h\""),
              std::make_pair("ALG_",              "#include \"LoKi/AlgFunctors.h\""),
              std::make_pair("TC_",               "#include \"LoKi/LoKiTrigger.h\""),
              std::make_pair("TS_",               "#include \"LoKi/LoKiTrigger.h\""),
              std::make_pair("ODIN_",             "#include \"LoKi/LoKiHlt.h\""),
              std::make_pair("L0_",               "#include \"LoKi/LoKiHlt.h\""),
              std::make_pair("HDR_",              "#include \"LoKi/LoKiHlt.h\""),
              std::make_pair("HLT_",              "#include \"LoKi/LoKiHlt.h\"") } ) {
          if ( std::string::npos != icode.first.find ( s.first ) )
          { morelines.insert ( s.second ) ; }
        }
      }
    }
    // additional include files
    *file << "\n// Additional include files: " << '\n' ;
    for ( const auto& l : morelines ) { *file << l << '\n' ; }
    *file << '\n' ;
    //
    return std::move(file) ; // must std::move to convert from std::fstream to std::ostream...
  }
  // ==========================================================================
}
// ============================================================================
void LoKi::Hybrid::Base::writeCpp () const
{
  //
  std::unique_ptr<std::ostream> file ;
  //
  unsigned short ifile  = 0 ;
  unsigned int   iwrite = 0 ;
  //
  int split = 0 ;
  //
  const std::string split_ = System::getEnv( "LOKI_GENERATE_CPPCODE" ) ;
  //
  // Positive: write N-files
  // Negative: write N-functors per file
  // Zero    : write one file
  //
  if ( !boost::conversion::try_lexical_convert ( split_, split ) ) { split = 0 ; }
  //
  //
  // number of files (if defined)
  const unsigned int n_files = 0 < split ? (unsigned int) split : 0 ;
  //
  // total number of functors
  auto ftotal = std::accumulate
    ( m_allfuncs.begin(), m_allfuncs.end(), 0,
      [](int i, const std::pair<std::string,FUNCTIONS>& fs)
      { return i+fs.second.size(); } );
  //
  // number of functors per file
  const unsigned int i_split =
    0 > split ?  std::abs ( split ) :
    0 < split ?  int ( double ( ftotal ) / split ) + 1 : 0 ;
  //
  for ( const auto& ia : m_allfuncs ) {
    //
    const std::string& cpptype = ia.first  ;
    for ( const auto& ic : ia.second ) {
      //
      const std::string& cppcode = ic.second.first  ;
      const std::string& pytype  = ic.second.second ;
      const std::string& pycode  = ic.first         ;
      //
      if ( !file ) file = openFile( m_cppname , ++ifile , m_cpplines , m_allfuncs );
      //
      *file << "\n// FUNCTOR #"
            << ++iwrite     << "/"
            << ia.second.size() << "" << '\n' ;
      //
      // write actual C++ code
      LoKi::Cache::makeCode ( *file      ,
                              cpptype    , cppcode    ,
                              pycode     , pytype     ) ;
      *file << '\n' ;
      //
      if ( 1 <= i_split && 0 == iwrite % i_split ) { file.reset() ; }
      //
    }
    //
  }
  // close the file
  file.reset() ;
  //
  if ( 0 < n_files ) {
    while ( ifile < n_files ) {
      file = openFile ( m_cppname , ++ifile ) ;
      file.reset() ;
    }
  }
  //
}
// ============================================================================
// build the universal context 
LoKi::Context LoKi::Hybrid::Base::make_context () const 
{
  const IAlgContextSvc* cntx = svc<IAlgContextSvc> ( "AlgContextSvc", true ) ;
  if ( nullptr == cntx ) 
  {
    Error ( "Can't get the context service!" ).ignore() ;
    return LoKi::Context() ;
  }
  //
  return LoKi::Context ( Gaudi::Utils::getGaudiAlg     ( cntx ) ,
                         Gaudi::Utils::getIDVAlgorithm ( cntx ) )  ;
}
// ============================================================================
  
// ============================================================================
// The END
// ============================================================================
