#ifndef _MNL_MAC_
#define _MNL_MAC_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>

void mnl_mac(uint8_t *ibuf, uint8_t *obuf, uint8_t *ikey, size_t ilen);

#endif
