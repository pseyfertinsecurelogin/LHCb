#ifndef DICT_RICHFUTUREUTILSDICT_H
#define DICT_RICHFUTUREUTILSDICT_H 1

//#include "RichFutureUtils/RichHypoData.h"
#include "RichFutureUtils/RichGeomPhoton.h"
#include "RichFutureUtils/RichSIMDGeomPhoton.h"

namespace
{
  struct _Instantiations
  {

    // instantiate templated classes
    // Rich::Future::HypoData<float>            obj_1;
    // Rich::Future::HypoData<double>           obj_2;
    Rich::SIMD::Future::RecoPhoton         obj_3;
    Rich::SIMD::Future::RecoPhoton::SIMDFP obj_4;
    Rich::SIMD::Future::RecoPhoton::Vector obj_6;
  };
} // namespace

#endif // DICT_RICHFUTUREUTILSDICT_H
