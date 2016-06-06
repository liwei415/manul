#ifndef _MNL_CBS_
#define _MNL_CBS_

#include <evhtp/evhtp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cjson/cJSON.h"
#include "mnl_utils.h"
#include "mnl_string.h"
#include "mnl_access.h"
#include "mnl_common.h"

typedef struct mp_arg_s mp_arg_t;
struct mp_arg_s {
  evhtp_request_t *req;
  thr_arg_t *thr_arg;
  char address[16];
  int partno;
  int succno;
  int check_name;
};

mnl_cbs_headers_conf_t * mnl_cbs_get_headers_conf(const char *hdr_str);
void mnl_cbs_headers_free(mnl_cbs_headers_conf_t *hcf);
int mnl_cbs_headers_add(evhtp_request_t *req, mnl_cbs_headers_conf_t *hcf);
evthr_t *mnl_cbs_get_request_thr(evhtp_request_t *request);
void mnl_cbs_index(evhtp_request_t *req, void *arg);

#endif
