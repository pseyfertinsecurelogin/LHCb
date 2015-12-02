/* cdb_init.c: cdb_init, cdb_free and cdb_read routines
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include <sys/types.h>
#ifdef _WIN32
# include <windows.h>
#else
# include <sys/mman.h>
# ifndef MAP_FAILED
#  define MAP_FAILED ((void*)-1)
# endif
#endif
#include <sys/stat.h>

/* cdb_int.h: internal cdb library declarations
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include "cdb.h"
#include <errno.h>
#include <string.h>

#ifndef EPROTO
# define EPROTO EINVAL
#endif

#define internal_function
#ifndef internal_function
# ifdef __GNUC__
#  define internal_function __attribute__((visibility("hidden")))
# else
#  define internal_function
# endif
#endif

struct cdb_rec {
  unsigned hval;
  unsigned rpos;
};

struct cdb_rl {
  struct cdb_rl *next;
  unsigned cnt;
  struct cdb_rec rec[254];
};

int _cdb_make_write(struct cdb_make *cdbmp,
		    const unsigned char *ptr, unsigned len);
int _cdb_make_fullwrite(int fd, const unsigned char *buf, unsigned len);
int _cdb_make_flush(struct cdb_make *cdbmp);
int _cdb_make_add(struct cdb_make *cdbmp, unsigned hval,
                  const unsigned char *key, unsigned klen,
                  const unsigned char *val, unsigned vlen);

int
cdb_init(struct cdb *cdbp, int fd)
{
  struct stat st;
  unsigned char *mem;
  unsigned fsize, dend;
#ifdef _WIN32
  HANDLE hFile, hMapping;
#endif

  /* get file size */
  if (fstat(fd, &st) < 0)
    return -1;
  /* trivial sanity check: at least toc should be here */
  if (st.st_size < 2048)
    return errno = EPROTO, -1;
  fsize = st.st_size < 0xffffffffu ? st.st_size : 0xffffffffu;
  /* memory-map file */
#ifdef _WIN32
  hFile = (HANDLE) _get_osfhandle(fd);
  if (hFile == (HANDLE) -1)
    return -1;
  hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
  if (!hMapping)
    return -1;
  mem = (unsigned char *)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
  CloseHandle(hMapping);
  if (!mem)
    return -1;
#else
  mem = (unsigned char*)mmap(nullptr, fsize, PROT_READ, MAP_SHARED, fd, 0);
  if (mem == MAP_FAILED)
    return -1;
#endif /* _WIN32 */

  cdbp->cdb_fd = fd;
  cdbp->cdb_fsize = fsize;
  cdbp->cdb_mem = mem;

#if 0
  /* XXX don't know well about madvise syscall -- is it legal
     to set different options for parts of one mmap() region?
     There is also posix_madvise() exist, with POSIX_MADV_RANDOM etc...
  */
#ifdef MADV_RANDOM
  /* set madvise() parameters. Ignore errors for now if system
     doesn't support it */
  madvise(mem, 2048, MADV_WILLNEED);
  madvise(mem + 2048, cdbp->cdb_fsize - 2048, MADV_RANDOM);
#endif
#endif

  cdbp->cdb_vpos = cdbp->cdb_vlen = 0;
  cdbp->cdb_kpos = cdbp->cdb_klen = 0;
  dend = cdb_unpack(mem);
  if (dend < 2048) dend = 2048;
  else if (dend >= fsize) dend = fsize;
  cdbp->cdb_dend = dend;

  return 0;
}

void
cdb_free(struct cdb *cdbp)
{
  if (cdbp->cdb_mem) {
#ifdef _WIN32
    UnmapViewOfFile((void*) cdbp->cdb_mem);
#else
    munmap((void*)cdbp->cdb_mem, cdbp->cdb_fsize);
#endif /* _WIN32 */
    cdbp->cdb_mem = nullptr;
  }
  cdbp->cdb_fsize = 0;
}

const void *
cdb_get(const struct cdb *cdbp, unsigned len, unsigned pos)
{
  if (pos > cdbp->cdb_fsize || cdbp->cdb_fsize - pos < len) {
    errno = EPROTO;
    return nullptr;
  }
  return cdbp->cdb_mem + pos;
}

