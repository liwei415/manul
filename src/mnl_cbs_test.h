#ifndef _MNL_CBS_TEST_
#define _MNL_CBS_TEST_

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

typedef struct mnl_req_test_s mnl_req_test_t;
struct mnl_req_test_s {
  char *md5;
  thr_arg_t *thr_arg;
};

void mnl_cbs_test_post(evhtp_request_t *req, void *arg);

#endif
