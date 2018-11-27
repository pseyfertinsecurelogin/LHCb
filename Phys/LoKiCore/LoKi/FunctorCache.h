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
#ifndef LOKI_FUNCTOR_CACHE_H
#define LOKI_FUNCTOR_CACHE_H
// ============================================================================
// Include files
// ============================================================================
#include "Gaudi/PluginService.h"
#if GAUDI_PLUGIN_SERVICE_USE_V2
#include "Gaudi/PluginServiceV1.h"
#define LOKI_FUNCTOR_CACHE_PLUGIN_VERSION v1::
#else
#define LOKI_FUNCTOR_CACHE_PLUGIN_VERSION
#endif
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Context.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  template <class T>
  struct CacheFactory
  {
    typedef T CutType;
    typedef ::Gaudi::PluginService:: LOKI_FUNCTOR_CACHE_PLUGIN_VERSION Factory<CutType*,LoKi::Context> Factory;
    static std::string id(unsigned int hash)
    {
      return "loki_functor_" + std::to_string(hash);
    }
  };
  // ==========================================================================
  namespace Details
  {
    // ========================================================================
    template <class T, unsigned int HASH>
    struct CacheFactory: public ::LoKi::CacheFactory<T>
    {
      typedef T CutType;
      typedef ::Gaudi::PluginService:: LOKI_FUNCTOR_CACHE_PLUGIN_VERSION Factory<CutType*,LoKi::Context> Factory;
      // static typename Factory::ReturnType create();
      static typename Factory::ReturnType create ( const  LoKi::Context& context );
    };
    // ========================================================================
  } //                                           end of namespace LoKi::Details
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  namespace PluginService
  {
#if GAUDI_PLUGIN_SERVICE_USE_V2
   namespace v1
   {
#endif
    // ========================================================================
    namespace Details
    {
      // ======================================================================
      template <class CutType, unsigned int HASH>
      class Factory<LoKi::Details::CacheFactory<CutType, HASH> >
      {
      public:
        //template <typename S>
        // static typename S::ReturnType create()
        // { return LoKi::Details::CacheFactory<CutType, HASH>::create(); }
        template <typename S,  typename... Args>
        static typename S::ReturnType create(Args... args)
        { return LoKi::Details::CacheFactory<CutType, HASH>::create ( args... ); }
      };
      // ======================================================================
    } //                         end of namespace Gaudi:PluginServiuce::Details
#if GAUDI_PLUGIN_SERVICE_USE_V2
   }
#endif
    // ========================================================================
  } //                                    end of namespace Gaudi::PluginService
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
#if GAUDI_PLUGIN_SERVICE_USE_V2
#define DECLARE_LOKI_FUNCTOR(CutType, Hash) \
  namespace { \
    typedef LoKi::Details::CacheFactory<CutType, Hash> loki_cache_entry ## Hash; \
    _PS_V1_DECLARE_COMPONENT_WITH_ID(loki_cache_entry ## Hash, loki_cache_entry ## Hash::id(Hash)) \
  }
#else
#define DECLARE_LOKI_FUNCTOR(CutType, Hash) \
  namespace { \
    typedef LoKi::Details::CacheFactory<CutType, Hash> loki_cache_entry ## Hash; \
    DECLARE_COMPONENT_WITH_ID(loki_cache_entry ## Hash, loki_cache_entry ## Hash::id(Hash)) \
  }
#endif
// ============================================================================
// The END
// ============================================================================
#endif
// ============================================================================
