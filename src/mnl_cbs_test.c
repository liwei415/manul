#include "mnl_cbs_test.h"

static const char *method_strmap[] = {
  "GET",
  "HEAD",
  "POST",
  "PUT",
  "DELETE",
  "MKCOL",
  "COPY",
  "MOVE",
  "OPTIONS",
  "PROPFIND",
  "PROPATCH",
  "LOCK",
  "UNLOCK",
  "TRACE",
  "CONNECT",
  "PATCH",
  "UNKNOWN",
};

static const char *post_error_list[] = {
  "Internal error.",
  "File type not support.",
  "Request method error.",
  "Access error.",
  "Request body parse error.",
  "Content-Length error.",
  "Content-Type error.",
  "File too large.",
  "Request url illegal.",
  "Image not existed.",
  "Delete resource failed.",
  "Wrong input params.",
  "Wrong Json Node."
};

int _jreturn(evhtp_request_t *req, int err_no, mnl_res_test_t *mnl_res)
{
  //json sample:
  cJSON *j_ret = cJSON_CreateObject();
  cJSON *j_ret_info = cJSON_CreateObject();
  if(err_no == -1) {
    cJSON_AddBoolToObject(j_ret, "ret", 1);
    cJSON_AddStringToObject(j_ret_info, "md5", mnl_res->test);
    cJSON_AddItemToObject(j_ret, "info", j_ret_info);
  }
  else {
    cJSON_AddBoolToObject(j_ret, "ret", 0);
    cJSON_AddNumberToObject(j_ret_info, "code", err_no);
    LOG_PRINT(LOG_DEBUG, "post_error_list[%d]: %s", err_no, post_error_list[err_no]);
    cJSON_AddStringToObject(j_ret_info, "message", post_error_list[err_no]);
    cJSON_AddItemToObject(j_ret, "error", j_ret_info);
  }
  char *ret_str_unformat = cJSON_PrintUnformatted(j_ret);
  LOG_PRINT(LOG_DEBUG, "ret_str_unformat: %s", ret_str_unformat);
  evbuffer_add_printf(req->buffer_out, "%s", ret_str_unformat);
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Content-Type", "application/json", 0, 0));

  cJSON_Delete(j_ret);
  free(ret_str_unformat);

  return 0;
}

