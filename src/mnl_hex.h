#ifndef _MNL_HEX_
#define _MNL_HEX_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void *mnl_hex_enc(uint8_t *ibuf, uint8_t *obuf, size_t ilen);
void *mnl_hex_dec(uint8_t *ibuf, uint8_t *obuf);

#endif
