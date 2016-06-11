#ifndef _MNL_STRING_
#define _MNL_STRING_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mnl_strnchr(const char *p, char c, size_t n);
char *mnl_strnstr(const char *s, const char *find, size_t slen);
size_t mnl_strlcat(char *dst, const char *src, size_t size);
size_t mnl_strlcpy(char *dst, const char *src, size_t size);
size_t mnl_cpy_str(char *dst, const char *src, size_t size);
size_t mnl_cpy_int(char *dst, const char *src, size_t size);

#endif
