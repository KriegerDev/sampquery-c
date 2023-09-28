#ifndef _SAMPQUERY_HEADER_H_
#define _SAMPQUERY_HEADER_H_
#include "internal/sq_net.h"

typedef struct sampquery_header
{
    struct sockaddr_in sock_address;
    socklen_t sock_len;
    int sock_fd;
} sampquery_header_t;

int sampquery_setup_header(sampquery_header_t *sq_header, const char *sq_hostname, const unsigned short sq_port);

#endif