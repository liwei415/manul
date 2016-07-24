#ifndef _MNL_MAC_
#define _MNL_MAC_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>

#include "mnl_common.h"

void *mnl_mac_enc(char *ibuf, char *obuf, char *ikey);

#endif