int
cdb_read(const struct cdb *cdbp, void *buf, unsigned len, unsigned pos)
{
  const void *data = cdb_get(cdbp, len, pos);
  if (!data) return -1;
  memcpy(buf, data, len);
  return 0;
}
/* cdb_find.c: cdb_find routine
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */


int
cdb_find(struct cdb *cdbp, const void *key, unsigned klen)
{
  const unsigned char *htp;	/* hash table pointer */
  const unsigned char *htab;	/* hash table */
  const unsigned char *htend;	/* end of hash table */
  unsigned httodo;		/* ht bytes left to look */
  unsigned pos, n;

  unsigned hval;

  if (klen >= cdbp->cdb_dend)	/* if key size is too large */
    return 0;

  hval = cdb_hash(key, klen);

  /* find (pos,n) hash table to use */
  /* first 2048 bytes (toc) are always available */
  /* (hval % 256) * 8 */
  htp = cdbp->cdb_mem + ((hval << 3) & 2047); /* index in toc (256x8) */
  n = cdb_unpack(htp + 4);	/* table size */
  if (!n)			/* empty table */
    return 0;			/* not found */
  httodo = n << 3;		/* bytes of htab to lookup */
  pos = cdb_unpack(htp);	/* htab position */
  if (n > (cdbp->cdb_fsize >> 3) /* overflow of httodo ? */
      || pos < cdbp->cdb_dend /* is htab inside data section ? */
      || pos > cdbp->cdb_fsize /* htab start within file ? */
      || httodo > cdbp->cdb_fsize - pos) /* entrie htab within file ? */
    return errno = EPROTO, -1;

  htab = cdbp->cdb_mem + pos;	/* htab pointer */
  htend = htab + httodo;	/* after end of htab */
  /* htab starting position: rest of hval modulo htsize, 8bytes per elt */
  htp = htab + (((hval >> 8) % n) << 3);

  for(;;) {
    pos = cdb_unpack(htp + 4);	/* record position */
    if (!pos)
      return 0;
    if (cdb_unpack(htp) == hval) {
      if (pos > cdbp->cdb_dend - 8) /* key+val lengths */
	return errno = EPROTO, -1;
      if (cdb_unpack(cdbp->cdb_mem + pos) == klen) {
	if (cdbp->cdb_dend - klen < pos + 8)
	  return errno = EPROTO, -1;
	if (memcmp(key, cdbp->cdb_mem + pos + 8, klen) == 0) {
	  n = cdb_unpack(cdbp->cdb_mem + pos + 4);
	  pos += 8;
	  if (cdbp->cdb_dend < n || cdbp->cdb_dend - n < pos + klen)
	    return errno = EPROTO, -1;
	  cdbp->cdb_kpos = pos;
	  cdbp->cdb_klen = klen;
	  cdbp->cdb_vpos = pos + klen;
	  cdbp->cdb_vlen = n;
	  return 1;
	}
      }
    }
    httodo -= 8;
    if (!httodo)
      return 0;
    if ((htp += 8) >= htend)
      htp = htab;
  }

}
/* cdb_findnext.c: sequential cdb_find routines
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

/* see cdb_find.c for comments */


int
cdb_findinit(struct cdb_find *cdbfp, struct cdb *cdbp,
             const void *key, unsigned klen)
{
  unsigned n, pos;

  cdbfp->cdb_cdbp = cdbp;
  cdbfp->cdb_key = key;
  cdbfp->cdb_klen = klen;
  cdbfp->cdb_hval = cdb_hash(key, klen);

  cdbfp->cdb_htp = cdbp->cdb_mem + ((cdbfp->cdb_hval << 3) & 2047);
  n = cdb_unpack(cdbfp->cdb_htp + 4);
  cdbfp->cdb_httodo = n << 3;
  if (!n)
    return 0;
  pos = cdb_unpack(cdbfp->cdb_htp);
  if (n > (cdbp->cdb_fsize >> 3)
      || pos < cdbp->cdb_dend
      || pos > cdbp->cdb_fsize
      || cdbfp->cdb_httodo > cdbp->cdb_fsize - pos)
    return errno = EPROTO, -1;

  cdbfp->cdb_htab = cdbp->cdb_mem + pos;
  cdbfp->cdb_htend = cdbfp->cdb_htab + cdbfp->cdb_httodo;
  cdbfp->cdb_htp = cdbfp->cdb_htab + (((cdbfp->cdb_hval >> 8) % n) << 3);

  return 1;
}

