// $Header: /afs/cern.ch/project/cvs/reps/lhcb/DAQ/MDF_GFAL/src/MDF_GFAL_dll.cpp,v 1.3 2008-01-21 10:02:18 cattanem Exp $
//	====================================================================
//  MDFIO.cpp
//	--------------------------------------------------------------------
//
//	Author    : Markus Frank
//
//	====================================================================
#include "MDF/PosixIO.h"
#include <cerrno>
#include <cstring>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include "gfal_api.h"

namespace {
  int* mdf_errno()  {  return &errno; }
  int __gfal_read(int a, void* b, unsigned int c) { return gfal_read(a,b,c); }
  int __gfal_write(int a, const void* b, unsigned int c) { return gfal_write(a,b,c); }
  long long __gfal_lseek64(int a, long long b, int c) { return gfal_lseek64(a,b,c); }
}

extern "C" EXPORT LHCb::PosixIO* MDF_GFAL()  {
  typedef LHCb::PosixIO _IO;
  static _IO p;
  if ( 0 == p.open )  {
    memset(&p,0,sizeof(p));
    p.unbuffered  = _IO::COMPLETE;
    p.open      = gfal_open;
    p.close     = gfal_close;
    p.access    = gfal_access;
    p.unlink    = gfal_unlink;
    p.read      = __gfal_read;
    p.write     = __gfal_write;
    p.lseek     = gfal_lseek;
    p.lseek64   = __gfal_lseek64;
    p.stat      = gfal_stat;
    p.stat64    = gfal_stat64;
    p.fstat     = 0;
    p.fstat64   = 0;

    p.buffered = _IO::NONE;
    p.fopen     = 0;
    p.fclose    = 0;
    p.ftell     = 0;
    p.ftell64   = 0;
    p.setopt    = 0;

    p.directory = _IO::COMPLETE;
    p.mkdir     = gfal_mkdir;
    p.rmdir     = gfal_rmdir;
    p.opendir   = gfal_opendir;
    p.readdir   = gfal_readdir;
    p.closedir  = gfal_closedir;


    p.serror    = 0;
  #ifdef _WIN32
    p.serrno    = mdf_errno;
    p.ioerrno   = mdf_errno;
  #endif
  }
  return &p;
}
