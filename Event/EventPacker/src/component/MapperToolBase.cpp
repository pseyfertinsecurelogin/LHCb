// Include files

// local
#include "MapperToolBase.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MapperToolBase
//
// 2012-03-26 : Chris Jones
//-----------------------------------------------------------------------------

//============================================================================

StatusCode MapperToolBase::finalize()
{
  m_jos.reset();
  return GaudiTool::finalize();
}

//=============================================================================

SmartIF<IJobOptionsSvc>& MapperToolBase::joSvc() const
{
  if ( UNLIKELY(!m_jos) ) { m_jos = service("JobOptionsSvc"); }
  return m_jos;
}

// ============================================================================

std::string MapperToolBase::streamName( const std::string & path ) const
{
  auto tmp = ( path.compare(0,7,"/Event/") ==  0 ? path.substr(7) : path );
  return tmp.substr(0,tmp.find_first_of( "/" ));
}

//=============================================================================
