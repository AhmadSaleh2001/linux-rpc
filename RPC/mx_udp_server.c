#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "rpc_common.h"
#include "serialized_buffer_packet.h"

#define TCP_SERVER_MAX_CLIENT_SUPPORTED 32

char data_buffer[MAX_SEND_RECV_SEGMENT_SIZE];

int monitored_fd_set[32];

static void
intitiaze_monitor_fd_set(){

    int i = 0;
    for(; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++)
        monitored_fd_set[i] = -1;
}

static void 
add_to_monitored_fd_set(int skt_fd){

    int i = 0;
    for(; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++){

        if(monitored_fd_set[i] != -1)
            continue;   
        monitored_fd_set[i] = skt_fd;
        break;
    }
}

static void
remove_from_monitored_fd_set(int skt_fd){

    int i = 0;
    for(; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++){

        if(monitored_fd_set[i] != skt_fd)
            continue;

        monitored_fd_set[i] = -1;
        break;
    }
}

static void
re_init_readfds(fd_set *fd_set_ptr){

    FD_ZERO(fd_set_ptr);
    int i = 0;
    for(; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++){
        if(monitored_fd_set[i] != -1){
            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

static int 
get_max_fd(){

    int i = 0;
    int max = -1;

    for(; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++){
        if(monitored_fd_set[i] > max)
            max = monitored_fd_set[i];
    }

    return max;
}

static uint32_t
tcp_ip_covert_ip_p_to_n(char *ip_addr){

    uint32_t binary_prefix = 0;
    inet_pton(AF_INET, ip_addr, &binary_prefix);
    binary_prefix = htonl(binary_prefix);
    return binary_prefix;
}

int multiply_server_stub(serialized_buffer_t client_data) {
    int received_a, received_b;
    deserialize_data(&client_data, &received_a, sizeof(int));
    deserialize_data(&client_data, &received_b, sizeof(int));
    return received_a * received_b;
}

void server_serialize_result(int result, serialized_buffer_t * server_send_response) {
    serialize_data(server_send_response, &result, sizeof(int));
    server_send_response->next = 0;
}

void rpc_server_process_message(serialized_buffer_t client_data, serialized_buffer_t * server_send_response) {
    int result = multiply_server_stub(client_data);
    server_serialize_result(result, server_send_response);
}

void
setup_udp_server_communication(){

    printf("UDP Server instantiated %s:%d\n", SERVER_IP, SERVER_PORT);

    /*Step 1 : Initialization*/
    /*Socket handle and other variables*/
    int master_sock_udp_fd = 0, /*Master socket file descriptor, used to accept new client connection only, no data exchange*/
        sent_recv_bytes = 0, 
        addr_len = 0, 
        opt = 1;

    int comm_socket_fd = 0;     /*client specific communication socket file descriptor, used for only data exchange/communication between client and server*/
    fd_set readfds;             /*Set of file descriptor on which select() polls. Select() unblocks whever data arrives on any fd present in this set*/
    /*variables to hold server information*/
    struct sockaddr_in server_addr, /*structure to store the server and client info*/
                       client_addr;

    /* Just drain the array of monitored file descriptors (sockets)*/
    intitiaze_monitor_fd_set();

    /*step 2: tcp master socket creation*/
    if ((master_sock_udp_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP )) == -1)
    {
        printf("socket creation failed\n");
        exit(1);
    }

    /*Step 3: specify server Information*/
    server_addr.sin_family = AF_INET;/*This socket will process only ipv4 network packets*/
    server_addr.sin_port = htons(SERVER_PORT);/*Server will process any data arriving on port no 2000*/
    server_addr.sin_addr.s_addr = htonl(tcp_ip_covert_ip_p_to_n(SERVER_IP)); 

    addr_len = sizeof(struct sockaddr);

    if (setsockopt(master_sock_udp_fd, SOL_SOCKET, 
			    SO_REUSEADDR, (char *)&opt, sizeof(opt))<0) {
	printf("setsockopt Failed\n");
	exit(0);
    }

    if (setsockopt(master_sock_udp_fd, SOL_SOCKET,
			    SO_REUSEPORT, (char *)&opt, sizeof(opt))<0) {
	printf("setsockopt Failed\n");
	exit(0);
    }

    /* Bind the server. Binding means, we are telling kernel(OS) that any data 
     * you recieve with dest ip address = 192.168.56.101, and tcp port no = 2000, pls send that data to this process
     * bind() is a mechnism to tell OS what kind of data server process is interested in to recieve. Remember, server machine
     * can run multiple server processes to process different data and service different clients. Note that, bind() is 
     * used on server side, not on client side*/

    if (bind(master_sock_udp_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("socket bind failed\n");
        return;
    }

	
    /*Add master socket to Monitored set of FDs*/
    add_to_monitored_fd_set(master_sock_udp_fd);

    /* Server infinite loop for servicing the client*/


    while(1){

        /*Step 5 : initialze and dill readfds*/
        FD_ZERO(&readfds);                     /* Initialize the file descriptor set*/
        re_init_readfds(&readfds);               /*Copy the entire monitored FDs to readfds*/

        printf("blocked on select System call...\n");

        /*Step 6 : Wait for client connection*/
        /*state Machine state 1 */
        select(get_max_fd() + 1, &readfds, NULL, NULL, NULL); /*Call the select system cal, server process blocks here. Linux OS keeps this process blocked untill the data arrives on any of the file Drscriptors in the 'readfds' set*/

        /*Some data on some fd present in monitored fd set has arrived, Now check on which File descriptor the data arrives, and process accordingly*/

        /*If Data arrives on master socket FD*/
        if (FD_ISSET(master_sock_udp_fd, &readfds))
        { 
            memset(data_buffer, 0, sizeof(data_buffer));
            sent_recv_bytes = recvfrom(master_sock_udp_fd, (char *)data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client_addr, &addr_len);

            printf("Server recvd %d bytes from client %s:%u\n", sent_recv_bytes,
                    inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
            serialized_buffer_t client_data = deserialize_packet(data_buffer);
            serialized_buffer_t * server_send_result_buffer = NULL;
            init_serializd_buffer(&server_send_result_buffer, MAX_SEND_RECV_SEGMENT_SIZE);
            rpc_server_process_message(client_data, server_send_result_buffer);

            serialized_buffer_packet_t * server_send_packet = create_serialized_packet(server_send_result_buffer);

            printf("bytes to send: %d\n", sizeof(serialized_buffer_packet_t) + server_send_result_buffer->size);
            
            /* Server replying back to client now*/
            sent_recv_bytes = sendto(master_sock_udp_fd, server_send_packet, sizeof(serialized_buffer_packet_t) + server_send_result_buffer->size, 0,
                                        (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

            printf("Server sent %d bytes in reply to client\n", sent_recv_bytes);

            close(master_sock_udp_fd);
            printf("Server closes connection with client : %s:%u\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            /*Goto state machine State 1*/
            break;/*Get out of inner while loop, server is done with this client, time to check for new connection request by executing selct()*/
        }
    }
}

int
main(int argc, char **argv){

    setup_udp_server_communication();
    return 0;
}
