#include "sampquery.h"

int setup_sampquery_client(sampquery_client_t *client, const char *ip_address, const uint16_t port)
{
    int ipsize = sizeof(ip_address) + 1;
    client->ip_address = (char *)malloc(ipsize);
    strncpy(client->ip_address, ip_address, ipsize);

    client->port = port;

    client->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client->socket_fd < 0)
    {
        char error[200];
        sprintf(error, "setup_sampquery_client:Failed to create client socket");
        sampquery_log(error, 0);
        return 0;
    }

    memset(&client->socket_address, 0, sizeof(client->socket_address));

    client->socket_address.sin_family = AF_INET;
    client->socket_address.sin_port = htons(port);
    if (inet_aton(ip_address, &client->socket_address.sin_addr) < 0)
    {
        sampquery_log("setup_sampquery_client:Failed to resolve server address", 0);
        return 0;
    }

    client->socket_len = sizeof(client->socket_address);
    return 1;
}

int setup_sampquery_request(sampquery_request_t *request_addr, sampquery_client_t client, enum E_SAMPQUERY_REQUEST_TYPE queryPacketType)
{
    request_addr->ip_address = (char *)malloc(sizeof(client.ip_address));
    strncpy(request_addr->ip_address, client.ip_address, sizeof(client.ip_address));

    request_addr->port = client.port;

    request_addr->queryPacketType = queryPacketType;

    request_addr->data[0] = 'S';
    request_addr->data[1] = 'A';
    request_addr->data[2] = 'M';
    request_addr->data[3] = 'P';

    int ip[4];
    sscanf(client.ip_address, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);

    request_addr->data[4] = (unsigned char)ip[0];
    request_addr->data[5] = (unsigned char)ip[1];
    request_addr->data[6] = (unsigned char)ip[2];
    request_addr->data[7] = (unsigned char)ip[3];

    request_addr->data[8] = (unsigned char)(client.port & 0xFF);
    request_addr->data[9] = (unsigned char)(client.port >> 8 & 0xFF);

    request_addr->data[10] = (unsigned char)queryPacketType;

    return 1;
}

int setup_sampquery_server(sampquery_server_t *server_addr, const char *ip, const uint16_t port, sampquery_sampsvr_t *sampsvr)
{
    server_addr->port = port;
    server_addr->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_addr->socket_fd < 0)
    {
        sampquery_log("setup_sampquery_server:Failed to create server socket", 0);
        return 0;
    }

    memset(&server_addr->socket_address, 0, sizeof(server_addr->socket_address));
    server_addr->socket_address.sin_port = htons(port);
    server_addr->socket_address.sin_family = AF_INET;
    if (inet_aton(ip, &server_addr->socket_address.sin_addr) < 0)
    {
        sampquery_log("setup_sampquery_server:Failed to resolve server address", 0);
        return 0;
    }

    server_addr->sock_len = sizeof(server_addr->socket_address);

    server_addr->shutdown = 0;

    if (bind(server_addr->socket_fd, (struct sockaddr *)&server_addr->socket_address, server_addr->sock_len) < 0)
    {
        sampquery_log("setup_sampquery_server:Failed to bind server socket", 0);
        return 0;
    }
    else
    {
        sampquery_log("setup_sampquery_server:Successfully binded server socket", 1);
    }

    if (sampsvr != NULL)
    {
    }
    return 1;
}

int setup_sampquery_response(sampquery_response_t *response_addr, char *client_ip, uint16_t client_port, char data[SAMPQUERY_BASE_PACKET_LEN])
{
    int client_ip_size = sizeof(client_ip);
    response_addr->client_ip = (char *)malloc(client_ip_size);
    strncpy(response_addr->client_ip, client_ip, client_ip_size);

    response_addr->client_port = client_port;

    memset(&response_addr->client_address, 0, sizeof(response_addr));
    response_addr->client_address.sin_port = htons(client_port);
    response_addr->client_address.sin_family = AF_INET;
    if (inet_aton(client_ip, &response_addr->client_address.sin_addr) < 0)
    {
        sampquery_log("setup_sampquery_response:Failed to resolve client address", 0);
        return 0;
    }
    else
    {
        sampquery_log("setup_sampquery_response:Successfully resolved client address", 1);
    }

    response_addr->client_len = sizeof(response_addr->client_address);
    return 1;
}

int sampquery_server_listen(sampquery_server_t *server_addr, sampquery_onRequest_t callback)
{
    server_addr->callback = callback;
    pthread_create(&server_addr->thread, NULL, sampquery_callback_listen, (void *)server_addr);
    pthread_detach(server_addr->thread);
    return 1;
}

