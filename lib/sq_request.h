#ifndef _SAMPQUERY_REQUEST_H_
#define _SAMPQUERY_REQUEST_H_
#include "sq_header.h"

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

int sampquery_request(sampquery_header_t header, enum E_SAMPQUERY_REQUEST_TYPE request_type, void *response);
int sampquery_respond(sampquery_header_t header, enum E_SAMPQUERY_REQUEST_TYPE request_type);
#endif