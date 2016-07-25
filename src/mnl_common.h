#ifndef _MNL_COMMON_
#define _MNL_COMMON_

#include <stdio.h>
#include <string.h>
#include <lualib.h>
#include <evhtp/evhtp.h>

#include "mnl_log.h"

#ifndef MNL_VERSION
#define MNL_VERSION "1.0.0"
#endif

#define MAX_LINE            1024
#define CACHE_MAX_SIZE      1048576 //1024*1024
#define RETRY_TIME_WAIT     1000
#define CACHE_KEY_SIZE      128
#define PATH_MAX_SIZE       512

typedef struct thr_arg_s thr_arg_t;
struct thr_arg_s {
  evthr_t *thread;
  lua_State* L;
};

typedef struct mnl_cbs_header_s mnl_cbs_header_t;
struct mnl_cbs_header_s {
  char key[128];
  char value[512];
};

typedef struct mnl_cbs_headers_s mnl_cbs_headers_t;
struct mnl_cbs_headers_s {
  mnl_cbs_header_t *value;
  mnl_cbs_headers_t *next;
};

typedef struct mnl_cbs_headers_conf_s mnl_cbs_headers_conf_t;
struct mnl_cbs_headers_conf_s {
  uint n;
  mnl_cbs_headers_t *headers;
};

// access
typedef struct mnl_access_rule_s mnl_access_rule_t;
struct mnl_access_rule_s {
  in_addr_t mask;
  in_addr_t addr;
  uint deny;	/* unsigned  deny:1; */
};

typedef struct mnl_access_rules_s mnl_access_rules_t;
struct mnl_access_rules_s {
    mnl_access_rule_t *value;
    mnl_access_rules_t *next;
};

typedef struct mnl_access_conf_s mnl_access_conf_t;
struct mnl_access_conf_s {
  uint n;
  mnl_access_rules_t *rules;
};

typedef struct mnl_vars_s mnl_vars_t;
struct mnl_vars_s {
  lua_State *L;
  int is_daemon;
  char ip[128];
  int port;
  int num_threads;
  int backlog;
  int max_keepalives;
  char version[128];
  char server_name[128];
  int log_level;
  char log_path[512];
  char root_path[512];
  int mode;
  int save_new;
  int max_size;
  mnl_cbs_headers_conf_t *headers;
  mnl_access_conf_t *access;
  char remote_ip[128];
  int remote_port;
  char plat_code[32];
  char plat_key[32];
  char ftp_user[32];
  char ftp_pass[32];
  int ftp_port;
};

extern mnl_vars_t vars;
pthread_key_t thread_key;

#define LOG_FATAL       0           /* System is unusable */
#define LOG_ALERT       1           /* Action must be taken immediately */
#define LOG_CRIT        2           /* Critical conditions */
#define LOG_ERROR       3           /* Error conditions */
#define LOG_WARNING     4           /* Warning conditions */
#define LOG_NOTICE      5           /* Normal, but significant */
#define LOG_INFO        6           /* Information */
#define LOG_DEBUG       7           /* DEBUG message */

#ifdef DEBUG
#define LOG_PRINT(level, fmt, ...)                      \
  do {                                                  \
    int log_id = mnl_log_open(vars.log_path, "a");          \
    mnl_log_printf0(log_id, level, "%s:%d %s() "fmt,        \
                   __FILE__, __LINE__, __FUNCTION__,       \
                   ##__VA_ARGS__);                         \
    mnl_log_close(log_id);                                     \
  } while (0)
#else
#define LOG_PRINT(level, fmt, ...)                                      \
  do {                                                                  \
    if (level <= vars.log_level) {                                      \
      int log_id = mnl_log_open(vars.log_path, "a");                        \
      mnl_log_printf0(log_id, level, fmt, ##__VA_ARGS__) ;                  \
      mnl_log_close(log_id);                                                \
    }                                                                   \
  } while (0)
#endif

#endif
