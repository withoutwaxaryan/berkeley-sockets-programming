// Echo server takes a string from the client as input and echoes it back to the client.
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<strings.h>
#include<unistd.h>
#include<arpa/inet.h>


#define ERROR     -1
#define MAX_CLIENTS  2
#define MAX_DATA   1024


int main(int argc, char **argv)
{
    struct sockaddr_in server;
    struct sockaddr_in client;
    int sock;
    int new;
    int sockaddr_len = sizeof(struct sockaddr_in);
    int data_len;
    char data[MAX_DATA];

    if((sock = socket(AF_INET, SOCK_STREAM, 0))== -1) // error is present
    {
        perror("SERVER socket: ");
        exit(-1);
    }
    // Server Data Structure
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1])); // convert to Network Byte order
    server.sin_addr.s_addr = INADDR_ANY; //Server should bind to all available interfaces on the machine --> 0.0.0.0
    bzero(&server.sin_zero, 8);

    if((bind(sock, (struct sockaddr *)&server, sockaddr_len)) == ERROR)
    {
        perror("bind: ");
        exit(-1);
    }

    // listen on the socket
    if((listen(sock, MAX_CLIENTS)) == ERROR)
    {
        perror("listen");
        exit(-1);
    }

    while(1) // Better signal handling --> Echoing Mechanism
    {
        if((new = accept(sock, (struct sockaddr *)&client, &sockaddr_len)) == ERROR)
        {
            perror("accept");
            exit(-1);
        }
        printf("New Client connected from port no %d and IP %s \n", ntohs(client.sin_port), inet_ntoa(client.sin_addr));
        data_len = 1;
        while(data_len)  // WHILE THE CONNECTION IS present
        {
            // SInce this is an echo server, first we will receive data from client.
            data_len = recv(new, data, MAX_DATA, 0); // data recv becomes -1 when the connection is closed, so that it can get back to the upper while & accept statement.
            // Now we send back the same data to the client
            if(data_len)
            {
                send(new, data, data_len, 0);
                data[data_len] = '\0'; // we null the last byte to tell that the string is over.
                printf("Sent Mesg: %s", data);
            }
        }
        printf("Client Disconnected\n");
        close(new); // close the socket
    }
}
/* In one terminal run the file : gcc EchoServer.c -o EchoServer
   followed by ./EchoServer 10000
   In another terminal check the tcp connections using netstat -atn
   Use telnet localhost 10000 to connect client to server
   The first terminal should say that new client has been connected along with its port and ip address
   In the second terminal u can now write down your message, and the server will echo it back
   To quit the connection do "ctrl + ]" and then type quit
   The connection will be disconnected.
   */
   