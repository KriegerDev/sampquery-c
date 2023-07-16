#include <stdio.h>
#include <stdlib.h>
#include "sampquery.h"

void callback(sampquery_response_t response)
{
    printf("New Request");
}

int main(void)
{
    sampquery_server_t server;
    setup_sampquery_server(&server, "172.17.78.33", 3034);
    sampquery_server_listen(&server, callback);
    while (server.shutdown == 0)
    {
    }
    return 0;
}