int
cdb_findnext(struct cdb_find *cdbfp) {
  struct cdb *cdbp = cdbfp->cdb_cdbp;
  unsigned pos, n;
  unsigned klen = cdbfp->cdb_klen;

  while(cdbfp->cdb_httodo) {
    pos = cdb_unpack(cdbfp->cdb_htp + 4);
    if (!pos)
      return 0;
    n = cdb_unpack(cdbfp->cdb_htp) == cdbfp->cdb_hval;
    if ((cdbfp->cdb_htp += 8) >= cdbfp->cdb_htend)
      cdbfp->cdb_htp = cdbfp->cdb_htab;
    cdbfp->cdb_httodo -= 8;
    if (n) {
      if (pos > cdbp->cdb_fsize - 8)
	return errno = EPROTO, -1;
      if (cdb_unpack(cdbp->cdb_mem + pos) == klen) {
	if (cdbp->cdb_fsize - klen < pos + 8)
	  return errno = EPROTO, -1;
	if (memcmp(cdbfp->cdb_key,
	    cdbp->cdb_mem + pos + 8, klen) == 0) {
	  n = cdb_unpack(cdbp->cdb_mem + pos + 4);
	  pos += 8;
	  if (cdbp->cdb_fsize < n ||
              cdbp->cdb_fsize - n < pos + klen)
	    return errno = EPROTO, -1;
	  cdbp->cdb_kpos = pos;
	  cdbp->cdb_klen = klen;
	  cdbp->cdb_vpos = pos + klen;
	  cdbp->cdb_vlen = n;
	  return 1;
	}
      }
    }
  }

  return 0;

}
/* cdb_seq.c: sequential record retrieval routines
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */


int
cdb_seqnext(unsigned *cptr, struct cdb *cdbp) {
  unsigned klen, vlen;
  unsigned pos = *cptr;
  unsigned dend = cdbp->cdb_dend;
  const unsigned char *mem = cdbp->cdb_mem;
  if (pos > dend - 8)
    return 0;
  klen = cdb_unpack(mem + pos);
  vlen = cdb_unpack(mem + pos + 4);
  pos += 8;
  if (dend - klen < pos || dend - vlen < pos + klen)
    return errno = EPROTO, -1;
  cdbp->cdb_kpos = pos;
  cdbp->cdb_klen = klen;
  cdbp->cdb_vpos = pos + klen;
  cdbp->cdb_vlen = vlen;
  *cptr = pos + klen + vlen;
  return 1;
}
/* cdb_seek.c: old interface for reading cdb file
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include <unistd.h>

#ifndef SEEK_SET
# define SEEK_SET 0
#endif

/* read a chunk from file, ignoring interrupts (EINTR) */

int
cdb_bread(int fd, void *buf, int len)
{
  int l;
  while(len > 0) {
    do l = read(fd, buf, len);
    while(l < 0 && errno == EINTR);
    if (l <= 0) {
      if (!l)
        errno = EIO;
      return -1;
    }
    buf = (char*)buf + l;
    len -= l;
  }
  return 0;
}

/* find a given key in cdb file, seek a file pointer to it's value and
   place data length to *dlenp. */

