#include "sampquery.h"

int sampquery_setup_header(sampquery_header_t *sq_header, const char *sq_ipaddress, const unsigned short sq_port)
{
    memset(&sq_header->sock_address, 0, sizeof(sq_header->sock_address));
    sq_header->sock_address.sin_family = AF_INET;
    sq_header->sock_address.sin_port = htons(sq_port);

    int result = inet_pton(AF_INET, sq_ipaddress, &sq_header->sock_address.sin_addr);
    if (result == 1)
    {
#ifdef _SAMPQUERY_DEBUG_
        puts("[SAMPQUERY LOG] Successfully setup sock_address");
#endif
    }
    else if (result == 0)
    {
        const char *fmt = "[SAMPQUERY LOG] sampquery_setup_header: Error on setup header - %s\n";
        printf(fmt, strerror(errno));
        return 0;
    }
    else
    {
        const char *fmt = "[SAMPQUERY LOG] sampquery_setup_header: Error on setup header - %s\n";
        printf(fmt, strerror(errno));
        return 0;
    }

    sq_header->sock_len = sizeof(sq_header->sock_address);
    sq_header->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sq_header->sock_fd < 0)
    {
        const char *fmt = "[SAMPQUERY LOG] sampquery_setup_header: Error on setup socket - %s";
        printf(fmt, strerror(errno));
        return 0;
    }
    else
    {
#ifdef _SAMPQUERY_DEBUG_
        const char *log = "[SAMPQUERY LOG] sampquery_setup_header: Successfully setup socket";
        puts(log);
#endif
    }

    return 1;
}

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

int sampquery_request(sampquery_header_t header, enum E_SAMPQUERY_REQUEST_TYPE request_type, void *response)
{
    char buffer[SAMPQUERY_REQUEST_LEN];
    sampquery_make_pkt(buffer, header, request_type);
    ssize_t bytes_sent = sendto(header.sock_fd, buffer, SAMPQUERY_REQUEST_LEN, 0, (struct sockaddr *)&header.sock_address, header.sock_len);
    if (bytes_sent < 0)
    {
        const char *fmt = "sampquery_request: Error on sending packet - %s\n";
        printf(fmt, strerror(errno));
        return 0;
    }
    else if (bytes_sent == 0)
    {
        const char *fmt = "sampquery_request: Error on sending packet - %s\n";
        printf(fmt, strerror(errno));
        return 0;
    }
    else
    {
        const char *fmt = "sampquery_request: Successfully sended %d bytes to %s\n";
        printf(fmt, bytes_sent, inet_ntoa(header.sock_address.sin_addr));
    }

    char received_buffer[SAMPQUERY_RESPONSE_LEN];
    ssize_t bytes_received = recvfrom(header.sock_fd, received_buffer, SAMPQUERY_RESPONSE_LEN, 0, NULL, NULL);

    if (bytes_received < 0 || bytes_received == 0)
    {
        const char *fmt = "sampquery_request: Error on receiving response - %s\n";
        printf(fmt, strerror(errno));
        return 0;
    }
    else
    {
        const char *fmt = "sampquery_request: Successfully received %d bytes\n";
        printf(fmt, bytes_received);
    }

    char *reader = received_buffer + SAMPQUERY_REQUEST_LEN;
    switch (request_type)
    {
    case INFORMATION_QUERY_PACKET:
    {
        sampquery_info_pkt_t *packet = (sampquery_info_pkt_t *)response;
        packet->isPassworded = *((unsigned char *)reader);
        reader += sizeof(unsigned char);

        packet->player_count = *((unsigned short *)reader);
        reader += sizeof(unsigned short);

        packet->max_players = *((unsigned short *)reader);
        reader += sizeof(unsigned short);

        packet->hostname_len = *((unsigned int *)reader);
        reader += sizeof(unsigned int);

        packet->hostname = (char *)malloc(packet->hostname_len);
        strncpy(packet->hostname, reader, packet->hostname_len);
        reader += packet->hostname_len;

        packet->gamemode_len = *((unsigned int *)reader);
        reader += sizeof(unsigned int);

        packet->gamemode = (char *)malloc(packet->gamemode_len);
        strncpy(packet->gamemode, reader, packet->gamemode_len);
        reader += packet->gamemode_len;

        packet->language_len = *((unsigned int *)reader);
        reader += sizeof(unsigned int);

        packet->language = (char *)malloc(packet->language_len);
        strncpy(packet->language, reader, packet->language_len);
        reader += packet->language_len;
        break;
    }

    default:
        break;
    }

    return 1;
}