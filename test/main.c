#include <stdio.h>
#include <stdlib.h>
#include "sampquery.h"

#define PORT 7777
int main(void)
{
    sampquery_info_pkt_t server_info;
    sampquery_header_t header;
    sampquery_setup_header(&header, "IP Address", PORT);
    sampquery_request(header, INFORMATION_QUERY_PACKET, &server_info);

    printf("Server %s\nGamemode: %s\nLanguage: %s\nMax players: %d\nOnline players: %d\n",
           server_info.hostname, server_info.gamemode, server_info.language, server_info.max_players, server_info.player_count);
    return 0;
}