int
cdb_seek(int fd, const void *key, unsigned klen, unsigned *dlenp)
{
  unsigned htstart;		/* hash table start position */
  unsigned htsize;		/* number of elements in a hash table */
  unsigned httodo;		/* hash table elements left to look */
  unsigned hti;			/* hash table index */
  unsigned pos;			/* position in a file */
  unsigned hval;			/* key's hash value */
  unsigned char rbuf[64];	/* read buffer */
  int needseek = 1;		/* if we should seek to a hash slot */

  hval = cdb_hash(key, klen);
  pos = (hval & 0xff) << 3; /* position in TOC */
  /* read the hash table parameters */
  if (lseek(fd, pos, SEEK_SET) < 0 || cdb_bread(fd, rbuf, 8) < 0)
    return -1;
  if ((htsize = cdb_unpack(rbuf + 4)) == 0)
    return 0;
  hti = (hval >> 8) % htsize;	/* start position in hash table */
  httodo = htsize;
  htstart = cdb_unpack(rbuf);

  for(;;) {
    if (needseek && lseek(fd, htstart + (hti << 3), SEEK_SET) < 0)
      return -1;
    if (cdb_bread(fd, rbuf, 8) < 0)
      return -1;
    if ((pos = cdb_unpack(rbuf + 4)) == 0) /* not found */
      return 0;

    if (cdb_unpack(rbuf) != hval) /* hash value not matched */
      needseek = 0;
    else { /* hash value matched */
      if (lseek(fd, pos, SEEK_SET) < 0 || cdb_bread(fd, rbuf, 8) < 0)
	return -1;
      if (cdb_unpack(rbuf) == klen) { /* key length matches */
	/* read the key from file and compare with wanted */
	unsigned l = klen, c;
	const char *k = (const char*)key;
	if (dlenp)
	  *dlenp = cdb_unpack(rbuf + 4); /* save value length */
	for(;;) {
	  if (!l) /* the whole key read and matches, return */
	    return 1;
	  c = l > sizeof(rbuf) ? sizeof(rbuf) : l;
	  if (cdb_bread(fd, rbuf, c) < 0)
	    return -1;
	  if (memcmp(rbuf, k, c) != 0) /* no, it differs, stop here */
	    break;
	  k += c; l -= c;
	}
      }
      needseek = 1; /* we're looked to other place, should seek back */
    }
    if (!--httodo)
      return 0;
    if (++hti == htsize) {
      hti = 0;
      needseek = 1;
    }
  }
}
/* cdb_unpack.c: unpack 32bit integer
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include "cdb.h"

unsigned
cdb_unpack(const unsigned char buf[4])
{
  unsigned n = buf[3];
  n <<= 8; n |= buf[2];
  n <<= 8; n |= buf[1];
  n <<= 8; n |= buf[0];
  return n;
}
/* cdb_make_add.c: basic cdb_make_add routine
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include <stdlib.h> /* for malloc */

int internal_function
_cdb_make_add(struct cdb_make *cdbmp, unsigned hval,
              const unsigned char *key, unsigned klen,
              const unsigned char *val, unsigned vlen)
{
  unsigned char rlen[8];
  struct cdb_rl *rl;
  unsigned i;
  if (klen > 0xffffffff - (cdbmp->cdb_dpos + 8) ||
      vlen > 0xffffffff - (cdbmp->cdb_dpos + klen + 8))
    return errno = ENOMEM, -1;
  i = hval & 255;
  rl = cdbmp->cdb_rec[i];
  if (!rl || rl->cnt >= sizeof(rl->rec)/sizeof(rl->rec[0])) {
    rl = (struct cdb_rl*)malloc(sizeof(struct cdb_rl));
    if (!rl)
      return errno = ENOMEM, -1;
    rl->cnt = 0;
    rl->next = cdbmp->cdb_rec[i];
    cdbmp->cdb_rec[i] = rl;
  }
  i = rl->cnt++;
  rl->rec[i].hval = hval;
  rl->rec[i].rpos = cdbmp->cdb_dpos;
  ++cdbmp->cdb_rcnt;
  cdb_pack(klen, rlen);
  cdb_pack(vlen, rlen + 4);
  if (_cdb_make_write(cdbmp, rlen, 8) < 0 ||
      _cdb_make_write(cdbmp, key, klen) < 0 ||
      _cdb_make_write(cdbmp, val, vlen) < 0)
    return -1;
  return 0;
}

