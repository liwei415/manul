#include "mnl.h"

mnl_vars_t vars;

static void vars_init(void)
{
  vars.is_daemon = 0;
  mnl_strlcpy(vars.ip, "0.0.0.0", sizeof(vars.ip));
  vars.port = 9527;
  vars.num_threads = 2;         /* N workers */
  vars.backlog = 1024;
  vars.max_keepalives = 1;
  snprintf(vars.server_name, 128, "MNL image server/v%s", vars.version);
  vars.log_level = 7;
  mnl_strlcpy(vars.version, MNL_VERSION, sizeof(vars.version));
  mnl_strlcpy(vars.log_path, "./mnl.log", sizeof(vars.log_path));
  mnl_strlcpy(vars.root_path, "./index.html", sizeof(vars.root_path));
  vars.save_new = 1;
  vars.max_size = 10485760;
  vars.headers = NULL;
  vars.access = NULL;
}

static int load_conf(const char *conf)
{
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  if (luaL_loadfile(L, conf) || lua_pcall(L, 0, 0, 0)) {
    lua_close(L);
    return -1;
  }

  lua_getglobal(L, "is_daemon"); //stack index: -12
  if(lua_isnumber(L, -1)) {
    vars.is_daemon = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  lua_getglobal(L, "ip");
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.ip, lua_tostring(L, -1), sizeof(vars.ip));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "port");
  if(lua_isnumber(L, -1)) {
    vars.port = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  lua_getglobal(L, "thread_num");
  if(lua_isnumber(L, -1)) {
    vars.num_threads = (int)lua_tonumber(L, -1);         /* N workers */
  }
  lua_pop(L, 1);

  lua_getglobal(L, "backlog_num");
  if(lua_isnumber(L, -1)) {
    vars.backlog = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  lua_getglobal(L, "max_keepalives");
  if(lua_isnumber(L, -1)) {
    vars.max_keepalives = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  /* lua_getglobal(L, "retry"); */
  /* if(lua_isnumber(L, -1)) { */
  /*   vars.retry = (int)lua_tonumber(L, -1); */
  /* } */
  /* lua_pop(L, 1); */

  lua_getglobal(L, "system");
  if(lua_isstring(L, -1)) {
    char tmp[128];
    snprintf(tmp, 128, "%s %s", vars.server_name, lua_tostring(L, -1));
    snprintf(vars.server_name, 128, "%s", tmp);
  }
  lua_pop(L, 1);

  lua_getglobal(L, "headers");
  if(lua_isstring(L, -1)) {
    vars.headers = mnl_cbs_get_headers_conf(lua_tostring(L, -1));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "access_rule");
  if(lua_isstring(L, -1)) {
    vars.access = mnl_access_get(lua_tostring(L, -1));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "log_level");
  if(lua_isnumber(L, -1)) {
    vars.log_level = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  lua_getglobal(L, "log_path"); //stack index: -1
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.log_path, lua_tostring(L, -1), sizeof(vars.log_path));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "root_path");
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.root_path, lua_tostring(L, -1), sizeof(vars.root_path));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "max_size");
  if(lua_isnumber(L, -1)) {
    vars.max_size = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  lua_getglobal(L, "remote_ip");
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.remote_ip, lua_tostring(L, -1), sizeof(vars.remote_ip));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "remote_port");
  if(lua_isnumber(L, -1)) {
    vars.remote_port = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  lua_getglobal(L, "plat_code");
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.plat_code, lua_tostring(L, -1), sizeof(vars.plat_code));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "plat_key");
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.plat_key, lua_tostring(L, -1), sizeof(vars.plat_key));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "ftp_user");
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.ftp_user, lua_tostring(L, -1), sizeof(vars.ftp_user));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "ftp_pass");
  if(lua_isstring(L, -1)) {
    mnl_strlcpy(vars.ftp_pass, lua_tostring(L, -1), sizeof(vars.ftp_pass));
  }
  lua_pop(L, 1);

  lua_getglobal(L, "ftp_port");
  if(lua_isnumber(L, -1)) {
    vars.ftp_port = (int)lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  vars.L = L;
  //lua_close(L);

  return 1;
}

void init_thread(evhtp_t *htp, evthr_t *thread, void *arg)
{
  thr_arg_t *thr_args;
  thr_args = calloc(1, sizeof(thr_arg_t));
  LOG_PRINT(LOG_DEBUG, "thr_args alloc");
  thr_args->thread = thread;

  thr_args->L = luaL_newstate();
  LOG_PRINT(LOG_DEBUG, "luaL_newstate alloc");
  if(thr_args->L != NULL) {
    luaL_openlibs(thr_args->L);
  }
  lua_pcall(thr_args->L, 0, 0, 0);

  evthr_set_aux(thread, thr_args);
}

int main()
{
  vars_init();
  const char *conf_file = "mnl.lua";
  if(load_conf(conf_file) == -1) {
    fprintf(stderr, "'%s' load failed!\n", conf_file);
    return -1;
  }

  if(vars.is_daemon == 1) {
    if(daemon(1, 1) < 0) {
      fprintf(stderr, "Create daemon failed!\n");
      return -1;
    }
    else {
      fprintf(stdout, "mnl %s\n", vars.version);
      fprintf(stdout, "Copyright (c) 2015-2016 l.inux.xyz\n");
      fprintf(stderr, "\n");
    }
  }

  if(mnl_mkdirf(vars.log_path) != 1) {
    fprintf(stderr, "%s log path create failed!\n", vars.log_path);
    return -1;
  }
  mnl_log_init();

  LOG_PRINT(LOG_DEBUG,"Paths Init Finished.");

  //begin to start httpd...
  LOG_PRINT(LOG_DEBUG, "Begin to Start Httpd Server...");
  LOG_PRINT(LOG_INFO, "mnl started");

  // httpd
  evbase_t *evbase = event_base_new();
  evhtp_t *htp = evhtp_new(evbase, NULL);

  // index
  evhtp_set_cb(htp, "/index", mnl_cbs_index, NULL);

  // test
  evhtp_set_cb(htp, "/test", mnl_cbs_test_post, NULL);

  // 用户开户 USRA
  evhtp_set_cb(htp, "/usra", mnl_cbs_usra_post, NULL);

  // all other
  evhtp_set_gencb(htp, mnl_cbs_index, NULL);

#ifndef EVHTP_DISABLE_EVTHR
  evhtp_use_threads_wexit(htp, init_thread, NULL, vars.num_threads, NULL);
#endif

  evhtp_set_max_keepalive_requests(htp, vars.max_keepalives);
  evhtp_bind_socket(htp, vars.ip, vars.port, vars.backlog);

  event_base_loop(evbase, 0);

  evhtp_unbind_socket(htp);
  evhtp_free(htp);
  event_base_free(evbase);
  mnl_cbs_headers_free(vars.headers);
  mnl_access_free(vars.access);

  return 0;
}
