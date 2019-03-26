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
#ifndef DETDESC_MATERIALEXCEPTION_H
#define DETDESC_MATERIALEXCEPTION_H
/// GaudiKernel
#include "GaudiKernel/GaudiException.h"
//// STL
#include <exception>
#include <iostream>
//
class Material;
class MSgStream;
///

///
class MaterialException : public GaudiException {
  ///
public:
  ///
  MaterialException( const std::string& message, const Material* mat = nullptr );
  ///
  MaterialException( const std::string& message, const GaudiException& Exception, const Material* mat = nullptr );
  ///
  std::ostream& printOut( std::ostream& os = std::cerr ) const override;
  MsgStream&    printOut( MsgStream& os ) const override;
  ///
  inline GaudiException* clone() const override { return new MaterialException( *this ); }
  ///
private:
  ///
  const Material* m_me_mat;
  ///
};
///

#endif // DETDESC_MATERIALEXCEPTION_H
