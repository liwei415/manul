#ifndef _MNL_CBS_USRA_
#define _MNL_CBS_USRA_

#include <evhtp/evhtp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cjson/cJSON.h"
#include "mnl_net.h"
#include "mnl_utils.h"
#include "mnl_cbs.h"
#include "mnl_string.h"
#include "mnl_access.h"
#include "mnl_mac.h"
#include "mnl_common.h"

#define REQ_LEN_USRA 558
#define REQ_LEN_USRA_HEADER 133
#define REQ_LEN_USRA_BODY 425
typedef struct mnl_req_usra_s mnl_req_usra_t;
struct mnl_req_usra_s {
  //header
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
  //body
  char uid[20];
  char username[160];
  char type[1];
  char cert[3];
  char cert_no[30];
  char mobile[11];
  char spare[200];
};

#define RES_LEN_USRA 550
#define RES_LEN_USRA_HEADER 133
#define RES_LEN_USRA_BODY 417
typedef struct mnl_res_usra_s mnl_res_usra_t;
struct mnl_res_usra_s {
  //header
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
  //body
  char code[7];
  char uid[20];
  char tuid[30];
  char desc[160];
  char spare[200];
};

void mnl_cbs_usra_post(evhtp_request_t *req, void *arg);

#endif
