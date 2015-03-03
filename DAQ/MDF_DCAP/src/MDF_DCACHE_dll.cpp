// $Header: /afs/cern.ch/project/cvs/reps/lhcb/DAQ/MDF_DCAP/src/MDF_DCACHE_dll.cpp,v 1.3 2008-01-28 07:44:45 frankb Exp $
//	====================================================================
//  MDFIO.cpp
//	--------------------------------------------------------------------
//
//	Author    : Markus Frank
//
//	====================================================================
#include "MDF/PosixIO.h"
#include <cstring>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include "dcap.h"

typedef unsigned int  uint;
typedef long long int lint;
namespace {
  int    mdf_open(const char* fn, int a,uint b)            { return dc_open(fn,a,b);    }
  int*   mdf_errno()                                       { return &dc_errno;          }
  char*  mdf_strerror()                                    { return (char*)dc_strerror(dc_errno); }
  int    mdf_read(int fd, void* b, uint l)                 { return dc_read(fd,b, l);   }
  int    mdf_write(int fd, const void* b, uint l)          { return dc_write(fd,b,l);   }
  size_t mdf_fwrite(const void* b, uint c,uint d, FILE* f) { return dc_fwrite(b,c,d,f); }
  size_t mdf_fread(void* b, uint c, uint d, FILE* f)       { return dc_fread(b,c,d,f);  }
  lint   mdf_lseek64(int fd,lint a,int b)                  { return dc_lseek64(fd,a,b); }
  int    mdf_fseeko64(FILE* f,lint a,int b)                { return dc_fseeko64(f,a,b); }
  lint   mdf_ftello64(FILE* a)                             { return dc_ftello64(a);     }
}
extern "C" EXPORT LHCb::PosixIO* MDF_DCAP()  {
  typedef LHCb::PosixIO _IO;
  static _IO p;
  if ( 0 == p.open )  {
    memset(&p,0,sizeof(p));
    p.unbuffered  = _IO::COMPLETE;
    p.open      = mdf_open;
    p.close     = dc_close;
    p.read      = mdf_read;
    p.write     = mdf_write;
    p.lseek     = dc_lseek;
    p.lseek64   = mdf_lseek64;
    p.access    = dc_access;
    p.unlink    = dc_unlink;
    p.stat      = dc_stat;
    p.stat64    = dc_stat64;

    p.fopen     = dc_fopen;
    p.fclose    = dc_fclose;
    p.fwrite    = mdf_fwrite;
    p.fread     = mdf_fread;
    p.fseek     = dc_fseeko;
    p.fseek64   = mdf_fseeko64;
    p.fstat     = dc_fstat;
    p.fstat64   = dc_fstat64;
    p.ftell     = dc_ftell;
    p.ftell64   = mdf_ftello64;

    p.mkdir     = dc_mkdir;
    p.rmdir     = dc_rmdir;
    p.opendir   = dc_opendir;
    p.readdir   = dc_readdir;
    p.closedir  = dc_closedir;
    p.serror    = mdf_strerror;

    p.setopt    = 0;

  #ifdef _WIN32
    p.serrno    = mdf_errno;
    p.ioerrno   = mdf_errno;
  #endif
  }
  return &p;
}
