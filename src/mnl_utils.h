#ifndef _MNL_UTILS_
#define _MNL_UTILS_

#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#include "mnl_log.h"
#include "mnl_string.h"
#include "mnl_common.h"

int mnl_file_type(const char *filename, char *type);
void mnl_get_file_path(const char *path, const char *file_name, char *file_path);
int mnl_isfile(const char *filename);
int mnl_isimg(const char *filename);
int mnl_isdoc(const char *filename);
int mnl_ismov(const char *filename);
int mnl_isdir(const char *path);
int mnl_issdir(const char *path);
int mnl_mklock(const char *path, char *passwd);
int mnl_rm(const char *path);
int mnl_mkdir(const char *path);
int mnl_mkdirs(const char *dir);
int mnl_mkdirf(const char *filename);
int mnl_ismd5(char *s);
int mnl_strhash(const char *str);
int mnl_genkey(char *key, char *md5, ...);

#endif
