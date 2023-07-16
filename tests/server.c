#include <stdio.h>
#include <stdlib.h>
#include "../sampquery.h"

void *callback(sampquery_response_t response)
{
    printf("New query request from %s\n", inet_ntoa(response.client_address.sin_addr));
    return (void *)1;
}

int main(void)
{
    sampquery_server_t server;
    setup_sampquery_server(&server, "172.17.78.33", 3034, NULL);
    sampquery_server_listen(&server, callback);
    while (server.shutdown == 0)
    {
    }
    return 0;
}