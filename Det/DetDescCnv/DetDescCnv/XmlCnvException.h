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
#ifndef DETDESCCNV_XMLCNVSVC_XMLCNVEXCEPTION_H
#define DETDESCCNV_XMLCNVSVC_XMLCNVEXCEPTION_H 1
/// Include files
#include <string>
/// GaudiKernel
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/Kernel.h"

/// Error codes used for XmlCnvException handling
enum Status {
  CANT_QUERY_INTERFACE = 2,
  CANT_RETRIEVE_OBJECT,
  INVALID_CLASS_ID,
  CORRUPTED_DATA,
  STOP_REQUEST,
  EXPRESSION_ERROR,
  WRONG_DTD_VERSION,
  ERROR_ADDING_TO_TS,
  LAST
};

///
class XmlCnvException : public GaudiException {
public:
  ///
  inline XmlCnvException( const std::string& message, const StatusCode& sc = StatusCode::FAILURE );
  inline XmlCnvException( const std::string& message, const GaudiException& ge,
                          const StatusCode& sc = StatusCode::FAILURE );
  inline XmlCnvException( const std::string& message, const Status sc ) : XmlCnvException( message, StatusCode{sc} ) {}
  inline XmlCnvException( const XmlCnvException& );
  ///
  virtual inline ~XmlCnvException() throw();
  ///
  inline const char*       getMessage() const;
  inline const char*       getType() const;
  inline const StatusCode& status() const;
  ///
  inline GaudiException* clone() const override;
  ///
};
///
#include "DetDescCnv/XmlCnvException.icpp"
///

#endif // DETDESCCNV_XMLCNVSVC_XMLCNVEXCEPTION_H
