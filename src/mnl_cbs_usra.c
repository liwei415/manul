#include "mnl_cbs_usra.h"

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
  "Wrong Json Node.",
};

int _cbs_usra_build(char *buf, mnl_req_usra_t *mnl_req) {

  cJSON *root, *chd;
  if((root = cJSON_Parse(buf)) == NULL) {
    goto err;
  }

  chd = cJSON_GetObjectItem(root, "uid");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_int(mnl_req->uid, chd->valuestring, 20);

  chd = cJSON_GetObjectItem(root, "username");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_hex(mnl_req->username, chd->valuestring, 160);

  chd = cJSON_GetObjectItem(root, "type");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->type, chd->valuestring, 1);

  chd = cJSON_GetObjectItem(root, "cert");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->cert, chd->valuestring, 3);

  chd = cJSON_GetObjectItem(root, "cert_no");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->cert_no, chd->valuestring, 30);

  chd = cJSON_GetObjectItem(root, "mobile");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->mobile, chd->valuestring, 11);

  chd = cJSON_GetObjectItem(root, "spare");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->spare, chd->valuestring, 200);

  // 包长度处理
  char plen[8];
  sprintf(plen, "%d", (int)sizeof(mnl_req_usra_t));

  mnl_cpy_str(mnl_req->flag, "CMBA", 4);
  mnl_cpy_int(mnl_req->len, plen, 4);
  mnl_cpy_str(mnl_req->biz, "USRA", 4);
  mnl_cpy_str(mnl_req->bank_date, " ", 8);
  mnl_cpy_str(mnl_req->bank_time, " ", 6);
  mnl_cpy_str(mnl_req->bank_no, " ", 30);

  chd = cJSON_GetObjectItem(root, "plat_date");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->plat_date, chd->valuestring, 8);

  chd = cJSON_GetObjectItem(root, "plat_time");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->plat_time, chd->valuestring, 6);

  chd = cJSON_GetObjectItem(root, "plat_no");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->plat_no, chd->valuestring, 30);

  chd = cJSON_GetObjectItem(root, "plat_code");
  if (chd == NULL) {
    goto jerr;
  }
  mnl_cpy_str(mnl_req->plat_code, chd->valuestring, 10);

  mnl_cpy_str(mnl_req->comm_code, " ", 7);

  char tt[16] = {0};
  mnl_mac_enc("0393BF6CE2353175", tt, "9999999999999999");
  LOG_PRINT(LOG_DEBUG, "ttttttttttttttttttttttttttt: %s", tt);

  goto done;

 jerr:
  cJSON_Delete(root);
  return -12;

 err:
  return -11;

 done:
  cJSON_Delete(root);
  return 0;
}

int _cbs_usra_dissect(char *buf, mnl_res_usra_t *mnl_res) {
  return 1;
}

int _cbs_usra_return(evhtp_request_t *req, int err_no, mnl_res_usra_t *mnl_res)
{
  //json sample:
  cJSON *j_ret = cJSON_CreateObject();
  cJSON *j_ret_info = cJSON_CreateObject();
  if(err_no == -1) {
    cJSON_AddBoolToObject(j_ret, "ret", 1);
    cJSON_AddStringToObject(j_ret_info, "code", mnl_res->code);
    cJSON_AddStringToObject(j_ret_info, "uid", mnl_res->uid);
    cJSON_AddStringToObject(j_ret_info, "tuid", mnl_res->tuid);
    cJSON_AddStringToObject(j_ret_info, "desc", mnl_res->desc);
    cJSON_AddStringToObject(j_ret_info, "spare", mnl_res->spare);
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

void mnl_cbs_usra_post(evhtp_request_t *req, void *arg)
{
  char *buff = NULL;
  int post_size = 0;
  int err_no = 0;

  mnl_req_usra_t *mnl_req = NULL;
  mnl_res_usra_t *mnl_res = NULL;

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

  // 业务逻辑处理

  // 1.build 请求包
  mnl_req = (mnl_req_usra_t *)calloc(1, sizeof(mnl_req_usra_t));

  int jerr = _cbs_usra_build(buff, mnl_req);
  if (jerr != 0) {
    err_no = jerr;
    goto err;
  }

  LOG_PRINT(LOG_DEBUG, "============input: %s===============", mnl_req->flag);

  // 2.远端服务器交互
  //  int sockfd = mnl_net_conn(vars.remote_ip, vars.remote_port);

  // 3.dissect 返回包


  mnl_res = (mnl_res_usra_t *)calloc(1, sizeof(mnl_res_usra_t));

  //业务逻辑处理成功
  err_no = -1;
  _cbs_usra_return(req, -1, mnl_res);
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Server", vars.server_name, 0, 1));
  evhtp_send_reply(req, EVHTP_RES_OK);
  LOG_PRINT(LOG_DEBUG, "============usra DONE!===============");
  goto done;

 forbidden:
  _cbs_usra_return(req, err_no, NULL);
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Server", vars.server_name, 0, 1));
  evhtp_send_reply(req, EVHTP_RES_OK);
  LOG_PRINT(LOG_DEBUG, "============usra FORBIDDEN!===============");
  goto done;

 err:
  _cbs_usra_return(req, err_no, NULL);
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Server", vars.server_name, 0, 1));
  evhtp_send_reply(req, EVHTP_RES_OK);
  LOG_PRINT(LOG_DEBUG, "============usra ERROR!===============");
  goto done;

 done:
  free(buff);
  free(mnl_req);
  free(mnl_res);
}
