#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "serialized_buffer.h"
#include "serialized_buffer_packet.h"
#include "rpc_common.h"

char data_buffer[MAX_SEND_RECV_SEGMENT_SIZE];

void rpc_send_recv(serialized_buffer_t * client_send_buffer, serialized_buffer_t * client_recv_buffer) {
    /*Step 1 : Initialization*/
    /*Socket handle*/
    int sockfd = 0, 
        sent_recv_bytes = 0;

    int addr_len = 0;

    addr_len = sizeof(struct sockaddr);

    /*to store socket addesses : ip address and port*/
    struct sockaddr_in dest;

    /*Step 2: specify server information*/
    /*Ipv4 sockets, Other values are IPv6*/
    dest.sin_family = AF_INET;

    /*Client wants to send data to server process which is running on server machine, and listening on 
     * port on DEST_PORT, server IP address SERVER_IP_ADDRESS.
     * Inform client about which server to send data to : All we need is port number, and server ip address. Pls note that
     * there can be many processes running on the server listening on different no of ports, 
     * our client is interested in sending data to server process which is lisetning on PORT = DEST_PORT*/ 
    dest.sin_port = htons(SERVER_PORT);
    struct hostent *host = (struct hostent *)gethostbyname(SERVER_IP);
    dest.sin_addr = *((struct in_addr *)host->h_addr);


    /*Step 3 : Create a TCP socket*/
    /*Create a socket finally. socket() is a system call, which asks for three paramemeters*/
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = inet_addr(CLIENT_IP);
    localaddr.sin_port = htons(CLIENT_PORT);  // Any local port will do
    bind(sockfd, (struct sockaddr *)&localaddr, sizeof(localaddr));

    /*Step 4 : get the data to be sent to server*/
    /*Our client is now ready to send data to server. sendto() sends data to Server*/

    serialized_buffer_packet_t *packet = create_serialized_packet(client_send_buffer);

    /*step 5 : send the data to server*/
    sent_recv_bytes = sendto(sockfd, 
           packet,
           sizeof(serialized_buffer_packet_t) + client_send_buffer->size, 
           0, 
           (struct sockaddr *)&dest, 
           sizeof(struct sockaddr));
    
    printf("No of bytes sent = %d\n", sent_recv_bytes);

    sent_recv_bytes = recvfrom(sockfd, (char *)&data_buffer, sizeof(serialized_buffer_packet_t) + client_send_buffer->size, 0,
                    (struct sockaddr *)&dest, &addr_len);

    *client_recv_buffer = deserialize_packet(data_buffer);
    printf("No of bytes recvd = %d\n", sent_recv_bytes);
}

serialized_buffer_t * client_multiply_marshall(int a, int b, int operationType) {
    serialized_buffer_t * client_send_buffer = NULL;
    init_serializd_buffer(&client_send_buffer, MAX_SEND_RECV_SEGMENT_SIZE);
    OPERATIONS op;
    if(operationType == 1)op = ADD;
    else if(operationType == 2)op = SUBTRACT;
    else if(operationType == 3)op = MULTIPLY;
    else if(operationType == 4)op = DIVISION;

    client_send_buffer->rpc_header.op = op;
    serialize_data(client_send_buffer, &a, sizeof(int));
    serialize_data(client_send_buffer, &b, sizeof(int));
    return client_send_buffer;
}

int operation_rpc(int a, int b, int operationType) {
    
    // serialzie data
    serialized_buffer_t * client_send_buffer = client_multiply_marshall(a, b, operationType);
    client_send_buffer->next = 0;

    serialized_buffer_t * client_recv_buffer = NULL;
    init_serializd_buffer(&client_recv_buffer, MAX_SEND_RECV_SEGMENT_SIZE);


    // send data through network
    rpc_send_recv(client_send_buffer, client_recv_buffer);

    // recieve the data

    int res = 0;
    deserialize_data(client_recv_buffer, &res, sizeof(int));
    free_serialize_buffer(client_send_buffer);
    free_serialize_buffer(client_recv_buffer);

    return res;
}

int main(int argc, char ** argv) {

    int a, b;
    printf("enter a:");
    scanf("%d", &a);

    printf("enter b:");
    scanf("%d", &b);

    int operationType;
    printf("1- add\n");
    printf("2- subtract\n");
    printf("3- multiply\n");
    printf("4- division\n");
    scanf("%d", &operationType);
    int res = operation_rpc(a, b, operationType);

    printf("res: %d\n", res);

    return 0;
}
