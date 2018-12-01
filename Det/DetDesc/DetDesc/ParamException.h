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
#ifndef DETDESC_PARAMEXCEPTION_H
#define DETDESC_PARAMEXCEPTION_H 1

#include <typeinfo>

// Base class
#include "GaudiKernel/GaudiException.h"

///---------------------------------------------------------------------------
/** @class ParamException ParamException.h DetDesc/ParamException.h

    Exception used in the ParamSet class.

    @author Sebastien Ponce
    @author Andrea Valassi
    @author Marco Clemencic
*///--------------------------------------------------------------------------

class ParamException : public GaudiException {

public:

  /// Type of problem encountered in ParamValidDataObject.
  enum ExceptionType {
    BAD_KEY,
    BAD_TYPE
  };

  /// Constructor
  ParamException( const std::string& name , ExceptionType t=BAD_KEY );

  /// Constructor for a BAD_TYPE exception giving informations about the types.
  ParamException( const std::string& name , const std::type_info &req, const std::type_info &actual );

  /// Returns the type of ParamException.
  inline ExceptionType type() const { return m_type; }

private:

  /// store the type
  ExceptionType m_type;

};

#endif  // DETDESC_PARAMEXCEPTION_H
