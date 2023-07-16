#include <stdio.h>
#include <stdlib.h>
#include "../sampquery.h"

int main(void)
{
    sampquery_client_t client;
    sampquery_request_t request;

    setup_sampquery_client(&client, "172.17.78.33", 3034);
    setup_sampquery_request(&request, client, INFORMATION_QUERY_PACKET);

    char queryResponse[SAMPQUERY_RESPONSE_LEN];
    sampquery_request(&request, client, queryResponse);

    struct sampquery_information_packet packet;
    sampquery_read_information_packet(queryResponse, &packet);

    printf("Server information of %s:%d\nHostname: %s\nGamemode: %s\nLanguage: %s\nMax players: %d\nOnline players: %d\n",
           client.ip_address, client.port, packet.hostname, packet.gamemode, packet.language, packet.max_players, packet.player_count);

    return 0;
}