#include "sq_packet.h"

void sampquery_make_pkt(char data[SAMPQUERY_REQUEST_LEN], sampquery_header_t header, unsigned char opcode)
{
    const char *ipaddres = inet_ntoa(header.sock_address.sin_addr);
    const unsigned short port = header.sock_address.sin_port;

    unsigned char *ip_bytes = (unsigned char *)&header.sock_address.sin_addr.s_addr;

    data[0] = 'S';
    data[1] = 'A';
    data[2] = 'M';
    data[3] = 'P';

    data[4] = ip_bytes[0];
    data[5] = ip_bytes[1];
    data[6] = ip_bytes[2];
    data[7] = ip_bytes[4];

    data[8] = (unsigned char)(port & 0xFF);
    data[9] = (unsigned char)(port >> 8 & 0xFF);

    data[10] = opcode;
}
