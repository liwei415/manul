#include "mnl_utils.h"

int mnl_file_type(const char *filename, char *type)
{
  char *flag, *tmp;
  if((flag = strchr(filename, '.')) == 0) {
    LOG_PRINT(LOG_DEBUG, "FileName [%s] Has No '.' in It.", filename);
    return -1;
  }

  while((tmp = strchr(flag + 1, '.')) != 0) {
    flag = tmp;
  }
  flag++;
  mnl_strlcpy(type, flag, 32);
  return 1;
}

int mnl_isfile(const char *filename)
{
  struct stat st;
  if(stat(filename, &st) < 0) {
    LOG_PRINT(LOG_DEBUG, "File [%s] is Not Existed!", filename);
    return -1;
  }
  if(S_ISREG(st.st_mode)) {
    LOG_PRINT(LOG_DEBUG, "File[%s] is A File.", filename);
    return 1;
  }
  return -1;
}

int mnl_isdir(const char *path)
{
  struct stat st;
  if(stat(path, &st) < 0) {
    LOG_PRINT(LOG_DEBUG, "Path[%s] is Not Existed!", path);
    return -1;
  }
  if(S_ISDIR(st.st_mode)) {
    LOG_PRINT(LOG_DEBUG, "Path[%s] is A Dir.", path);
    return 1;
  }
  else {
    return -1;
  }
}

int mnl_issdir(const char *path)
{
  if (strcmp(path, ".") == 0 || strcmp(path, "..") == 0) {
    return 1;
  }
  else {
    return -1;
  }
}

void mnl_get_file_path(const char *path, const char *file_name, char *file_path)
{
  strcpy(file_path, path);
  if(file_path[strlen(path) - 1] != '/') {
    mnl_strlcat(file_path, "/", PATH_MAX_SIZE);
  }
  mnl_strlcat(file_path, file_name, PATH_MAX_SIZE);
}

int mnl_rm(const char *path)
{
  DIR *dir;
  struct dirent *dir_info;
  char file_path[PATH_MAX_SIZE];
  int ret = -1;
  if(mnl_isfile(path) == 1) {
    remove(path);
    ret = 1;
  }
  if(mnl_isdir(path) == 1) {
    if((dir = opendir(path)) == NULL) {
      return ret;
    }
    ret = 1;
    while((dir_info = readdir(dir)) != NULL) {
      mnl_get_file_path(path, dir_info->d_name, file_path);
      if(mnl_issdir(dir_info->d_name) == 1) {
        continue;
      }
      ret = mnl_rm(file_path);
      if(ret == -1) {
        break;
      }
    }
    if(ret == 1) {
      ret = rmdir(path);
    }
  }
  return ret;
}

int mnl_mkdir(const char *path)
{
  if(access(path, 0) == -1) {
    int status = mkdir(path, 0755);
    if(status == -1) {
      LOG_PRINT(LOG_DEBUG, "mkdir[%s] Failed!", path);
      return -1;
    }
    LOG_PRINT(LOG_DEBUG, "mkdir[%s] sucessfully!", path);
    return 1;
  }
  else {
    LOG_PRINT(LOG_DEBUG, "Path[%s] is Existed!", path);
    return -1;
  }
}

int mnl_mkdirs(const char *dir)
{
  char tmp[256];
  mnl_strlcpy(tmp, dir, sizeof(tmp));
  int i, len = strlen(tmp);
  if(tmp[len-1] != '/') {
    mnl_strlcat(tmp, "/", sizeof(tmp));
  }

  len = strlen(tmp);
  for(i=1; i<len; i++) {
    if(tmp[i] == '/') {
      tmp[i] = 0;
      if(access(tmp, 0) != 0) {
        if(mkdir(tmp, 0755) == -1) {
          fprintf(stderr, "mnl_mkdirs: tmp=%s\n", tmp);
          return -1;
        }
      }
      tmp[i] = '/';
    }
  }
  return 1;
}

int mnl_mkdirf(const char *filename)
{
  int ret = 1;
  if (access(filename, 0) == 0) {
    return ret;
  }
  size_t len = strlen(filename);
  char str[256];
  mnl_strlcpy(str, filename, len);
  str[len] = '\0';
  char *end = str;
  char *start = strchr(end, '/');
  while (start) {
    end = start + 1;
    start = strchr(end, '/');
  }
  if (end != str) {
    str[end-str] = '\0';
    ret = mnl_mkdirs(str);
  }
  return ret;
}
