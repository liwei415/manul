#include "mnl_net.h"

int mnl_net_conn(char *serv, int port) {

  int sockfd, error;
  struct hostent *host;
  struct sockaddr_in serv_addr;

  /* 30秒无结果断开 */
  struct timeval timeout = {30, 0};

  host = gethostbyname(serv);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

  if (host == NULL) {
    LOG_PRINT(LOG_DEBUG, "gethostbyname failed");
    sockfd = -1;
  }
  else if (sockfd == -1) {
    LOG_PRINT(LOG_DEBUG, "socket failed");
    sockfd = -1;
  }
  else {
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(serv_addr.sin_zero), 8);

    error = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));

    if (error == -1) {
      LOG_PRINT(LOG_DEBUG, "connect failed");
      sockfd = -1;
    }
  }

  return sockfd;
}

int mnl_net_disconn(sockfd)
{
  close(sockfd);

  return 0;
}

int mnl_net_wt(int sockfd, char *ibuf, int ilen)
{

  int r = send(sockfd, ibuf, ilen, MSG_NOSIGNAL);
  LOG_PRINT(LOG_DEBUG, "-----sending : %d, %s-----------<%d", ilen, ibuf, r);
  if (!r) {
    return -1;
  }
  return 0;
}

int mnl_net_rd(int sockfd, char *obuf, int olen)
{

  // 招行做得比较简单，长度不足补空格，所以直接取长度，不用管包头标识长度，作为客户端，不校验mac（发送包校验）
  if (!recv(sockfd, obuf, olen, MSG_NOSIGNAL)) {
    return -1;
  }
  return 0;
}
