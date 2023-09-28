#ifndef _SAMPQUERY_PACKET_H_
#define _SAMPQUERY_PACKET_H_
#include "sq_request.h"
#include "sq_header.h"

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

void sampquery_make_pkt(char data[SAMPQUERY_REQUEST_LEN], sampquery_header_t header, unsigned char opcode);
#endif