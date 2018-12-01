/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "Event/HltDecReports.h"

#include "LoKi/FilterTool.h"
#include "LoKi/HLTTypes.h"
#include "LoKi/IHDRFilterTool.h"
#include "LoKi/IHltFactory.h"

namespace LoKi
{
  /** @class HDRFilterTool
   *  Simple filtering tool based on LoKi/Bender "hybrid" approach
   *  for filtering according to LHCb::HltDecReports.
   *  @author: Konstantin Gizdov, Rosen Matev
   *  @date: 2017-03-24
   */

  class HDRFilterTool : public extends<LoKi::FilterTool, LoKi::IHDRFilterTool> {
  public:

   /// standard constructor
   HDRFilterTool(const std::string& type,   // tool type (?)
                 const std::string& name,   // toolinstance name
                 const IInterface* parent); // tool's parent

   /// the default constructor is disabled
   HDRFilterTool() = delete;
   /// the copy and move constructos are disabled
   HDRFilterTool(const HDRFilterTool &) = delete;
   HDRFilterTool(HDRFilterTool &&) = delete;
   /// the assignement operators are disabled
   HDRFilterTool& operator=(const HDRFilterTool &) = delete;
   HDRFilterTool& operator=(HDRFilterTool &&) = delete;

   bool predicate(const LHCb::HltDecReports& hdr) const override;

   /** Decode the functor (use the factory)
    *  @see LoKi::FilterTool
    *  @see LoKi::FilterTool::decode
    *  @see LoKi::FilterTool::i_decode
    */
   StatusCode decode() override;

  private:

   /// the functor itself
   LoKi::Types::HLT_Cut m_cut{LoKi::BasicFunctors<const LHCb::HltDecReports*>::BooleanConstant(false)};

  };
}  // namespace LoKi

/// the factory (needed for instantiation)
DECLARE_COMPONENT( LoKi::HDRFilterTool )

LoKi::HDRFilterTool::HDRFilterTool
( const std::string& type   ,   // tool type (?)
  const std::string& name   ,   // toolinstance name
  const IInterface*  parent )   // tool's parent
  : base_class(type, name, parent)
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

bool LoKi::HDRFilterTool::predicate(const LHCb::HltDecReports& hdr) const {
  return m_cut(&hdr);
}
