# SAMPQUERY C
    Simple API for send SA-MP queries with C Language.

## Build
```bash
gcc sampquery.c [yoursources] -o [youroutput]
```
## Usage

```c
// Getting server info...

#include <stdio.h>
#include <stdlib.h>
#include "../sampquery.h"

int main(void)
{
    sampquery_info_pkt_t server_info;
    sampquery_header_t header;
    sampquery_setup_header(&header, "149.56.41.53", 7774);
    sampquery_request(header, INFORMATION_QUERY_PACKET, &server_info);

    printf("Server %s\nGamemode: %s\nLanguage: %s\nMax players: %d\nOnline players: %d\n",
           server_info.hostname, server_info.gamemode, server_info.language, server_info.max_players, server_info.player_count);
    return 0;
}
```