#include "mnl_hex.h"

#define xtod(c) ((c>='0' && c<='9') ? c-'0' : ((c>='A' && c<='F') ? c-'A'+10 : ((c>='a' && c<='f') ? c-'a'+10 : 0)))

static char byte_map[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
static int byte_map_len = sizeof(byte_map);

static uint8_t _hex_fchar(char c) {

  if(c >= '0' && c <= '9') {
    return c - '0';
  }

  if(c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  }

  if(c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  return 255;
}

static char _hex_tchar(uint8_t nibble) {

  if(nibble < byte_map_len) {
    return byte_map[nibble];
  }
  return '*';
}

void *mnl_hex_enc(uint8_t *ibuf, uint8_t *obuf, size_t ilen) {

  size_t i;
  for(i = 0; i < ilen; i++) {
    obuf[i*2] = _hex_tchar(ibuf[i] >> 4);
    obuf[i*2+1] = _hex_tchar(ibuf[i] & 0x0f);
  }
  return obuf;
}

void *mnl_hex_dec(uint8_t *ibuf, uint8_t *obuf) {

  uint8_t *p;
  int len, i;

  len = strlen((char *)ibuf) / 2;
  p = ibuf;
  for(i = 0; i < len; i++) {
    obuf[i] = (_hex_fchar(*p) << 4) | _hex_fchar(*(p+1));
    p += 2;
  }
  obuf[len] = 0;
  return obuf;
}
