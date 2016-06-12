#ifndef _MNL_CBS_USRA_
#define _MNL_CBS_USRA_

#include <evhtp/evhtp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cjson/cJSON.h"
#include "mnl_utils.h"
#include "mnl_cbs.h"
#include "mnl_string.h"
#include "mnl_access.h"
#include "mnl_common.h"

typedef struct mnl_req_usra_s mnl_req_usra_t;
struct mnl_req_usra_s {
  //包头
  char flag[4];
  char len[4];
  char biz[4];
  char bank_date[8];
  char bank_time[6];
  char bank_no[30];
  char plat_date[8];
  char plat_time[6];
  char plat_no[30];
  char plat_code[10];
  char comm_code[7];
  char comm_vcode[16];
  //包体
  char uid[20];
  char username[160];
  char type[1];
  char cert[3];
  char cert_no[30];
  char mobile[11];
  char spare[200];
};

typedef struct mnl_res_usra_s mnl_res_usra_t;
struct mnl_res_usra_s {
  char code[7+1];
  char uid[20+1];
  char tuid[30+1];
  char desc[160+1];
  char spare[200+1];
};

void mnl_cbs_usra_post(evhtp_request_t *req, void *arg);

#endif
