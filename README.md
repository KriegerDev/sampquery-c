# SAMPQUERY C
    Simple API for send SA-MP queries with C Language.

## Build
```bash
gcc sampquery.c [yoursources] -o [youroutput]
```
## Usage

### Enums
- **`E_SAMPQUERY_REQUEST_TYPE`**
### Types
- **`sampquery_client_t`**
    - use `setup_sampquery_client` to create.
- **`sampquery_request_t`**
    - use `setup_sampquery_request` to create.

### Functions
- **`setup_sampquery_client`**
    - Initialize the `sampquery_client_t` type.
    - View:

        - ```c 
            int setup_sampquery_client(sampquery_client_t *client, const char *ip_address, const uint16_t port);

            sampquery_client_t *client; // Pointer to client.
            const char *ip_address; // Server IP address.
            const uint16_t port; // Server port.
            ```

- **`setup_sampquery_request`**
    - Initialize the `sampquery_request_t` type.
    - View:

        - ```c 
            int setup_sampquery_request(sampquery_request_t *request_addr, sampquery_client_t client, enum E_SAMPQUERY_REQUEST_TYPE queryPacketType);

            sampquery_request_t *request_addr; // Pointer to request.
            sampquery_client_t client; // Client to execute.
            enum E_SAMPQUERY_REQUEST_TYPE queryPacketType; // Request type.
            ```

- **`sampquery_request`**
    - Perform request.
    - View:

        - ```c 
            int sampquery_request(sampquery_request_t *request, sampquery_client_t client, char response[SAMPQUERY_RESPONSE_LEN]);

            sampquery_request_t *request; // Pointer to request.
            sampquery_client_t client; // Client to execute.
            char response[SAMPQUERY_RESPONSE_LEN]; // Request response.
            ```

- **`sampquery_read_information_packet`**
    - Read response from request, and fill `sampquery_read_information_packet`.
    - View:

        - ```c 
            int sampquery_read_information_packet(char response[SAMPQUERY_RESPONSE_LEN], struct sampquery_information_packet *serverInformation);

            char response[SAMPQUERY_RESPONSE_LEN]; // Response from request.
            struct sampquery_information_packet *serverInformation; // Pointer to sampquery_information_packet struct
            ```    

- **`sampquery_log`**
    - API internal log.
    - View:

        - ```c 
            void sampquery_log(const char *log, unsigned char debug);

            const char *log; // String to log.
            unsigned char debug; // debug = 1 | Error = 0
            ```            