#include "mnl_cbs.h"

void mnl_cbs_headers_free(mnl_cbs_headers_conf_t *hcf)
{
  if(hcf == NULL) {
    return;
  }
  mnl_cbs_headers_t *headers = hcf->headers;
  while(headers) {
    hcf->headers = headers->next;
    free(headers->value);
    free(headers);
    headers = hcf->headers;
  }
  free(hcf);
}

mnl_cbs_headers_conf_t * mnl_cbs_get_headers_conf(const char *hdr_str)
{
  if(hdr_str == NULL) {
    return NULL;
  }
  mnl_cbs_headers_conf_t *hdrconf = (mnl_cbs_headers_conf_t *)malloc(sizeof(mnl_cbs_headers_conf_t));
  if(hdrconf == NULL) {
    return NULL;
  }
  hdrconf->n = 0;
  hdrconf->headers = NULL;
  size_t hdr_len = strlen(hdr_str);
  char *hdr = (char *)malloc(hdr_len);
  if(hdr == NULL) {
    return NULL;
  }
  strncpy(hdr, hdr_str, hdr_len);
  char *start = hdr, *end;
  while(start <= hdr+hdr_len) {
    end = strchr(start, ';');
    end = (end) ? end : hdr + hdr_len;
    char *key = start;
    char *value = strchr(key, ':');
    size_t key_len = value - key;
    if (value) {
      mnl_cbs_header_t *this_header = (mnl_cbs_header_t *)malloc(sizeof(mnl_cbs_header_t));
      if (this_header == NULL) {
        start = end + 1;
        continue;
      }
      (void) memset(this_header, 0, sizeof(mnl_cbs_header_t));
      size_t value_len;
      value++;
      value_len = end - value;

      strncpy(this_header->key, key, key_len);
      strncpy(this_header->value, value, value_len);

      mnl_cbs_headers_t *headers = (mnl_cbs_headers_t *)malloc(sizeof(mnl_cbs_headers_t));
      if (headers == NULL) {
        start = end + 1;
        continue;
      }

      headers->value = this_header;
      headers->next = hdrconf->headers;
      hdrconf->headers = headers;
      hdrconf->n++;
    }
    start = end + 1;
  }
  free(hdr);
  return hdrconf;
}

int mnl_cbs_headers_add(evhtp_request_t *req, mnl_cbs_headers_conf_t *hcf)
{
  if(hcf == NULL) {
    return -1;
  }
  mnl_cbs_headers_t *headers = hcf->headers;
  LOG_PRINT(LOG_DEBUG, "headers: %d", hcf->n);

  while(headers) {
    evhtp_headers_add_header(req->headers_out,
                             evhtp_header_new(headers->value->key,
                                              headers->value->value, 1, 1));
    headers = headers->next;
  }
  return 1;
}

evthr_t *mnl_cbs_get_request_thr(evhtp_request_t *request)
{
  evhtp_connection_t *htpconn;
  evthr_t *thread;

  htpconn = evhtp_request_get_connection(request);
  thread  = htpconn->thread;

  return thread;
}

void mnl_cbs_index(evhtp_request_t *req, void *arg)
{
  evbuffer_add_printf(req->buffer_out, "<html><body><h1>MNL works!</h1></body></html>");
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Server", vars.server_name, 0, 1));
  evhtp_headers_add_header(req->headers_out, evhtp_header_new("Content-Type", "text/html", 0, 0));
  evhtp_send_reply(req, EVHTP_RES_OK);
}
