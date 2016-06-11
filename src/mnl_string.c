#include "mnl_string.h"

/**
 * @brief strnchr find the pointer of a char in a string
 *
 * @param p the string
 * @param c the char
 * @param n find length
 *
 * @return the char position or 0
 */
char *mnl_strnchr(const char *p, char c, size_t n)
{
  if (!p) {
    return 0;
  }

  while (n-- > 0) {
    if (*p == c) {
      return ((char *)p);
    }
    p++;
  }
  return 0;
}

/**
 * @brief strnstr find the sub string in a string
 *
 * @param s the string
 * @param find the sub string
 * @param slen find length
 *
 * @return the position of sub string or NULL
 */
char *mnl_strnstr(const char *s, const char *find, size_t slen)
{
  char c, sc;
  size_t len;

  if ((c = *find++) != '\0') {
    len = strlen(find);
    do {
      do {
        if ((sc = *s++) == '\0' || slen-- < 1)
          return (NULL);
      } while (sc != c);

      if (len > slen) {
        return (NULL);
      }
    } while (strncmp(s, find, len) != 0);
    s--;
  }
  return ((char *)s);
}

// '_cups_strlcat()' - Safely concatenate two strings.
size_t mnl_strlcat(char *dst, const char *src, size_t size) {
  size_t srclen;
  size_t dstlen;

  // Figure out how much room is left...
  dstlen = strlen(dst);
  size   -= dstlen + 1;

  if (!size) {
    return (dstlen); // No room, return immediately...
  }

  // Figure out how much room is needed...
  srclen = strlen(src);

  // Copy the appropriate amount...
  if (srclen > size) {
    srclen = size;
  }

  memcpy(dst + dstlen, src, srclen);
  dst[dstlen + srclen] = '\0';

  return (dstlen + srclen);
}

// '_cups_strlcpy()' - Safely copy two strings.
size_t mnl_strlcpy(char *dst, const char *src, size_t size) {
  size_t srclen;
  // Figure out how much room is needed...
  size --;
  srclen = strlen(src);

  // Copy the appropriate amount...
  if (srclen > size) {
    srclen = size;
  }

  memcpy(dst, src, srclen);
  dst[srclen] = '\0';

  return (srclen);
}

// copy two strings file blank on the right.
size_t mnl_cpy_str(char *dst, const char *src, size_t size) {

  size_t slen = strlen(src);
  if (slen > size) {
    slen = size;
  }
  memcpy(dst, src, slen);

  size_t i;
  for (i = slen; i < size; i++) {
    *(dst+i) = ' ';
  }
  return (slen);
}

// copy two strings file '0' on the left.
size_t mnl_cpy_int(char *dst, const char *src, size_t size) {

  size_t slen = strlen(src);
  if (slen > size) {
    slen = size;
  }

  size_t i, j;
  // 补0
  for (i = 0; i < size - slen; i++) {
    dst[i] = '0';
  }

  for (i = size - slen, j = 0; i < size; i++, j++) {
    *(dst+i) = *(src+j);
  }
  return (slen);
}

// copy two strings file blank on the right.
size_t mnl_cpy_hex(char *dst, const char *src, size_t size) {
  size_t srclen;
  // Figure out how much room is needed...
  size --;
  srclen = strlen(src);

  // Copy the appropriate amount...
  if (srclen > size) {
    srclen = size;
  }

  memcpy(dst, src, srclen);
  dst[srclen] = '\0';

  return (srclen);
}
