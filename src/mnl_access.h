#ifndef _MNL_ACCESS_
#define _MNL_ACCESS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "mnl_log.h"
#include "mnl_common.h"

#define  MNL_OK          0
#define  MNL_ERROR      -1
#define  MNL_FORBIDDEN  -2

#ifndef INADDR_NONE  /* Solaris */
#define INADDR_NONE  ((unsigned int) -1)
#endif

mnl_access_conf_t * mnl_access_get(const char *acc_str);
int mnl_access_inet(mnl_access_conf_t *cf, in_addr_t addr);
void mnl_access_free(mnl_access_conf_t *cf);

#endif
