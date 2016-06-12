#include "mnl_iconv.h"

void mnl_iconv(const char *fset, const char *tset, char *fbuf, size_t flen, char *tbuf, size_t tlen) {

  iconv_t cd;
  cd = iconv_open(tset, fset);
  char **f2 = &fbuf;
  char **t2 = &tbuf;
  iconv(cd, f2, &flen, t2, &tlen);
  iconv_close(cd);
}
