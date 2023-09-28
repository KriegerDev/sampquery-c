# SAMPQUERY C
    Simple API for send SA-MP queries with C Language.


## Download
Download content from lib/ and bin/ or download a release.

## Build
```bash
gcc -lsampquery [yoursources] -o [youroutput]
```

## CMake
```bash
target_link_libraries(yourtarget
    "sampquery"
)
```

## Usage

```c
// Getting server info...

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
```

## Building sampquery-c

```bash
git clone https://github.com/KriegerDev/sampquery-c.git 
cd sampquery-c
make
```