int
cdb_make_add(struct cdb_make *cdbmp,
             const unsigned char *key, unsigned klen,
             const unsigned char *val, unsigned vlen) {
  return _cdb_make_add(cdbmp, cdb_hash(key, klen), key, klen, val, vlen);
}
/* cdb_make_put.c: "advanced" cdb_make_put routine
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

static void
fixup_rpos(struct cdb_make *cdbmp, unsigned rpos, unsigned rlen) {
  unsigned i;
  struct cdb_rl *rl;
  register struct cdb_rec *rp, *rs;
  for (i = 0; i < 256; ++i) {
    for (rl = cdbmp->cdb_rec[i]; rl; rl = rl->next)
      for (rs = rl->rec, rp = rs + rl->cnt; --rp >= rs;)
        if (rp->rpos <= rpos) goto nexthash;
        else rp->rpos -= rlen;
nexthash:;
  }
}

static int
remove_record(struct cdb_make *cdbmp, unsigned rpos, unsigned rlen) {
  unsigned pos, len;
  int r, fd;

  len = cdbmp->cdb_dpos - rpos - rlen;
  cdbmp->cdb_dpos -= rlen;
  if (!len)
    return 0;	/* it was the last record, nothing to do */
  pos = rpos;
  fd = cdbmp->cdb_fd;
  do {
    r = len > sizeof(cdbmp->cdb_buf) ? sizeof(cdbmp->cdb_buf) : len;
    if (lseek(fd, pos + rlen, SEEK_SET) < 0 ||
        (r = read(fd, cdbmp->cdb_buf, r)) <= 0)
      return -1;
    if (lseek(fd, pos, SEEK_SET) < 0 ||
        _cdb_make_fullwrite(fd, cdbmp->cdb_buf, r) < 0)
      return -1;
    pos += r;
    len -= r;
  } while(len);
  assert(cdbmp->cdb_dpos == pos);
  fixup_rpos(cdbmp, rpos, rlen);
  return 0;
}

static int
zerofill_record(struct cdb_make *cdbmp, unsigned rpos, unsigned rlen) {
  if (rpos + rlen == cdbmp->cdb_dpos) {
    cdbmp->cdb_dpos = rpos;
    return 0;
  }
  if (lseek(cdbmp->cdb_fd, rpos, SEEK_SET) < 0)
    return -1;
  memset(cdbmp->cdb_buf, 0, sizeof(cdbmp->cdb_buf));
  cdb_pack(rlen - 8, cdbmp->cdb_buf + 4);
  for(;;) {
    rpos = rlen > sizeof(cdbmp->cdb_buf) ? sizeof(cdbmp->cdb_buf) : rlen;
    if (_cdb_make_fullwrite(cdbmp->cdb_fd, cdbmp->cdb_buf, rpos) < 0)
      return -1;
    rlen -= rpos;
    if (!rlen) return 0;
    memset(cdbmp->cdb_buf + 4, 0, 4);
  }
}

/* return: 0 = not found, 1 = error, or record length */
static unsigned
match(struct cdb_make *cdbmp, unsigned pos, const unsigned char *key, unsigned klen)
{
  int len;
  unsigned rlen;
  if (lseek(cdbmp->cdb_fd, pos, SEEK_SET) < 0)
    return 1;
  if (read(cdbmp->cdb_fd, cdbmp->cdb_buf, 8) != 8)
    return 1;
  if (cdb_unpack(cdbmp->cdb_buf) != klen)
    return 0;

  /* record length; check its validity */
  rlen = cdb_unpack(cdbmp->cdb_buf + 4);
  if (rlen > cdbmp->cdb_dpos - pos - klen - 8)
    return errno = EPROTO, 1;	/* someone changed our file? */
  rlen += klen + 8;

  while(klen) {
    len = klen > sizeof(cdbmp->cdb_buf) ? sizeof(cdbmp->cdb_buf) : klen;
    len = read(cdbmp->cdb_fd, cdbmp->cdb_buf, len);
    if (len <= 0)
      return 1;
    if (memcmp(cdbmp->cdb_buf, key, len) != 0)
      return 0;
    key += len;
    klen -= len;
  }

  return rlen;
}

