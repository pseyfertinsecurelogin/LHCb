/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef VELOCLUSTERWORD_H
#define VELOCLUSTERWORD_H 1

#include "SiDAQ/SiClusterWord.h"
#include "VeloBitsPolicy.h"

/** Decodes/encodes velo clusterword
 *
 *  This class is a simple type wrapper for an
 *  SiClusterWord instantiated with a VeloBitsPoliciy.
 *  
 * @see SiClusterWord
 * @see VeloBitsPolicy
 *
 * @author Kurt Rinnert
 * @date   2006-02-02
 */
typedef SiClusterWord<VeloBitsPolicy> VeloClusterWord;

#endif

