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

  return 0
}

int mnl_net_wt(int sockfd, char *cmd, char *res_buf)
{
  /* // Send cmd */
  /* // 2 terminators */
  /* uint16_t out_buf_len = sizeof(atm_mtpe_data_packet_t) - CMD_LEN + strlen(cmd) + 2; */

  /* char out_buf[out_buf_len]; */
  /* size_t out_size = atm_mtpe_build_data_packet(c, cmd, out_buf); */
  /* if (send(c->sockfd, out_buf, out_size, MSG_NOSIGNAL)) { */
  /*   // Response */
  /*   if (atm_net_tcp_rd(c, res_buf) == 0) { */
  /*     char *e = strrchr(res_buf, '\r'); */
  /*     /\* Create start and end for res_buf *\/ */
  /*     if (e != NULL) { */
  /*       *(e+1) = '\0'; */
  /*     } */
  /*   } */
  /*   else { */
  /*     strcpy(res_buf, "RD FAILED!"); */
  /*     perror("data tcp rd failed"); */
  /*     c->err = -2; */
  /*   } */
  /* } */
  /* else { */
  /*   strcpy(res_buf, "WT FAILED!"); */
  /*   perror("data tcp wt failed"); */
  /*   c->err = -1; */
  /* } */

  return 0;
}

int mnl_net_rd(int sockfd, char *buf)
{
  /* int recv_l = 0, recv_n = 0, recv_c = 0; */
  /* char t_buf[32]; */
  /* int bts; */

  /* /\* 验证数据包包头 *\/ */
  /* while (1) { */
  /*   memset(t_buf, 0, 32); */
  /*   /\* Get 4bytes. Count length *\/ */
  /*   bts = recv(sockfd, t_buf, 18, MSG_NOSIGNAL); */
  /*   if (bts > 0) { */
  /*     recv_l = (t_buf[2] << 8) + (t_buf[3] & 0xff); */
  /*   } */
  /*   else { */
  /*     return -1; */
  /*   } */

  /*   if ((unsigned char)t_buf[0] == 0x01 && (unsigned char)t_buf[1] == 0x00) { */
  /*     break; */
  /*   } */
  /* } */

  /* recv_l -= 18; */
  /* if (recv_l < 0) { */
  /*   return -1; */
  /* } */

  /* while (recv_l > 0 && recv_c < RES_LEN) { */
  /*   recv_n = recv(sockfd, buf+recv_c, recv_l, MSG_NOSIGNAL); */
  /*   if (recv_n) { */
  /*     recv_l -= recv_n; */
  /*     recv_c += recv_n; */
  /*   } */
  /*   else { */
  /*     return -1; */
  /*   } */
  /* } */

  return 0;
}