static int
findrec(struct cdb_make *cdbmp,
        const unsigned char *key, unsigned klen, unsigned hval,
        enum cdb_put_mode mode)
{
  struct cdb_rl *rl;
  struct cdb_rec *rp, *rs;
  unsigned r;
  int seeked = 0;
  int ret = 0;
  for(rl = cdbmp->cdb_rec[hval&255]; rl; rl = rl->next)
    for(rs = rl->rec, rp = rs + rl->cnt; --rp >= rs;) {
      if (rp->hval != hval)
	continue;
      /*XXX this explicit flush may be unnecessary having
       * smarter match() that looks into cdb_buf too, but
       * most of a time here spent in finding hash values
       * (above), not keys */
      if (!seeked && _cdb_make_flush(cdbmp) < 0)
        return -1;
      seeked = 1;
      r = match(cdbmp, rp->rpos, key, klen);
      if (!r)
	continue;
      if (r == 1)
	return -1;
      ret = 1;
      switch(mode) {
      case CDB_FIND_REMOVE:
        if (remove_record(cdbmp, rp->rpos, r) < 0)
          return -1;
	break;
      case CDB_FIND_FILL0:
	if (zerofill_record(cdbmp, rp->rpos, r) < 0)
          return -1;
	break;
      default: goto finish;
      }
      memmove(rp, rp + 1, (rs + rl->cnt - 1 - rp) * sizeof(*rp));
      --rl->cnt;
      --cdbmp->cdb_rcnt;
  }
finish:
  if (seeked && lseek(cdbmp->cdb_fd, cdbmp->cdb_dpos, SEEK_SET) < 0)
    return -1;
  return ret;
}

int
cdb_make_find(struct cdb_make *cdbmp,
              const unsigned char *key, unsigned klen,
              enum cdb_put_mode mode)
{
  return findrec(cdbmp, key, klen, cdb_hash(key, klen), mode);
}

int
cdb_make_exists(struct cdb_make *cdbmp,
                const unsigned char *key, unsigned klen)
{
  return cdb_make_find(cdbmp, key, klen, CDB_FIND);
}

int
cdb_make_put(struct cdb_make *cdbmp,
	     const unsigned char *key, unsigned klen,
	     const unsigned char *val, unsigned vlen,
	     enum cdb_put_mode mode)
{
  unsigned hval = cdb_hash(key, klen);
  int r;

  switch(mode) {
    case CDB_PUT_REPLACE:
    case CDB_PUT_INSERT:
    case CDB_PUT_WARN:
    case CDB_PUT_REPLACE0:
      r = findrec(cdbmp, key, klen, hval, mode);
      if (r < 0)
        return -1;
      if (r && mode == CDB_PUT_INSERT)
        return errno = EEXIST, 1;
      break;

    case CDB_PUT_ADD:
      r = 0;
      break;

    default:
      return errno = EINVAL, -1;
  }

  if (_cdb_make_add(cdbmp, hval, key, klen, val, vlen) < 0)
    return -1;

  return r;
}

