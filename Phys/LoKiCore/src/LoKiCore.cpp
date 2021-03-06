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
// ============================================================================
// Include files
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/LoKiCore.h"
// ============================================================================
#include "LoKi/AddRef.h"
#include "LoKi/AlgCuts.h"
#include "LoKi/AlgFunctors.h"
#include "LoKi/AlgUtils.h"
#include "LoKi/Algs.h"
#include "LoKi/Assert.h"
#include "LoKi/AuxFunBase.h"
#include "LoKi/Base.h"
#include "LoKi/BasicFunctors.h"
#include "LoKi/BiFunctions.h"
#include "LoKi/Binders.h"
#include "LoKi/CacheFactory.h"
#include "LoKi/Calls.h"
#include "LoKi/Cast.h"
#include "LoKi/Colors.h"
#include "LoKi/Combiner.h"
#include "LoKi/Const.h"
#include "LoKi/ConstIterator.h"
#include "LoKi/ConstReference.h"
#include "LoKi/ConstView.h"
#include "LoKi/Constants.h"
#include "LoKi/CoreCuts.h"
#include "LoKi/CoreEngine.h"
#include "LoKi/CoreEngineActor.h"
#include "LoKi/CoreLock.h"
#include "LoKi/CoreTypes.h"
#include "LoKi/DecayBase.h"
#include "LoKi/DecayChainBase.h"
#include "LoKi/DecayDescriptor.h"
#include "LoKi/DecayFinder.h"
#include "LoKi/Dicts.h"
#include "LoKi/Dump.h"
#include "LoKi/Dumper.h"
#include "LoKi/ErrorReport.h"
#include "LoKi/Exception.h"
#include "LoKi/ExtraInfo.h"
#include "LoKi/Field.h"
#include "LoKi/FilterAlg.h"
#include "LoKi/FilterOps.h"
#include "LoKi/FilterTool.h"
#include "LoKi/Filters.h"
#include "LoKi/FinderDict.h"
#include "LoKi/FirstN.h"
#include "LoKi/FuncAdapters.h"
#include "LoKi/FuncCast.h"
#include "LoKi/FuncOps.h"
#include "LoKi/Funcs.h"
#include "LoKi/Functions.h"
#include "LoKi/Functor.h"
#include "LoKi/Functors.h"
#include "LoKi/Geometry.h"
#include "LoKi/GetN.h"
#include "LoKi/GetType.h"
#include "LoKi/Helpers.h"
#include "LoKi/HistoBook.h"
#include "LoKi/Holder.h"
#include "LoKi/HybridBase.h"
#include "LoKi/ICoreAntiFactory.h"
#include "LoKi/ICoreFactory.h"
#include "LoKi/IDecayNode.h"
#include "LoKi/ILoKiSvc.h"
#include "LoKi/IReporter.h"
#include "LoKi/Info.h"
#include "LoKi/Interface.h"
#include "LoKi/Keeper.h"
#include "LoKi/KeeperBase.h"
#include "LoKi/KinTypes.h"
#include "LoKi/Kinematics.h"
#include "LoKi/Listener.h"
#include "LoKi/LoKiCore.h"
#include "LoKi/Math.h"
#include "LoKi/MissingParticle.h"
#include "LoKi/Monitor.h"
#include "LoKi/Monitoring.h"
#include "LoKi/MoreFunctions.h"
#include "LoKi/NamedRange.h"
#include "LoKi/NodeGrammar.h"
#include "LoKi/NodeParser.h"
#include "LoKi/Objects.h"
#include "LoKi/Operations.h"
#include "LoKi/Operators.h"
#include "LoKi/PIDOps.h"
#include "LoKi/Param.h"
#include "LoKi/ParamFunctors.h"
#include "LoKi/ParserFactory.h"
#include "LoKi/ParticleProperties.h"
#include "LoKi/PidFunctions.h"
#include "LoKi/Power.h"
#include "LoKi/Primitives.h"
#include "LoKi/Print.h"
#include "LoKi/Random.h"
#include "LoKi/Range.h"
#include "LoKi/RangeList.h"
#include "LoKi/RecStat.h"
#include "LoKi/Record.h"
#include "LoKi/Reference.h"
#include "LoKi/Release.h"
#include "LoKi/Report.h"
#include "LoKi/Scalers.h"
#include "LoKi/Selected.h"
#include "LoKi/Separators.h"
#include "LoKi/Services.h"
#include "LoKi/Sources.h"
#include "LoKi/Stat.h"
#include "LoKi/Status.h"
#include "LoKi/Streamers.h"
#include "LoKi/TES.h"
#include "LoKi/Tensors.h"
#include "LoKi/Timers.h"
#include "LoKi/ToCpp.h"
#include "LoKi/Tokens.h"
#include "LoKi/TreeGrammar.h"
#include "LoKi/TreeHelpers.h"
#include "LoKi/TreeOps.h"
#include "LoKi/TreeParser.h"
#include "LoKi/Trees.h"
#include "LoKi/UniqueKeeper.h"
#include "LoKi/Value.h"
#include "LoKi/Welcome.h"
#include "LoKi/WrongMass.h"
#include "LoKi/apply.h"
#include "LoKi/compose.h"
#include "LoKi/iTree.h"
#include "LoKi/same.h"
#include "LoKi/shifts.h"
#include "LoKi/valid.h"
// ============================================================================
/** @file
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
 *  @date 2001-01-23
 */
// ============================================================================
// The END
// ============================================================================