void mnl_cbs_test_post(evhtp_request_t *req, void *arg)
{
  char *md5 = NULL;
  char *buff = NULL;
  int post_size = 0;
  int err_no = 0;

  mnl_req_test_t *mnl_req = NULL;
  mnl_res_test_t *mnl_res = NULL;

  evhtp_connection_t *ev_conn = evhtp_request_get_connection(req);
  struct sockaddr *saddr = ev_conn->saddr;
  struct sockaddr_in *ss = (struct sockaddr_in *)saddr;
  char address[16];

  int req_method = evhtp_request_get_method(req);
  if(req_method >= 16) {
    req_method = 16;
  }

  LOG_PRINT(LOG_DEBUG, "Method: %d", req_method);
  if(strcmp(method_strmap[req_method], "POST") != 0) {
    LOG_PRINT(LOG_DEBUG, "Request Method Not Support.");
    err_no = 2;
    goto err;
  }

  const char *xff_address = evhtp_header_find(req->headers_in, "X-Forwarded-For");
  if(xff_address) {
    inet_aton(xff_address, &ss->sin_addr);
  }
  else {
    inet_aton("0.0.0.0", &ss->sin_addr);
  }
  strncpy(address, inet_ntoa(ss->sin_addr), 16);

  if (vars.access != NULL) {
    int acs = mnl_access_inet(vars.access, ss->sin_addr.s_addr);
    LOG_PRINT(LOG_DEBUG, "access check: %d", acs);
    if(acs == MNL_FORBIDDEN) {
      LOG_PRINT(LOG_DEBUG, "check access: ip[%s] forbidden!", address);
      LOG_PRINT(LOG_INFO, "%s refuse post forbidden", address);
      err_no = 3;
      goto forbidden;
    }
    else if (acs == MNL_ERROR) {
      LOG_PRINT(LOG_DEBUG, "check access: check ip[%s] failed!", address);
      LOG_PRINT(LOG_ERROR, "%s fail post access %s", address);
      err_no = 0;
      goto err;
    }
  }

  const char *content_len = evhtp_header_find(req->headers_in, "Content-Length");
  if(!content_len) {
    LOG_PRINT(LOG_DEBUG, "Get Content-Length error!");
    LOG_PRINT(LOG_ERROR, "%s fail post content-length", address);
    err_no = 5;
    goto err;
  }
  post_size = atoi(content_len);
  if(post_size <= 0) {
    LOG_PRINT(LOG_DEBUG, "Request data size is zero!");
    LOG_PRINT(LOG_ERROR, "%s fail post empty", address);
    err_no = 5;
    goto err;
  }
  if(post_size > vars.max_size) {
    LOG_PRINT(LOG_DEBUG, "Request data size too large!");
    LOG_PRINT(LOG_ERROR, "%s fail post large", address);
    err_no = 7;
    goto err;
  }
  evbuf_t *buf;
  buf = req->buffer_in;
  buff = (char *)calloc(post_size + 1, sizeof(char));
  if(buff == NULL) {
    LOG_PRINT(LOG_DEBUG, "buff malloc failed!");
    LOG_PRINT(LOG_ERROR, "%s fail malloc buff", address);
    err_no = 0;
    goto err;
  }
  int rmblen, evblen;
  if(evbuffer_get_length(buf) <= 0) {
    LOG_PRINT(LOG_DEBUG, "Empty Request!");
    LOG_PRINT(LOG_ERROR, "%s fail post empty", address);
    err_no = 4;
    goto err;
  }
  while((evblen = evbuffer_get_length(buf)) > 0) {
    LOG_PRINT(LOG_DEBUG, "evblen = %d", evblen);
    rmblen = evbuffer_remove(buf, buff, evblen);
    LOG_PRINT(LOG_DEBUG, "rmblen = %d", rmblen);
    if(rmblen < 0) {
      LOG_PRINT(LOG_DEBUG, "evbuffer_remove failed!");
      LOG_PRINT(LOG_ERROR, "%s fail post parse", address);
      err_no = 4;
      goto err;
    }
  }

  evthr_t *thread = mnl_cbs_get_request_thr(req);
  thr_arg_t *thr_arg = (thr_arg_t *)evthr_get_aux(thread);

  mnl_req = (mnl_req_test_t *)calloc(1, sizeof(mnl_req_test_t));
  mnl_req->thr_arg = thr_arg;

  // 处理json输入
  cJSON *root, *chd;
  if((root = cJSON_Parse(buff)) == NULL) {
    err_no = 11;
    goto err;
  }

  chd = cJSON_GetObjectItem(root, "md5");
  if (chd == NULL) {
    err_no = 12;
    goto jerr;
  }
  md5 = calloc(strlen(chd->valuestring) + 1, sizeof(char));
  mnl_strlcpy(md5, chd->valuestring, strlen(chd->valuestring) + 1);
  mnl_req->md5 = md5;

  // 业务逻辑处理
  // 1.build 请求包

  // 2.远端服务器交互
  //  int sockfd = mnl_net_conn(vars.remote_ip, vars.remote_port);

  // 3.dissect 返回包


  mnl_res = (mnl_res_test_t *)calloc(1, sizeof(mnl_res_test_t));
  mnl_res->test = calloc(32, sizeof(char));
  strcpy(mnl_res->test, "helloaaaa!");


  //业务逻辑处理成功
  err_no = -1;
  _jreturn(req, -1, mnl_res);
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Server", vars.server_name, 0, 1));
  evhtp_send_reply(req, EVHTP_RES_OK);
  LOG_PRINT(LOG_DEBUG, "============test DONE!===============");
  goto jdone;

 forbidden:
  _jreturn(req, err_no, NULL);
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Server", vars.server_name, 0, 1));
  evhtp_send_reply(req, EVHTP_RES_OK);
  LOG_PRINT(LOG_DEBUG, "============test FORBIDDEN!===============");
  goto done;

 jerr:
  cJSON_Delete(root);

 err:
  _jreturn(req, err_no, NULL);
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Server", vars.server_name, 0, 1));
  evhtp_send_reply(req, EVHTP_RES_OK);
  LOG_PRINT(LOG_DEBUG, "============test ERROR!===============");
  goto done;

 jdone:
  cJSON_Delete(root);

 done:
  free(buff);
  free(md5);
  free(mnl_req);
}
