#include "internal/sq_commom.h"
#include "sq_header.h"
#include "internal/sq_debug.h"

int sampquery_setup_header(sampquery_header_t *sq_header, const char *sq_ipaddress, const unsigned short sq_port)
{
    memset(&sq_header->sock_address, 0, sizeof(sq_header->sock_address));
    sq_header->sock_address.sin_family = AF_INET;
    sq_header->sock_address.sin_port = htons(sq_port);

    int result = inet_pton(AF_INET, sq_ipaddress, &sq_header->sock_address.sin_addr);
    if (result == 1)
    {
        sampquery_log(1, "[SAMPQUERY LOG] Successfully setup sock_address");
    }
    else if (result == 0)
    {
        sampquery_log(0, "[SAMPQUERY LOG] sampquery_setup_header: Error on setup header - %s\n", strerror(errno));
        return 0;
    }
    else
    {
        sampquery_log(0, "[SAMPQUERY LOG] sampquery_setup_header: Error on setup header - %s\n", strerror(errno));
        return 0;
    }

    sq_header->sock_len = sizeof(sq_header->sock_address);
    sq_header->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sq_header->sock_fd < 0)
    {
        sampquery_log(0, "[SAMPQUERY LOG] sampquery_setup_header: Error on setup socket - %s", strerror(errno));
        return 0;
    }
    else
    {
        sampquery_log(1, "[SAMPQUERY LOG] sampquery_setup_header: Successfully setup socket");
    }

    return 1;
}