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
#include <errno.h>

// Request len
#define SAMPQUERY_REQUEST_LEN 11
// Response len
#define SAMPQUERY_RESPONSE_LEN 512

// Request type
enum E_SAMPQUERY_REQUEST_TYPE
{
    INFORMATION_QUERY_PACKET = 0x69,
    RULES_QUERY_PACKET = 0x72,
    DETAILED_QUERY_PACKET = 0x64,
    CLIENTLIST_QUERY_PACKET = 0x63,
    RCON_QUERY_PACKET = 0x78
};

typedef struct sampquery_info_pkt
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
} sampquery_info_pkt_t;

typedef struct sampquery_header
{
    struct sockaddr_in sock_address;
    socklen_t sock_len;
    int sock_fd;
} sampquery_header_t;

int sampquery_setup_header(sampquery_header_t *sq_header, const char *sq_hostname, const unsigned short sq_port);
void sampquery_make_pkt(char data[SAMPQUERY_REQUEST_LEN], sampquery_header_t header, unsigned char opcode);

int sampquery_request(sampquery_header_t header, enum E_SAMPQUERY_REQUEST_TYPE request_type, void *response);

int sampquery_respond(sampquery_header_t header, enum E_SAMPQUERY_REQUEST_TYPE request_type);

#endif