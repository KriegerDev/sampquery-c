#ifndef _SAMPQUERY_H_
#define _SAMPQUERY_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define SAMPQUERY_BASE_PACKET_LEN 11
#define SAMPQUERY_RESPONSE_LEN 512

enum E_SAMPQUERY_REQUEST_TYPE
{
    INFORMATION_QUERY_PACKET = 0x69,
    RULES_QUERY_PACKET = 0x72,
    DETAILED_QUERY_PACKET = 0x64,
    CLIENTLIST_QUERY_PACKET = 63,
    RCON_QUERY_PACKET = 0x78
};

struct sampquery_information_packet
{
    uint8_t isPassworded;
    uint16_t player_count;
    uint16_t max_players;
    int hostname_len;
    char *hostname;
    int gamemode_len;
    char *gamemode;
    int language_len;
    char *language;
};

typedef struct sampquery_request
{
    char *ip_address;
    uint16_t port;
    enum E_SAMPQUERY_REQUEST_TYPE queryPacketType;
    char data[SAMPQUERY_BASE_PACKET_LEN];
} sampquery_request_t;

typedef struct sampquery_client
{
    char *ip_address;
    uint16_t port;
    int socket_fd;
    struct sockaddr_in socket_address;
    socklen_t socket_len;
} sampquery_client_t;

int setup_sampquery_client(sampquery_client_t *client, const char *ip_address, const uint16_t port);
int setup_sampquery_request(sampquery_request_t *request_addr, sampquery_client_t client, enum E_SAMPQUERY_REQUEST_TYPE queryPacketType);
int sampquery_request(sampquery_request_t *request, sampquery_client_t client, char response[SAMPQUERY_RESPONSE_LEN]);
int sampquery_read_information_packet(char response[SAMPQUERY_RESPONSE_LEN], struct sampquery_information_packet *serverInformation);

void sampquery_log(const char *log, unsigned char debug);

#endif