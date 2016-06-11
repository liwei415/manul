#ifndef _MNL_NET_H_
#define _MNL_NET_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "mnl_common.h"

int mnl_net_conn(char *serv, int port);
int mnl_net_rd(int sockfd, char *buf);
int mnl_net_disconn(int sockfd);

#endif
