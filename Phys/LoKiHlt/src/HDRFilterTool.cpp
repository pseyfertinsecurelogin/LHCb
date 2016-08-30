// Class Header
#include "LoKi/HDRFilterTool.h"

/** @file HDRFilterTool.cpp
 *
 *    @author: Konstantin Gizdov
 *    @date:   26 August 2016
 */

LoKi::HDRFilterTool::HDRFilterTool
( const std::string& type   ,   // tool type (?)
  const std::string& name   ,   // toolinstance name
  const IInterface*  parent )   // tool's parent
  : LoKi::FilterTool(type, name, parent),
  // the functor itself
  m_cut(LoKi::BasicFunctors<const LHCb::HltDecReports*>::BooleanConstant(false))
{
  StatusCode sc = setProperty("Code", "HLT_NONE");
  Assert(sc.isSuccess(), "Unable (re)set property 'Code'", sc);
  sc = setProperty("Factory",
                   0 == name.find("Hlt1") ?
                   "LoKi::Hybrid::HltFactory/Hlt1HltFactory:PUBLIC" :
                    (0 == name.find("Hlt2") ?
                     "LoKi::Hybrid::HltFactory/Hlt2HltFactory:PUBLIC" :
                     "LoKi::Hybrid::HltFactory/HltFactory:PUBLIC"));
  Assert(sc.isSuccess(), "Unable (re)set property 'Factory'", sc);
}

StatusCode LoKi::HDRFilterTool::decode() {
      StatusCode sc = i_decode<LoKi::Hybrid::IHltFactory>(m_cut);
      Assert(sc.isSuccess(), "Unable to decode the functor!");
      return StatusCode::SUCCESS;
}

// the main method: pass
bool LoKi::HDRFilterTool::pass(const LHCb::HltDecReports * hdr) const {
  // use the functor
  return m_cut(hdr);
}

/// the factory (needed for instantiation)
DECLARE_NAMESPACE_TOOL_FACTORY(LoKi, HDRFilterTool)