void *sampquery_callback_listen(void *args)
{
    sampquery_server_t *server = (sampquery_server_t *)malloc(sizeof(sampquery_server_t));
    server = (sampquery_server_t *)args;
    while (server->shutdown == 0)
    {
        sampquery_response_t response;

        struct sockaddr_in client_address;
        socklen_t client_len;

        struct sockaddr client_addr;

        char buffer[SAMPQUERY_BASE_PACKET_LEN];
        int res = recvfrom(server->socket_fd, buffer, SAMPQUERY_BASE_PACKET_LEN, 0, &client_addr, &client_len);
        if (res <= 0)
        {
            sampquery_log("sampquery_callback_listen:Tried to received packets but failed", 0);
            return 0;
        }
        else
        {
            char log[200];
            sprintf(log, "sampquery_callback_listen:Successfully received %d bytes", res);
            sampquery_log(log, 1);
        }
        client_address = *((struct sockaddr_in *)&client_addr);

        setup_sampquery_response(&response, inet_ntoa(client_address.sin_addr), client_address.sin_port, buffer);
        sampquery_log("Calling callback", 1);
        if (server->callback(response) == NULL)
        {
            sampquery_log("callback error", 0);
        }
    }
}

int sampquery_request(sampquery_request_t *request, sampquery_client_t client, char response[SAMPQUERY_RESPONSE_LEN])
{
    int resp = sendto(client.socket_fd, request->data, SAMPQUERY_BASE_PACKET_LEN, 0, (struct sockaddr *)&client.socket_address, sizeof(client.socket_address));
    if (resp <= 0)
    {
        char error[200];
        sprintf(error, "sampquery_request:An error ocurred while sending packets to %s:%d", client.ip_address, client.port);
        sampquery_log(error, 0);
        return 0;
    }
    else
    {
        char log[200];
        sprintf(log, "sampquery_request:Successfully sended %d bytes", resp);
        sampquery_log(log, 1);
    }

    resp = recvfrom(client.socket_fd, response, SAMPQUERY_RESPONSE_LEN, 0, NULL, NULL);
    if (resp <= 0)
    {
        char error[200];
        sprintf(error, "sampquery_request:Tried to received packets off %s:%d but failed", client.ip_address, client.port);
        sampquery_log(error, 0);
        return 0;
    }
    else
    {
        char log[200];
        sprintf(log, "sampquery_request:Successfully received %d bytes.", resp);
        sampquery_log(log, 1);
    }

    return 1;
}

int sampquery_read_information_packet(char response[SAMPQUERY_RESPONSE_LEN], struct sampquery_information_packet *serverInformation)
{
    if (response[10] != 'i')
    {
        sampquery_log("sampquery_read_information_packet:Tried to read a invalid 'sampquery_information_packet' data", 0);
        return 0;
    }

    char *reader = response + SAMPQUERY_BASE_PACKET_LEN;

    if (reader == NULL)
    {
        sampquery_log("sampquery_read_information_packet:Tried to read a invalid 'sampquery_information_packet' data", 0);
        return 0;
    }

    serverInformation->isPassworded = *((unsigned char *)reader);
    reader += sizeof(unsigned char);

    serverInformation->player_count = *((unsigned short *)reader);
    reader += sizeof(unsigned short);

    serverInformation->max_players = *((unsigned short *)reader);
    reader += sizeof(unsigned short);

    serverInformation->hostname_len = *((unsigned int *)reader);
    reader += sizeof(unsigned int);

    serverInformation->hostname = (char *)malloc(sizeof(serverInformation->hostname_len));
    strncpy(serverInformation->hostname, reader, serverInformation->hostname_len);
    reader += serverInformation->hostname_len;

    serverInformation->gamemode_len = *((unsigned int *)reader);
    reader += sizeof(unsigned int);

    serverInformation->gamemode = (char *)malloc(sizeof(serverInformation->gamemode_len));
    strncpy(serverInformation->gamemode, reader, serverInformation->gamemode_len);
    reader += serverInformation->gamemode_len;

    serverInformation->language_len = *((unsigned int *)reader);
    reader += sizeof(unsigned int);

    serverInformation->language = (char *)malloc(sizeof(serverInformation->language));
    strncpy(serverInformation->language, reader, serverInformation->language_len);
    reader += serverInformation->language_len;

    return 1;
}

void sampquery_log(const char *log, unsigned char debug)
{
    if (!debug)
    {
        char error[512];
        sprintf(error, "[SAMPQUERY : ERROR] %s\n", log);
        perror(error);
    }
#ifdef _SAMPQUERY_DEBUG_
    char logstr[512];
    sprintf(logstr, "[SAMPQUERY : DEBUG] %s\n", log);
    puts(logstr);
#endif
}

unsigned char sampquery_getpackettype(char buffer[SAMPQUERY_RESPONSE_LEN])
{
    return (unsigned char)buffer[10];
}