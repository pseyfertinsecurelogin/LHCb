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
#ifndef ANNSVC_H
#define ANNSVC_H 1

// Include files
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Service.h"
#include "Kernel/IANNSvc.h"
#include <string>

namespace ANNSvcUtilities {
  template <typename KEY, typename VALUE>
  class bimap_t;
}
/** @class ANNSvc ANNSvc.h
 *
 *  functionality:
 *     maps between 'Assigned Names and Numbers'
 *         and also 'Assigned Numbers and Names'
 *
 *  @author Gerhard Raven
 *  @date   2008-01-02
 */

class ANNSvc : public extends1<Service, IANNSvc> {
public:
  ANNSvc( const std::string& name, ISvcLocator* pSvcLocator, const std::vector<major_key_type>& majors );

  StatusCode finalize() override;

  std::optional<minor_value_type> value( const major_key_type& major, const std::string& minor ) const override;
  std::optional<minor_value_type> value( const major_key_type& major, int minor ) const override;

  bool hasMajor( const major_key_type& major ) const override;

  std::vector<minor_key_type>   keys( const major_key_type& major ) const override;
  std::vector<minor_value_type> items( const major_key_type& major ) const override;
  GaudiUtils::VectorMap<minor_value_type::first_type, minor_value_type::second_type>
                              item_map( const major_key_type& major ) const override;
  std::vector<major_key_type> majors() const override;

protected:
  // Call handleUndefined if an unknown key/value is requested...
  //  default implementation just returns an unitialized std::optional
  virtual std::optional<minor_value_type> handleUndefined( const major_key_type& major, int minor ) const;
  virtual std::optional<minor_value_type> handleUndefined( const major_key_type& major,
                                                           const std::string&    minor ) const;

private:
  typedef ANNSvcUtilities::bimap_t<minor_key_type, minor_mapped_type> bimap_type;
  typedef GaudiUtils::VectorMap<major_key_type, bimap_type*>          maps_type;
  maps_type                                                           m_maps;
  friend class ANNSvcUtilities::bimap_t<minor_key_type, minor_mapped_type>;
};

#endif // ANNSVC_H 1