/* cdb_make.c: basic cdb creation routines
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void
cdb_pack(unsigned num, unsigned char buf[4])
{
  buf[0] = num & 255; num >>= 8;
  buf[1] = num & 255; num >>= 8;
  buf[2] = num & 255;
  buf[3] = num >> 8;
}

int
cdb_make_start(struct cdb_make *cdbmp, int fd)
{
  memset(cdbmp, 0, sizeof(*cdbmp));
  cdbmp->cdb_fd = fd;
  cdbmp->cdb_dpos = 2048;
  cdbmp->cdb_bpos = cdbmp->cdb_buf + 2048;
  return 0;
}

int internal_function
_cdb_make_fullwrite(int fd, const unsigned char *buf, unsigned len)
{
  while(len) {
    int l = write(fd, buf, len);
    if (l > 0) {
      len -= l;
      buf += l;
    }
    else if (l < 0 && errno != EINTR)
      return -1;
  }
  return 0;
}

int internal_function
_cdb_make_flush(struct cdb_make *cdbmp) {
  unsigned len = cdbmp->cdb_bpos - cdbmp->cdb_buf;
  if (len) {
    if (_cdb_make_fullwrite(cdbmp->cdb_fd, cdbmp->cdb_buf, len) < 0)
      return -1;
    cdbmp->cdb_bpos = cdbmp->cdb_buf;
  }
  return 0;
}

int internal_function
_cdb_make_write(struct cdb_make *cdbmp, const unsigned char *ptr, unsigned len)
{
  unsigned l = sizeof(cdbmp->cdb_buf) - (cdbmp->cdb_bpos - cdbmp->cdb_buf);
  cdbmp->cdb_dpos += len;
  if (len > l) {
    memcpy(cdbmp->cdb_bpos, ptr, l);
    cdbmp->cdb_bpos += l;
    if (_cdb_make_flush(cdbmp) < 0)
      return -1;
    ptr += l; len -= l;
    l = len / sizeof(cdbmp->cdb_buf);
    if (l) {
      l *= sizeof(cdbmp->cdb_buf);
      if (_cdb_make_fullwrite(cdbmp->cdb_fd, ptr, l) < 0)
        return -1;
      ptr += l; len -= l;
    }
  }
  if (len) {
    memcpy(cdbmp->cdb_bpos, ptr, len);
    cdbmp->cdb_bpos += len;
  }
  return 0;
}

static int
cdb_make_finish_internal(struct cdb_make *cdbmp)
{
  unsigned hcnt[256];		/* hash table counts */
  unsigned hpos[256];		/* hash table positions */
  struct cdb_rec *htab;
  unsigned char *p;
  struct cdb_rl *rl;
  unsigned hsize;
  unsigned t, i;

  if (((0xffffffff - cdbmp->cdb_dpos) >> 3) < cdbmp->cdb_rcnt)
    return errno = ENOMEM, -1;

  /* count htab sizes and reorder reclists */
  hsize = 0;
  for (t = 0; t < 256; ++t) {
    struct cdb_rl *rlt = nullptr;
    i = 0;
    rl = cdbmp->cdb_rec[t];
    while(rl) {
      struct cdb_rl *rln = rl->next;
      rl->next = rlt;
      rlt = rl;
      i += rl->cnt;
      rl = rln;
    }
    cdbmp->cdb_rec[t] = rlt;
    if (hsize < (hcnt[t] = i << 1))
      hsize = hcnt[t];
  }

  /* allocate memory to hold max htable */
  htab = (struct cdb_rec*)malloc((hsize + 2) * sizeof(struct cdb_rec));
  if (!htab)
    return errno = ENOENT, -1;
  p = (unsigned char *)htab;
  htab += 2;

  /* build hash tables */
  for (t = 0; t < 256; ++t) {
    unsigned len, hi;
    hpos[t] = cdbmp->cdb_dpos;
    if ((len = hcnt[t]) == 0)
      continue;
    for (i = 0; i < len; ++i)
      htab[i].hval = htab[i].rpos = 0;
    for (rl = cdbmp->cdb_rec[t]; rl; rl = rl->next)
      for (i = 0; i < rl->cnt; ++i) {
       hi = (rl->rec[i].hval >> 8) % len;
        while(htab[hi].rpos)
          if (++hi == len)
            hi = 0;
        htab[hi] = rl->rec[i];
      }
    for (i = 0; i < len; ++i) {
      cdb_pack(htab[i].hval, p + (i << 3));
      cdb_pack(htab[i].rpos, p + (i << 3) + 4);
    }
    if (_cdb_make_write(cdbmp, p, len << 3) < 0) {
      free(p);
      return -1;
    }
  }
  free(p);
  if (_cdb_make_flush(cdbmp) < 0)
    return -1;
  p = cdbmp->cdb_buf;
  for (t = 0; t < 256; ++t) {
    cdb_pack(hpos[t], p + (t << 3));
    cdb_pack(hcnt[t], p + (t << 3) + 4);
  }
  if (lseek(cdbmp->cdb_fd, 0, 0) != 0 ||
      _cdb_make_fullwrite(cdbmp->cdb_fd, p, 2048) != 0)
    return -1;

  return 0;
}

static void
cdb_make_free(struct cdb_make *cdbmp)
{
  unsigned t;
  for(t = 0; t < 256; ++t) {
    struct cdb_rl *rl = cdbmp->cdb_rec[t];
    while(rl) {
      struct cdb_rl *tm = rl;
      rl = rl->next;
      free(tm);
    }
  }
}

int
cdb_make_finish(struct cdb_make *cdbmp)
{
  int r = cdb_make_finish_internal(cdbmp);
  cdb_make_free(cdbmp);
  return r;
}

/* cdb_hash.c: cdb hashing routine
 *
 * This file is a part of tinycdb package by Michael Tokarev, mjt@corpit.ru.
 * Public domain.
 */

#include "cdb.h"

unsigned
cdb_hash(const void *buf, unsigned len)
{
  register const unsigned char *p = (const unsigned char *)buf;
  register const unsigned char *end = p + len;
  register unsigned hash = 5381;	/* start value */
  while (p < end) hash = (hash + (hash << 5)) ^ *p++;
  return hash;
}
