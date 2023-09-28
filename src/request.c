#include "sq_commom.h"
#include "sq_request.h"
#include "sq_packet.h"
#include "internal/sq_debug.h"

int sampquery_request(sampquery_header_t header, enum E_SAMPQUERY_REQUEST_TYPE request_type, void *response)
{
    char buffer[SAMPQUERY_REQUEST_LEN];
    sampquery_make_pkt(buffer, header, request_type);
    ssize_t bytes_sent = sendto(header.sock_fd, buffer, SAMPQUERY_REQUEST_LEN, 0, (struct sockaddr *)&header.sock_address, header.sock_len);
    if (bytes_sent < 0)
    {
        sampquery_log(0, "sampquery_request: Error on sending packet - %s\n", strerror(errno));
        return 0;
    }
    else if (bytes_sent == 0)
    {
        sampquery_log(0, "sampquery_request: Error on sending packet - %s\n", strerror(errno));
        return 0;
    }
    else
    {
        sampquery_log(1, "sampquery_request: Successfully sended %d bytes to %s\n", bytes_sent, inet_ntoa(header.sock_address.sin_addr));
    }

    char received_buffer[SAMPQUERY_RESPONSE_LEN];
    ssize_t bytes_received = recvfrom(header.sock_fd, received_buffer, SAMPQUERY_RESPONSE_LEN, 0, NULL, NULL);

    if (bytes_received < 0 || bytes_received == 0)
    {
        sampquery_log(0, "sampquery_request: Error on receiving response - %s\n", strerror(errno));
        return 0;
    }
    else
    {
        sampquery_log(1, "sampquery_request: Successfully received %d bytes\n", bytes_received);
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