#ifndef _MNL_ICONV_
#define _MNL_ICONV_

#include <iconv.h>

void mnl_iconv(const char *fset, const char *tset, char *fbuf, size_t flen, char *tbuf, size_t tlen);

#endif
