#include "mnl_mac.h"

char *_bin2hex(unsigned char *data, int size)
{
  int  i = 0;
  int  v = 0;
  char *p = NULL;
  char *buf = NULL;
  char base_char = 'A';

  buf = p = (char *)malloc(size * 2 + 1);
  for (i = 0; i < size; i++) {
    v = data[i] >> 4;
    *p++ = v < 10 ? v + '0' : v - 10 + base_char;

    v = data[i] & 0x0f;
    *p++ = v < 10 ? v + '0' : v - 10 + base_char;
  }

  *p = '\0';
  return buf;
}

unsigned char *_hex2bin(const char *data, int size, int *outlen)
{
  int i = 0;
  int len = 0;
  char char1 = '\0';
  char char2 = '\0';
  unsigned char value = 0;
  unsigned char *out = NULL;

  if (size % 2 != 0) {
    return NULL;
  }

  len = size / 2;
  out = (unsigned char *)malloc(len * sizeof(char) + 1);
  if (out == NULL) {
    return NULL;
  }

  while (i < len) {
    char1 = *data;
    if (char1 >= '0' && char1 <= '9') {
      value = (char1 - '0') << 4;
    }
    else if (char1 >= 'a' && char1 <= 'f') {
      value = (char1 - 'a' + 10) << 4;
    }
    else if (char1 >= 'A' && char1 <= 'F') {
      value = (char1 - 'A' + 10) << 4;
    }
    else {
      free(out);
      return NULL;
    }
    data++;

    char2 = *data;
    if (char2 >= '0' && char2 <= '9') {
      value |= char2 - '0';
    }
    else if (char2 >= 'a' && char2 <= 'f') {
      value |= char2 - 'a' + 10;
    }
    else if (char2 >= 'A' && char2 <= 'F') {
      value |= char2 - 'A' + 10;
    }
    else {
      free(out);
      return NULL;
    }

    data++;
    *(out + i++) = value;
  }
  *(out + i) = '\0';

  if (outlen != NULL) {
    *outlen = i;
  }

  return out;
}

void _des(char *data, char *obuf, char *ikey) {

  DES_key_schedule ks;

  int i = 0;
  int len = 0;
  int nlen = 0;
  int count = 0;

  unsigned char ch = '\0';
  unsigned char *ptr = NULL;

  unsigned char src[16] = {0}; /* 补齐后的明文, data补齐后的1/2长度 */
  unsigned char dst[16] = {0}; /* 解密后的明文, data补齐后的1/2长度 */

  unsigned char out[8] = {0};
  unsigned char tmp[8] = {0};
  unsigned char block[8] = {0};

  /* 设置密码表 */
  ptr = _hex2bin(ikey, strlen(ikey), &nlen);
  memcpy(block, ptr, 8);
  free(ptr);
  DES_set_key_unchecked((const_DES_cblock*)block, &ks);

  /* 分析补齐明文所需空间及补齐填充数据 */
  len = strlen((char *)data);
  ptr = _hex2bin(data, len, &nlen);
  len = (nlen / 8 + (nlen % 8 ? 1: 0)) * 8;
  memcpy(src, ptr, len);
  free(ptr);

  ch = 8 - nlen % 8;
  memset(src + nlen, ch, 8 - nlen % 8);

  /* 分组加密，每8字节一组 */
  count = len / 8;
  for (i = 0; i < count; i++) {
    memcpy(tmp, src + 8 * i, 8);
    DES_ecb_encrypt((const_DES_cblock*)tmp, (DES_cblock*)out, &ks, DES_ENCRYPT);
    memcpy(dst + 8 * i, out, 8);
  }

  memcpy(obuf, dst, len);
}

void mnl_mac(char *ibuf, char *obuf, char *key, size_t ilen) {

  uint8_t val[8], xor[8];
  uint8_t block[512];

  int x, n;
  int i, j = 0;

  memcpy(block, ibuf, ilen);
  x = ilen / 8;
  n = ilen % 8;
  if (n != 0) {
    memset(&block[x * 8 + n], 0x00, 8 - n);
    x += 1;
  }

  //开始运算
  memset(val, 0x00, 8);
  for(i = 0; i < x; i++, j += 8) {
    xor = val ^ &block[j];
    _des((char *)xor, (char *)val, (char *)ikey);
  }
  memcpy(obuf,val, 8 );
}
